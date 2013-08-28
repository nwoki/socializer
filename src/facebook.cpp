/*
 * facebook.cpp
 *
 * This file is part of the Socializer library
 * Copyright (C) 2012 Ispirata <info@ispirata.com>
 *
 * Author Francesco Nwokeka <francesco@ispirata.com>
 */

#include "facebook.h"
#include "facebookuser.h"

#include <QtCore/QDebug>

#ifdef USING_QT5
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#else
#include <qjson/parser.h>
#endif

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


Facebook::Facebook(const QByteArray &authToken, QObject *parent)
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

    populateData();
}



Facebook::~Facebook()
{
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

    reply->deleteLater();

#ifdef USING_QT5
    // extract json object from data received
    QJsonObject jsonObj = jsonObject(rcv);

    if (jsonObj.isEmpty()) {
        // error occured
        return;
    }
#else
    QJson::Parser parser;
    bool ok;

    QVariantMap jsonObj = parser.parse(rcv, &ok).toMap();

    if (jsonObj.isEmpty()) {
        return;
    }
#endif

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

    qDebug() << m_fbUser->id() << m_fbUser->email() << m_fbUser->birthday().toString() << m_fbUser->birthday() << m_fbUser->locale();


    // hometown
#ifdef USING_QT5
    QJsonObject hometownObj = jsonObj.value("hometown").toObject();
#else
    QVariantMap hometownObj = jsonObj.value("hometown").toMap();
#endif

    if (!hometownObj.isEmpty()) {
        m_fbUser->hometown().first = hometownObj.value("id").toString();
        m_fbUser->hometown().second = hometownObj.value("name").toString();
        qDebug() << "[Facebook::onPopulateDataReplyReceived] HOME TOWN: " << m_fbUser->hometown().first << m_fbUser->hometown().second;
    }

    // location
#ifdef USING_QT5
    QJsonObject locationObj = jsonObj.value("location").toObject();
#else
    QVariantMap locationObj = jsonObj.value("location").toMap();
#endif

    if (!locationObj.isEmpty()) {
        m_fbUser->location().first = locationObj.value("id").toString();
        m_fbUser->location().second = locationObj.value("name").toString();
        qDebug() << "[Facebook::onPopulateDataReplyReceived] LOCATION: " << m_fbUser->location().first << m_fbUser->location().second;
    }

    // status (for now, just the last one)
#ifdef USING_QT5
    QJsonObject  statusesObj = jsonObj.value("statuses").toObject();

    if (!statusesObj.isEmpty()) {
        QJsonArray statusArray = statusesObj.value("data").toArray();
        m_fbUser->setStatus(statusArray[0].toObject().value("message").toString());
    }
#else
    QVariantMap  statusesObj = jsonObj.value("statuses").toMap();

    if (!statusesObj.isEmpty()) {
        QList<QVariant> statusArray = statusesObj.value("data").toList();
        m_fbUser->setStatus(statusArray[0].toMap().value("message").toString());
    }
#endif

    qDebug() << "[Facebook::onPopulateDataReplyReceived] user status: " << m_fbUser->status();

    // user avatar
#ifdef USING_QT5
    QJsonObject pictureObj = jsonObj.value("picture").toObject();

    if (!pictureObj.isEmpty()) {
        QJsonObject pictureDataObj = pictureObj.value("data").toObject();
        m_fbUser->setPicture(pictureDataObj.value("url").toString());
    }
#else
    QVariantMap pictureObj = jsonObj.value("picture").toMap();

    if (!pictureObj.isEmpty()) {
        QVariantMap pictureDataObj = pictureObj.value("data").toMap();
        m_fbUser->setPicture(pictureDataObj.value("url").toString());
    }
#endif

    qDebug() << "[Facebook::onPopulateDataReplyReceived] Picture: " << m_fbUser->picture();


    // LIKES
#ifdef USING_QT5
    QJsonObject likesObj = jsonObj.value("likes").toObject();
    if (!likesObj.isEmpty()) {
        QJsonArray likesArray = likesObj.value("data").toArray();

        for (int i = 0; i < likesArray.size(); ++i) {
            QJsonObject likeDetail = likesArray.at(i).toObject();
#else
    QVariantMap likesObj = jsonObj.value("likes").toMap();
    if (!likesObj.isEmpty()) {
        QList<QVariant> likesArray = likesObj.value("data").toList();

        for (int i = 0; i < likesArray.size(); ++i) {
            QVariantMap likeDetail = likesArray.at(i).toMap();
#endif

            FacebookUser::Like like;

            like.category = likeDetail.value("category").toString();
            like.createdTime = likeDetail.value("created_time").toString();
            like.id = likeDetail.value("id").toString();
            like.name = likeDetail.value("name").toString();

            // add to hash
            m_fbUser->addLike(like.id, like);
            qDebug() << "[Facebook::onPopulateDataReplyReceived] NEW LIKE: " << like.id << " - " << like.category << " - " << like.name;
        }
    }

    // FRIENDS
#ifdef USING_QT5
    QJsonObject friendsObj = jsonObj.value("friends").toObject();
    if (!friendsObj.isEmpty()) {
        QJsonArray friendsArray = friendsObj.value("data").toArray();

        for (int i = 0; i < friendsArray.size(); ++i) {
            QJsonObject friendDetail = friendsArray.at(i).toObject();
#else
    QVariantMap friendsObj = jsonObj.value("friends").toMap();
    if (!friendsObj.isEmpty()) {
        QList<QVariant> friendsArray = friendsObj.value("data").toList();

        for (int i = 0; i < friendsArray.size(); ++i) {
            QVariantMap friendDetail = friendsArray.at(i).toMap();
#endif

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
#ifdef USING_QT5
            QJsonObject friendPictureObj = friendDetail.value("picture").toObject();
            if (!friendPictureObj.isEmpty()) {
                QJsonObject pictureDataObj = friendPictureObj.value("data").toObject();
                myFriend.picture = friendPictureObj.value("url").toString();
            }
#else
            QVariantMap friendPictureObj = friendDetail.value("picture").toMap();
            if (!friendPictureObj.isEmpty()) {
                QVariantMap pictureDataObj = friendPictureObj.value("data").toMap();
                myFriend.picture = friendPictureObj.value("url").toString();
            }
#endif

            m_fbUser->addFriend(myFriend.id, myFriend);
            qDebug() << "[Facebook::onPopulateDataReplyReceived] new friend: " << myFriend.picture << " - " << myFriend.firstName << " - " << myFriend.lastName << " - " << myFriend.id << myFriend.link;
        }
    }


    // WORK
#ifdef USING_QT5
    QJsonArray workArray = jsonObj.value("work").toArray();
#else
    QList<QVariant> workArray = jsonObj.value("work").toList();
#endif

    for (int i = 0; i < workArray.size(); ++i) {
        FacebookUser::Work work;
#ifdef USING_QT5
        QJsonObject workObj = workArray.at(i).toObject();
#else
        QVariantMap workObj = workArray.at(i).toMap();
#endif

        work.description = workObj.value("description").toString();
        work.endDate = QDate::fromString(workObj.value("end_date").toString(), "yyyy-MM-dd");
        work.startDate = QDate::fromString(workObj.value("start_date").toString(), "yyyy-MM-dd");

        // employer
#ifdef USING_QT5
        QJsonObject workEmployer = workObj.value("employer").toObject();
#else
        QVariantMap workEmployer = workObj.value("employer").toMap();
#endif
        if (!workEmployer.isEmpty()) {
            work.employer.first = workEmployer.value("id").toString();
            work.employer.second = workEmployer.value("name").toString();
        }

        // location
#ifdef USING_QT5
        QJsonObject workLocation = workObj.value("location").toObject();
#else
        QVariantMap workLocation = workObj.value("location").toMap();
#endif
        if (!workLocation.isEmpty()) {
            work.location.first = workLocation.value("id").toString();
            work.location.second = workLocation.value("name").toString();
        }

        // position
#ifdef USING_QT5
        QJsonObject workPosition = workObj.value("position").toObject();
#else
        QVariantMap workPosition = workObj.value("position").toMap();
#endif
        if (!workPosition.isEmpty()) {
            work.position.first = workPosition.value("id").toString();
            work.position.second = workPosition.value("name").toString();
        }

        m_fbUser->addWork(work);
        qDebug() << "[Facebook::onPopulateDataReplyReceived] WORK - " << work.employer.second << " - " << work.description << " - " << work.startDate.toString();
    }


    // education
#ifdef USING_QT5
    QJsonArray educationArray = jsonObj.value("education").toArray();
#else
    QList<QVariant> educationArray = jsonObj.value("education").toList();
#endif

    for (int i = 0; i < educationArray.size(); ++i) {
        FacebookUser::Education education;

#ifdef USING_QT5
        QJsonObject educationObj = educationArray.at(i).toObject();
#else
        QVariantMap educationObj = educationArray.at(i).toMap();
#endif
        education.type = educationObj.value("type").toString();

#ifdef USING_QT5
        QJsonObject educationSchool = educationObj.value("school").toObject();
#else
        QVariantMap educationSchool = educationObj.value("school").toMap();
#endif
        education.school.first = educationSchool.value("id").toString();;
        education.school.second = educationSchool.value("name").toString();

        m_fbUser->addEducation(education);
        qDebug() << "[Facebook::onPopulateDataReplyReceived] EDUCATION: " << education.type << " - " << education.school.first << " - " << education.school.second;
    }

    // notify that the profile has been updated
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

    reqStr += "?fields=id,name,first_name,last_name,email,birthday,address,gender,hometown,link,locale,political,relationship_status,religion,education,sports,username,verified,work,likes,website,statuses.limit(1),picture.type(large)";
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
