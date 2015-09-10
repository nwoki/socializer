/*
 * oauth.cpp
 *
 * This file is part of the Socializer library
 *
 * Author Francesco Nwokeka <francesco.nwokeka@gmail.com>
 */

#include "oauth.h"


#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <stdio.h>

#include <QtCore/QCryptographicHash>
#include <QtCore/QDateTime>
#include <QtCore/QDebug>

#ifdef USING_QT5
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonParseError>
#else
#include <qjson/parser.h>
#endif

#include <QtNetwork/QNetworkRequest>

#define NONCE_SIZE 16

using namespace Socializer;

OAuth::OAuth(const QByteArray &appId, const QByteArray &redirectUrl, const QByteArray &consumerSecret, QObject *parent)
    : QObject(parent)
    , m_appId(appId)
    , m_consumerSecret(consumerSecret)
    , m_redirectUrl(redirectUrl)
    , m_networkAccessManager(new QNetworkAccessManager(this))
    , m_networkReply(nullptr)
{
}


OAuth::OAuth(const QByteArray& authToken, QObject *parent)
    : QObject(parent)
    , m_appId(QByteArray())
    , m_authToken(authToken)
    , m_consumerSecret(QByteArray())
    , m_redirectUrl(QByteArray())
    , m_networkAccessManager(new QNetworkAccessManager(this))
    , m_networkReply(nullptr)
{
}


OAuth::~OAuth()
{
}


QByteArray OAuth::authToken() const
{
    return m_authToken;
}


QByteArray OAuth::generateBaseString(QNetworkAccessManager::Operation opType, const QByteArray& url, QList< QPair< QByteArray, QByteArray > > params)
{
    QByteArray baseStr;

    // Convert the HTTP Method to uppercase and set the output string equal to this value
    // and append the '&' character to the output string.
    if (opType == QNetworkAccessManager::PostOperation) {
        baseStr += "POST&";
    } else if (opType == QNetworkAccessManager::GetOperation) {
        baseStr += "GET&";
    }

    // Percent encode the URL and append it to the output string.
    baseStr += QUrl::toPercentEncoding(url);

    // Append the '&' character to the output string.
    baseStr += "&";

    // Percent encode the parameter string and append it to the output string.
    QByteArray paramStr;
    QPair<QByteArray, QByteArray> param;

    // Sort the list of parameters alphabetically
    qSort(params);

    foreach (param, params) {
        paramStr += QUrl::toPercentEncoding(param.first) + "=" + QUrl::toPercentEncoding(param.second) + "&";
    }

    // remove last "&" (not supposed to be there)
    paramStr.chop(1);

    baseStr += QUrl::toPercentEncoding(paramStr);

    return baseStr;
}


QByteArray OAuth::generateRequestHeader(QNetworkAccessManager::Operation opType, QByteArray url)
{
#ifdef DEBUG_MODE
    qDebug("[OAuth::generateRequestAccessTokenHeader]");
#endif

    QByteArray timeStamp = OAuth::timeStamp();
    QByteArray nonce = OAuth::nonce();

    // consumer secret + "&" + "requestToken
    QByteArray key(QUrl::toPercentEncoding(m_consumerSecret));
    key += "&";

    // create list of params for signing
    QList<QPair<QByteArray, QByteArray> > params;

    params.append(QPair<QByteArray, QByteArray>("oauth_nonce", nonce));
    params.append(QPair<QByteArray, QByteArray>("oauth_callback", m_redirectUrl));
    params.append(QPair<QByteArray, QByteArray>("oauth_signature_method", "HMAC-SHA1"));
    params.append(QPair<QByteArray, QByteArray>("oauth_timestamp", timeStamp));
    params.append(QPair<QByteArray, QByteArray>("oauth_consumer_key", m_appId));
    params.append(QPair<QByteArray, QByteArray>("oauth_version", "1.0"));

    // add auth token if present
    if (!m_authToken.isEmpty()) {
        params.append(QPair<QByteArray, QByteArray>("oauth_token", m_authToken));
    }

    QByteArray baseStr = generateBaseString(opType, url, params);
    QByteArray signature = hmacsha1Encode(baseStr, key);

    // and now create the header
    QByteArray header("OAuth ");

    if (!m_redirectUrl.isEmpty()) {
        header += "oauth_callback=\"" + QUrl::toPercentEncoding(m_redirectUrl) + "\",";
    } else {
        header += "oauth_callback=\"oob\",";
    }

    header += "oauth_consumer_key=\"" + m_appId + "\",";
    header += "oauth_nonce=\"" + nonce + "\",";
    header += "oauth_signature=\"" + QUrl::toPercentEncoding(signature) + "\",";
    header += "oauth_signature_method=\"HMAC-SHA1\",";
    header += "oauth_timestamp=\"" + timeStamp + "\",";

    // auth token
    if (!m_authToken.isEmpty()) {
        header += "oauth_token=\"" + m_authToken + "\",";
    }

    header += "oauth_version=\"1.0\"";

#ifdef DEBUG_MODE
    qDebug() << "[OAuth::generateRequestAccessTokenHeader] header is: " << header;
#endif

    return header;
}


QByteArray OAuth::hmacsha1Encode(const QByteArray &baseStr, QByteArray key)
{
    int blockSize = 64; // HMAC-SHA-1 block size, defined in SHA-1 standard

    if (key.length() > blockSize) { // if key is longer than block size (64), reduce key length with SHA-1 compression
        key = QCryptographicHash::hash(key, QCryptographicHash::Sha1);
    }

    QByteArray innerPadding(blockSize, char(0x36)); // initialize inner padding with char "6"
    QByteArray outerPadding(blockSize, char(0x5c)); // initialize outer padding with char "\"
    // ascii characters 0x36 ("6") and 0x5c ("\") are selected because they have large
    // Hamming distance (http://en.wikipedia.org/wiki/Hamming_distance)

    for (int i = 0; i < key.length(); i++) {
        innerPadding[i] = innerPadding[i] ^ key.at(i); // XOR operation between every byte in key and innerpadding, of key length
        outerPadding[i] = outerPadding[i] ^ key.at(i); // XOR operation between every byte in key and outerpadding, of key length
    }

    // result = hash ( outerPadding CONCAT hash ( innerPadding CONCAT baseString ) ).toBase64
    QByteArray total = outerPadding;
    QByteArray part = innerPadding;

    part.append(baseStr);
    total.append(QCryptographicHash::hash(part, QCryptographicHash::Sha1));

    QByteArray hashed = QCryptographicHash::hash(total, QCryptographicHash::Sha1);

    return hashed.toBase64();
}


#ifdef USING_QT5
QJsonObject OAuth::jsonObject(const QByteArray &jsonData)
#else
QVariantMap OAuth::jsonObject(const QByteArray& jsonData)
#endif
{
    qDebug("[OAuth::jsonObject]");

#ifdef USING_QT5
    QJsonParseError jsonError;
    QJsonDocument jsonParser = QJsonDocument::fromJson(jsonData, &jsonError);

    if (jsonError.error != QJsonParseError::NoError) {
        QString errStr("[OAuth::jsonObject] Invalid json!");
        errStr += " - " + jsonError.errorString();

        qDebug() << errStr;

        return QJsonObject();
    }

    // extract json object
    return jsonParser.object();
#else
    QJson::Parser parser;
    bool ok;
    QVariantMap jsonObj = parser.parse(jsonData, &ok).toMap();

    if (!ok) {
        qWarning("[OAuth::jsonObject] error parsing json");
        return QVariantMap();
    } else {
        return jsonObj;
    }
#endif
}


QByteArray OAuth::timeStamp()
{
    return QByteArray::number(QDateTime::currentDateTime().toTime_t());
}


QByteArray OAuth::nonce()
{
    QByteArray chars("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
    int max = chars.size();

    QByteArray nonce;

    for(int i = 0; i < NONCE_SIZE; ++i){
        nonce.append(chars[qrand() % max]);
    }

    return nonce;
}


void OAuth::obtainRequestToken(const QByteArray &requestUrl)
{
    // https://dev.twitter.com/docs/auth/implementing-sign-twitter (see step 1)
    QUrl reqUrlized(requestUrl);
    QNetworkRequest request(reqUrlized);
    request.setRawHeader("Authorization", generateRequestHeader(QNetworkAccessManager::PostOperation, requestUrl));

    if (m_networkReply != 0) {
        m_networkReply->deleteLater();
    }

    m_networkReply = m_networkAccessManager->post(request, "application/octet-stream");

    connect(m_networkReply, SIGNAL(finished()), this, SLOT(onObtainRequestTokenReplyRecieved()));
    connect(m_networkReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onNetworkErrorRecieved(QNetworkReply::NetworkError)));
}


void OAuth::onObtainRequestTokenReplyRecieved()
{
    if (!m_networkReply) {
        return;
    }

    QByteArray rcv = m_networkReply->readAll();

#ifdef DEBUG_MODE
    qDebug() << "[OAuth::onObtainRequestTokenReplyRecieved] REQUEST TOKEN RECIEVED: " << rcv;
#endif

    m_networkReply->deleteLater();

    // extract oauth_token, auth_token_secret and check if the callback is confirmed
    // split the string
    QList<QByteArray> parts = rcv.split('&');
    bool valid = true;

    foreach (QByteArray part, parts) {
        QList<QByteArray>subPart = part.split('=');

        if (subPart.at(0) == "oauth_token") {
            m_authToken = subPart.at(1);
        } else if (subPart.at(0) == "oauth_token_secret") {
            m_authTokenSecret = subPart.at(1);
        } else if (subPart.at(0) == "oauth_callback_confirmed") {
            if (subPart.at(1) != "true") {
                valid = false;
                /// TODO what to do in this case?
            }
        }
    }

#ifdef DEBUG_MODE
    qDebug() << "[OAuth::onObtainRequestTokenReplyRecieved] Got values: " << m_authToken << " " << m_authTokenSecret;
#endif

    if (valid) {
        Q_EMIT requestTokenRecieved();
    } else {
        qDebug("ERROR [OAuth::onObtainRequestTokenReplyRecieved] Callback was not comfirmed");
    }
}


void OAuth::onNetworkErrorRecieved(QNetworkReply::NetworkError error)
{
    if (!m_networkReply) {
        return;
    }

    // don't need to do check for int conversion here. I trust Qt's code ;)
    int statusCode = m_networkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QByteArray errStr = m_networkReply->readAll();

#ifdef DEBUG_MODE
    qDebug("[OAuth::onNetworkErrorRecieved]");
    qDebug() << "Error msg: " << errStr;
    qDebug() << "Status code: " << statusCode;
#endif

    m_networkReply->deleteLater();

    /// TODO implement cases
    Q_UNUSED(error)
    Q_UNUSED(errStr)
    Q_UNUSED(statusCode)
}


void OAuth::onRequestAccessTokenReceived()
{
    if (!m_networkReply) {
        return;
    }

    QByteArray rcv = m_networkReply->readAll();

#ifdef DEBUG_MODE
    qDebug("[OAuth::onRequestAccessTokenReceived]");
    qDebug() << rcv;
#endif

    m_networkReply->deleteLater();

    // extract values
    QList<QByteArray> params = rcv.split('&');

    foreach (QByteArray param, params) {
        QList<QByteArray> elements = param.split('=');

        if (elements.at(0) == "oauth_token") {
            m_authToken = elements.at(1);
        } else if (elements.at(0) == "oauth_token_secret") {
            m_authTokenSecret = elements.at(1);
        } else if (elements.at(0) == "user_id") {
            m_userId = elements.at(1);
        } else if (elements.at(0) == "screen_name") {
            m_screenName = elements.at(1);
        }
    }

#ifdef DEBUG_MODE
    qDebug("Definitive values after auth are: ");
    qDebug() << "auth token: " << m_authToken;
    qDebug() << "auth token secret: " << m_authTokenSecret;
    qDebug() << "user id: " << m_userId;
    qDebug() << "screen name: " << m_screenName;
#endif
}


QByteArray OAuth::redirectUrl() const
{
    return m_redirectUrl;
}


void OAuth::requestAccessToken(const QByteArray &url, const QByteArray &authVerifier)
{
#ifdef DEBUG_MODE
    qDebug("[OAuth::requestAccessToken]");
    qDebug() << "url: " << url;
    qDebug() << "authTok: " << m_authToken;
    qDebug() << "auth ver: " << authVerifier;
#endif



    // https://dev.twitter.com/docs/auth/implementing-sign-twitter (see step 1)
    QUrl reqUrlized(url);
    QNetworkRequest request(reqUrlized);
    QByteArray data;
    data += "oauth_verifier=";
    data += authVerifier;

    request.setRawHeader("Authorization", generateRequestHeader(QNetworkAccessManager::PostOperation, url));

    if (m_networkReply != 0) {
        m_networkReply->deleteLater();
    }

    m_networkReply = m_networkAccessManager->post(request, data);

    connect(m_networkReply, SIGNAL(finished()), this, SLOT(onRequestAccessTokenReceived()));
    connect(m_networkReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onNetworkErrorRecieved(QNetworkReply::NetworkError)));

}


void OAuth::setAuthToken(const QByteArray &authToken)
{
    qDebug("[OAuth::setAuthToken]");
    qDebug() << "[OAuth::setAuthToken] auth token set is: " << authToken;

    m_authToken = authToken;
    Q_EMIT authTokenChanged();
}
