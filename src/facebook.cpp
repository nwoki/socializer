/*
 * facebook.cpp
 *
 * This file is part of the Socializer library
 * Copyright (C) 2012 Ispirata <info@ispirata.com>
 *
 * Author Francesco Nwokeka <francesco@ispirata.com>
 */

#include "facebook.h"

#include <QtCore/QDebug>
#include <QtCore/QRegExp>
#include <QtCore/QStringList>
#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeContext>

#include <qjson/parser.h>

#define AUTH_URL "https://m.facebook.com/dialog/oauth?"
#define GRAPH_URL "https://graph.facebook.com/me/"

using namespace Socializer;


Facebook::Facebook(const QByteArray &appId, const QByteArray &redirectUrl, QObject *parent)
    : OAuth(appId, redirectUrl, QByteArray(), parent)
    , m_scopeEmail(false)
    , m_scopePublishAcions(false)
    , m_scopePublishCheckins(false)
    , m_scopePublishStream(false)
    , m_scopeReadStream(false)
    , m_scopeUserInfo(false)
    , m_userInfo(new Me)
{
    connect(this, SIGNAL(authTokenChanged()), this, SLOT(onAuthTokenChanged()));
}


Facebook::Facebook(const QByteArray &authToken, QObject *parent)
    : OAuth(authToken, parent)
    , m_scopeEmail(false)
    , m_scopePublishAcions(false)
    , m_scopePublishCheckins(false)
    , m_scopePublishStream(false)
    , m_scopeReadStream(false)
    , m_scopeUserInfo(false)
    , m_userInfo(new Me)
{
    connect(this, SIGNAL(authTokenChanged()), this, SLOT(onAuthTokenChanged()));
    populateData();
}



Facebook::~Facebook()
{
    delete m_userInfo;
}


QString Facebook::createScope()
{
    QList<QString> scopeList;
    QString scopeLine("&scope=");
    bool isFirst = true;

    if (m_scopeEmail) {
        scopeList.append("email");
    }

    if (m_scopePublishAcions) {
        scopeList.append("publish_actions");
    }

    if (m_scopePublishCheckins) {
        scopeList.append("publish_checkins");
    }

    if (m_scopePublishStream) {
        scopeList.append("publish_stream");
    }

    if (m_scopeReadStream) {
        scopeList.append("read_stream");
    }

    if (m_scopeUserInfo) {
        scopeList.append("user_about_me");
        scopeList.append("user_birthday");
        scopeList.append("user_education_history");
        scopeList.append("user_hometown");
        scopeList.append("user_interests");
        scopeList.append("user_likes");
        scopeList.append("user_religion_politics");
        scopeList.append("user_website");
        scopeList.append("user_work_history");
        scopeList.append("user_relationships");
        scopeList.append("user_relationship_details");
    }

    foreach(QString scope, scopeList) {
        if (!isFirst) {
            scope.prepend(",");
        } else {
            isFirst = false;
        }

        scopeLine.append(scope);
    }

    if (!isFirst) {
        return scopeLine;
    } else {
        return QString();
    }
}


void Facebook::enableScopeEmail(bool enable)
{
    m_scopeEmail = enable;
}


void Facebook::enableScopePublishActions(bool enable)
{
    m_scopePublishAcions = enable;
}


void Facebook::enableScopePublishCheckins(bool enable)
{
    m_scopePublishCheckins = enable;
}


void Facebook::enableScopePublishStream(bool enable)
{
    m_scopePublishStream = enable;
}


void Facebook::enableScopeReadStream(bool enable)
{
    m_scopeReadStream = enable;
}


void Facebook::enableScopeUserInfo(bool enable)
{
    m_scopeUserInfo = enable;

    if (m_scopeUserInfo) {
        populateData();
    }
}


void Facebook::obtainAuthPageUrl()
{
    qDebug("[Facebook::obtainAuthPageUrl]");

    QString urlStr(AUTH_URL);

    urlStr.append("client_id=");
    urlStr.append(m_appId);
    urlStr.append("&redirect_uri=");
    urlStr.append(m_redirectUrl);
    urlStr.append(createScope());
    urlStr.append("&response_type=token");

    Q_EMIT authPageUrlReady(urlStr);
}


void Facebook::onNetReplyError(QNetworkReply::NetworkError error)
{
    qDebug("[Facebook::onNetReplyError]");

    // don't need to do check for int conversion here. I trust Qt's code ;)
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QString errStr = reply->errorString();

    qDebug() << "Error msg: " << errStr;
    qDebug() << "Status code: " << statusCode;


    reply->deleteLater();

    /// TODO implement cases
    Q_UNUSED(error)
    Q_UNUSED(errStr)
    Q_UNUSED(statusCode)

//     qDebug() << "Remaining replies in list: " << m_fbNetReplies.count();
}


void Facebook::onAuthTokenChanged()
{
    qDebug("[Facebook::onAuthTokenChanged]");

    qDebug() << "TOKEN: " << m_authToken;

    populateData();
}


void Facebook::onPopulateDataReplyReceived()
{
    qDebug("[Facebook::onPopulateDataReplyReceived]");

    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    QByteArray rcv = reply->readAll();

    qDebug() << "[Facebook::onPopulateDataReplyReceived] rcv: " << rcv;

    // parse incoming json
    QJson::Parser parser;
    bool ok;

    QVariantMap result = parser.parse(rcv, &ok).toMap();

    if (!ok) {
        qDebug("[Facebook::onPopulateDataReplyReceived] ERROR: invalid json");
        return;
    }

    // populate user data
    m_userInfo->brithday = result["birthday"].toString();
    m_userInfo->email = result["email"].toString();
    m_userInfo->firstName = result["first_name"].toString();
    m_userInfo->gender = result["gender"].toString();
    m_userInfo->id = result["id"].toString();
    m_userInfo->lastName = result["last_name"].toString();
    m_userInfo->link = result["link"].toString();
    m_userInfo->name = result["name"].toString();
    m_userInfo->relationshipStatus = result["relationship_status"].toString();
    m_userInfo->username = result["username"].toString();
    m_userInfo->verified = result["verified"].toString();
    m_userInfo->bio = result["bio"].toString();

    // status (for now, just the last one)
    QVariantMap statusesMap = result["statuses"].toMap();
    QList<QVariant> statusesList = statusesMap["data"].toList();

    if (statusesList.count() >= 1) {
        // set the last status
        QVariantMap status = statusesList.at(0).toMap();
        m_userInfo->status = status["message"].toString();
    }

    // extract picture
    QVariantMap pictureMap = result["picture"].toMap();
    QVariantMap pictureDataMap = pictureMap["data"].toMap();

    m_userInfo->picture = pictureDataMap["url"].toString();

//     qDebug() << "INFO: " << m_userInfo.firstName << " : " << m_userInfo.id << " : " << m_userInfo.email << " : " << m_userInfo.picture;
    // populate LIKES data
    QVariantMap likesMap = result["likes"].toMap();

    Q_FOREACH (QVariant likeData, likesMap["data"].toList()) {
        QVariantMap likeDataMap = likeData.toMap();
        Like *newLike = new Like;

        newLike->category = likeDataMap["category"].toString();
        newLike->createdTime = likeDataMap["created_time"].toString();
        newLike->id = likeDataMap["id"].toString();
        newLike->name = likeDataMap["name"].toString();

        // add to hash
        m_likes.insert(newLike->id, newLike);

//         qDebug() << "new like: " << newLike->id << ":" << newLike->name << ":" << newLike->category;
    }


    // populate FRIENDS data
    QVariantMap friendsMap = result["friends"].toMap();

    Q_FOREACH (QVariant friendData, friendsMap["data"].toList()) {
        QVariantMap friendDataMap = friendData.toMap();
        Friend *newFriend = new Friend;

        newFriend->id = friendDataMap["id"].toString();
        newFriend->firstName = friendDataMap["first_name"].toString();
        newFriend->lastName = friendDataMap["last_name"].toString();
        newFriend->name = friendDataMap["name"].toString();
        newFriend->gender = friendDataMap["gender"].toString();

        QVariantMap friendPictureMap = friendDataMap["picture"].toMap();
        QVariantMap friendPictureDataMap = friendPictureMap["data"].toMap();

        newFriend->picture = friendPictureDataMap["url"].toString();

        // add to hash
        m_friends.insert(newFriend->id, newFriend);

//         qDebug() << "new friend: " << newFriend->id << ":" << newFriend->name << ":" << newFriend->picture;
    }

    reply->deleteLater();

    // tell that the profile data has been updated
    Q_EMIT profileUpdated();
}


void Facebook::parseNewUrl(const QString& url)
{
    qDebug() << "[Facebook::parseNewUrl] got url: " << url;

    if (url.contains("access_token")) {
        QRegExp regex("access_token=?[^&]+");

        if (regex.indexIn(url) > -1) {
            QString access = regex.cap(0);

            // extract access token
            setAuthToken(access.split("=").at(1).toUtf8());

            qDebug() << "[Facebook::parseNewUrl] Auth token is: " << m_authToken;
        }
    }
}


void Facebook::populateData()
{
    qDebug("[Facebook::populateData]");

    if (m_authToken.isEmpty()) {
        qDebug("[Facebook::populateData] no access token has been set!");
        return;
    }

    if (m_scopeUserInfo) {

        qDebug("[Facebook::populateData] user info scope");

        QNetworkRequest req;
        QNetworkReply *netRep;

        // request all possible data. The access token will return only the data that the user has agreed to share with us
        // statuses.limit(1) -> keep last status the user posted
        QString reqStr(GRAPH_URL);

        reqStr += "?fields=id,name,first_name,last_name,email,birthday,address,gender,hometown,link,political,relationship_status,religion,sports,username,verified,work,likes,website,statuses.limit(1),picture.type(large)";
        reqStr += ",friends.fields(id,name,first_name,last_name,gender,picture.type(large))";                  // friends
//                 ",games.fields(id,link,website,name,picture.type(large))";                           // games
//                 ",music.fields(id,bio,description,hometown,link,name,picture.type(large),website)";  // music
        reqStr += "&access_token=" + m_authToken;

        qDebug() << "[Facebook::populateData] requesting: " << reqStr;

        req.setUrl(QUrl(reqStr));
        netRep = m_networkAccessManager->get(req);

        // connect
        connect(netRep, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onNetReplyError(QNetworkReply::NetworkError)));
        connect(netRep, SIGNAL(finished()), this, SLOT(onPopulateDataReplyReceived()));
    }
}


bool Facebook::scopeEmail() const
{
    return m_scopeEmail;
}


void Facebook::setContextProperty(QDeclarativeView *view)
{
//    view->rootContext()->setContextProperty("Facebook", this);
}


bool Facebook::scopePublishActions() const
{
    return m_scopePublishAcions;
}


bool Facebook::scopePublishCheckins() const
{
    return m_scopePublishCheckins;
}


bool Facebook::scopePublishStream() const
{
    return m_scopePublishStream;
}


bool Facebook::scopeReadStream() const
{
    return m_scopeReadStream;
}


bool Facebook::scopeUserInfo() const
{
    return m_scopeUserInfo;
}


Facebook::Me *Facebook::userInfo() const
{
    qDebug("[Facebook::userInfo]");

    return m_userInfo;
}


QList<Facebook::Friend*> Facebook::friends() const
{
    qDebug("[Facebook::friends]");

    QHash<QString, Friend*>::const_iterator it = m_friends.constBegin();
    QList<Friend*> friends;

    while (it != m_friends.constEnd()) {
        friends.append(it.value());
        qDebug() << "FRIEND is: " << it.value()->name;
        it++;
    }

    return friends;
}


QList<Facebook::Like*> Facebook::userLikes() const
{
    qDebug("[Facebook::userLikes]");

    QHash<QString, Like*>::const_iterator it = m_likes.constBegin();
    QList<Like*> likes;

    while (it != m_likes.constEnd()) {
        likes.append(it.value());
        qDebug() << "LIKS is: " << it.value()->name;
        it++;
    }

    return likes;
}



