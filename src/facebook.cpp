/*
 * facebook.cpp
 *
 * This file is part of the Socializer library
 *
 * Author Francesco Nwokeka <francesco.nwokeka@gmail.com>
 */

#include "facebook.h"
#include "facebookuser.h"

#include <QtCore/QDebug>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QRegExp>
#include <QtCore/QStringList>

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
    , m_fbUser(new FacebookUser(this))
{
    connect(this, SIGNAL(authTokenChanged()), this, SLOT(onAuthTokenChanged()));
}


Facebook::Facebook(const QByteArray &authToken, bool updateOnCreate, QObject *parent)
    : OAuth(authToken, parent)
    , m_scopeEmail(false)
    , m_scopePublishAcions(false)
    , m_scopePublishCheckins(false)
    , m_scopePublishStream(false)
    , m_scopeReadStream(false)
    , m_scopeUserInfo(false)
    , m_fbUser(new FacebookUser(this))
{
    connect(this, SIGNAL(authTokenChanged()), this, SLOT(onAuthTokenChanged()));

    if (updateOnCreate) {
        populateData();
    }
}



Facebook::~Facebook()
{
}


void Facebook::checkLastUpdateTime() const
{
    if (m_authToken.isEmpty()) {
        qWarning("[Facebook::checkLastUpdateTime] auth token empty");
        return;
    }

    QNetworkRequest req;
    QNetworkReply *netRep;
    QString reqStr(GRAPH_URL);

    reqStr += "?fields=updated_time&access_token=" + m_authToken;

    req.setUrl(QUrl(reqStr));
    netRep = m_networkAccessManager->get(req);

    // connect
    connect(netRep, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onNetReplyError(QNetworkReply::NetworkError)));
    connect(netRep, SIGNAL(finished()), this, SLOT(onLastUpdatedTimeReceived()));
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

    foreach(const QString &scope, scopeList) {
        if (!isFirst) {
            scopeLine.append(',');
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
    QByteArray rcv = reply->readAll();

    reply->deleteLater();

    QJsonObject jsonObj = jsonObject(rcv);

    if (jsonObj.isEmpty()) {
        qWarning("[Facebook::profileInfoReceived] error parsing json");
        return;
    }

    if (jsonObj.value("error").toObject().value("code").toVariant().toInt() == 190) {
        int errorSubCode = jsonObj.value("error").toObject().value("error_subcode").toVariant().toInt();

        switch (errorSubCode) {
            case 460:
                Q_EMIT authTokenError(PasswordChanged);
                break;
            case 463:
                Q_EMIT authTokenError(Expired);
                break;
            case 467:
                Q_EMIT authTokenError(Invalid);
                break;
            default:
                Q_EMIT authTokenError(Unknown);
        };
    }

    Q_UNUSED(error)
    Q_UNUSED(errStr)
    Q_UNUSED(statusCode)
}


void Facebook::onAuthTokenChanged()
{
    qDebug("[Facebook::onAuthTokenChanged]");

    qDebug() << "TOKEN: " << m_authToken;

    populateData();
}


void Facebook::onLikeDataReceived()
{
    qDebug("[Facebook::onLikeDataReceived]");

    QNetworkReply *rep = qobject_cast<QNetworkReply*>(sender());
    QByteArray data;

    if (rep == 0) {
        return;
    }

    while  (rep->bytesAvailable()) {
        data += rep->readAll();
    }

    rep->deleteLater();

    parseLikeData(data);
}


void Facebook::onLastUpdatedTimeReceived()
{
    qDebug("[Facebook::onLastUpdatedTimeReceived]");

    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    if (reply->error() != QNetworkReply::NoError) {
        return;
    }

    QByteArray rcv = reply->readAll();
    reply->deleteLater();

    QJsonObject jsonObj = jsonObject(rcv);

    // check if new time differs from old. If so, update the profile data
    if (jsonObj.value("updated_time").toString() != m_fbUser->lastUpdatedTime()) {
        populateData();
    }
}


void Facebook::onPopulateDataReplyReceived()
{
    qDebug("[Facebook::onPopulateDataReplyReceived]");

    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    if (reply->error() != QNetworkReply::NoError) {
        // don't want to handle errors here
        return;
    }

    QByteArray rcv = reply->readAll();

    reply->deleteLater();

    // extract json object from data received
    QJsonObject jsonObj = jsonObject(rcv);

    if (jsonObj.isEmpty()) {
        // error occured
        return;
    }

    // populate user data
    m_fbUser->setBirthday(QDate::fromString(jsonObj.value("birthday").toString(), "MM/dd/yyyy"));
    m_fbUser->setEmail(jsonObj.value("email").toString());
    m_fbUser->setFirstName(jsonObj.value("first_name").toString());
    m_fbUser->setGender(jsonObj.value("gender").toString());
    m_fbUser->setId(jsonObj.value("id").toString());
    m_fbUser->setLastName(jsonObj.value("last_name").toString());
    m_fbUser->setLink(jsonObj.value("link").toString());
    m_fbUser->setName(jsonObj.value("name").toString());
    m_fbUser->setRelationshipStatus(jsonObj.value("relationship_status").toString());
    m_fbUser->setUsername(jsonObj.value("username").toString());
    m_fbUser->setVerified(jsonObj.value("verified").toString());
    m_fbUser->setBio(jsonObj.value("bio").toString());
    m_fbUser->setLocale(jsonObj.value("locale").toString());
    m_fbUser->setLastUpdatedTime(jsonObj.value("updated_time").toString());

    qDebug() << m_fbUser->id() << m_fbUser->email() << m_fbUser->birthday().toString() << m_fbUser->birthday() << m_fbUser->locale() << m_fbUser->lastUpdatedTime();


    // hometown
    QJsonObject hometownObj = jsonObj.value("hometown").toObject();

    if (!hometownObj.isEmpty()) {
        m_fbUser->hometown().first = hometownObj.value("id").toString();
        m_fbUser->hometown().second = hometownObj.value("name").toString();
        qDebug() << "[Facebook::onPopulateDataReplyReceived] HOME TOWN: " << m_fbUser->hometown().first << m_fbUser->hometown().second;
    }

    // location
    QJsonObject locationObj = jsonObj.value("location").toObject();

    if (!locationObj.isEmpty()) {
        m_fbUser->location().first = locationObj.value("id").toString();
        m_fbUser->location().second = locationObj.value("name").toString();
        qDebug() << "[Facebook::onPopulateDataReplyReceived] LOCATION: " << m_fbUser->location().first << m_fbUser->location().second;
    }

    // status (for now, just the last one)
    QJsonObject  statusesObj = jsonObj.value("statuses").toObject();

    if (!statusesObj.isEmpty()) {
        QJsonArray statusArray = statusesObj.value("data").toArray();
        m_fbUser->setStatus(statusArray[0].toObject().value("message").toString());
    }

    qDebug() << "[Facebook::onPopulateDataReplyReceived] user status: " << m_fbUser->status();

    // user avatar
    QJsonObject pictureObj = jsonObj.value("picture").toObject();

    if (!pictureObj.isEmpty()) {
        QJsonObject pictureDataObj = pictureObj.value("data").toObject();
        m_fbUser->setPicture(pictureDataObj.value("url").toString());
    }

    qDebug() << "[Facebook::onPopulateDataReplyReceived] Picture: " << m_fbUser->picture();

    // LIKES
    parseLikeData(jsonObj);

    // FRIENDS
    QJsonObject friendsObj = jsonObj.value("friends").toObject();
    if (!friendsObj.isEmpty()) {
        QJsonArray friendsArray = friendsObj.value("data").toArray();

        for (int i = 0; i < friendsArray.size(); ++i) {
            QJsonObject friendDetail = friendsArray.at(i).toObject();
            FacebookUser::Friend myFriend;

            myFriend.about = friendDetail.value("about").toString();
            myFriend.birthday = friendDetail.value("birthday").toString();
            myFriend.firstName = friendDetail.value("first_name").toString();
            myFriend.gender = friendDetail.value("gender").toString();
            myFriend.id = friendDetail.value("id").toString();
            myFriend.lastName = friendDetail.value("last_name").toString();
            myFriend.link = friendDetail.value("link").toString();
            myFriend.locale = friendDetail.value("locale").toString();
            myFriend.name = friendDetail.value("name").toString();
            myFriend.relationshipStatus = friendDetail.value("relationship_status").toString();
            myFriend.username = friendDetail.value("username").toString();

            // picture
            QJsonObject friendPictureObj = friendDetail.value("picture").toObject();
            if (!friendPictureObj.isEmpty()) {
                QJsonObject pictureDataObj = friendPictureObj.value("data").toObject();
                myFriend.picture = friendPictureObj.value("url").toString();
            }

            m_fbUser->addFriend(myFriend.id, myFriend);
            qDebug() << "[Facebook::onPopulateDataReplyReceived] new friend id: " << myFriend.id;
        }
    }


    // WORK
    QJsonArray workArray = jsonObj.value("work").toArray();

    for (int i = 0; i < workArray.size(); ++i) {
        FacebookUser::Work work;
        QJsonObject workObj = workArray.at(i).toObject();

        work.description = workObj.value("description").toString();
        work.endDate = QDate::fromString(workObj.value("end_date").toString(), "yyyy-MM-dd");
        work.startDate = QDate::fromString(workObj.value("start_date").toString(), "yyyy-MM-dd");

        // employer
        QJsonObject workEmployer = workObj.value("employer").toObject();

        if (!workEmployer.isEmpty()) {
            work.employer.first = workEmployer.value("id").toString();
            work.employer.second = workEmployer.value("name").toString();
        }

        // location
        QJsonObject workLocation = workObj.value("location").toObject();

        if (!workLocation.isEmpty()) {
            work.location.first = workLocation.value("id").toString();
            work.location.second = workLocation.value("name").toString();
        }

        // position
        QJsonObject workPosition = workObj.value("position").toObject();

        if (!workPosition.isEmpty()) {
            work.position.first = workPosition.value("id").toString();
            work.position.second = workPosition.value("name").toString();
        }

        m_fbUser->addWork(work);
        qDebug() << "[Facebook::onPopulateDataReplyReceived] WORK - " << work.employer.second << " - " << work.description << " - " << work.startDate.toString();
    }


    // education
    QJsonArray educationArray = jsonObj.value("education").toArray();

    for (int i = 0; i < educationArray.size(); ++i) {
        FacebookUser::Education education;


        QJsonObject educationObj = educationArray.at(i).toObject();
        education.type = educationObj.value("type").toString();


        QJsonObject educationSchool = educationObj.value("school").toObject();
        education.school.first = educationSchool.value("id").toString();;
        education.school.second = educationSchool.value("name").toString();

        m_fbUser->addEducation(education);
        qDebug() << "[Facebook::onPopulateDataReplyReceived] EDUCATION: " << education.type << " - " << education.school.first << " - " << education.school.second;
    }

    // notify that the profile has been updated
    Q_EMIT profileUpdated();
}


void Facebook::parseLikeData(const QByteArray &data)
{
    qDebug("[Facebook::parseLikeData]");

    // extract json object from data received
    QJsonObject jsonObj = jsonObject(data);

    if (jsonObj.isEmpty()) {
        // error occured
        return;
    }

    parseLikeData(jsonObj);
}


void Facebook::parseLikeData(const QJsonObject &jsonObj)
{
    qDebug("[Facebook::parseLikeData]");

    QJsonObject likesObj = jsonObj.value("likes").toObject();
    QJsonArray likesArray;

    likesObj.isEmpty() ? likesArray = jsonObj.value("data").toArray() : likesArray = likesObj.value("data").toArray();


    for (int i = 0; i < likesArray.size(); ++i) {
        QJsonObject likeDetail = likesArray.at(i).toObject();
        FacebookUser::Like like;

        like.description = likeDetail.value("description").toString();
        like.createdTime = likeDetail.value("created_time").toString();
        like.id = likeDetail.value("id").toString();
        like.name = likeDetail.value("name").toString();
        like.link = likeDetail.value("link").toString();
        like.category = likeDetail.value("category").toString();

        // add to hash
        m_fbUser->addLike(like.id, like);
        qDebug() << "[Facebook::parseLikeData] new like id: " << like.id;
    }

    // now check for next page
    QJsonObject likesPaging;
    likesObj.isEmpty() ? likesPaging = jsonObj.value("paging").toObject() : likesPaging = likesObj.value("paging").toObject();

    if (!likesPaging.isEmpty() && !likesPaging.value("next").toString().isEmpty()) {
        QNetworkRequest req;
        req.setUrl(QUrl(likesPaging.value("next").toString()));

        QNetworkReply *netRep = m_networkAccessManager->get(req);

        // connect
        connect(netRep, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onNetReplyError(QNetworkReply::NetworkError)));
        connect(netRep, SIGNAL(finished()), this, SLOT(onLikeDataReceived()));
    }
}


void Facebook::parseNewUrl(const QString &url)
{
    qDebug() << "[Facebook::parseNewUrl] got url: " << url;

    if (url.contains("access_token")) {
        QRegExp regex("access_token=?[^&]+");

        if (regex.indexIn(url) > -1) {
            QString access = regex.cap(0);

            // extract access token
            setAuthToken(access.split('=').at(1).toUtf8());

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

    qDebug("[Facebook::populateData] pulling user info");

    QNetworkRequest req;
    QNetworkReply *netRep;

    // request all possible data. The access token will return only the data that the user has agreed to share with us
    // statuses.limit(1) -> keep last status the user posted
    QString reqStr(GRAPH_URL);

    reqStr += "?fields=id,name,first_name,last_name,email,birthday,address,gender,hometown,link,locale,political,relationship_status,religion,education,sports,username,verified,work,website,statuses.limit(1),picture.type(large),updated_time";
    reqStr += ",likes.fields(id,description,name,created_time,link,category)";
    reqStr += ",friends.fields(id,name,username,first_name,birthday,last_name,locale,gender,link,picture.type(large))";                  // friends
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


FacebookUser *Facebook::facebookUser() const
{
    return m_fbUser;
}


void Facebook::update()
{
    qDebug("[Facebook::update]");

    // get new "lastUpdatedTime" from the fb server. We don't know if the user has modified
    // any info
    m_fbUser->lastUpdatedTime().isEmpty() ? populateData() : checkLastUpdateTime();
}

