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

#ifdef USING_QT5
#include <QtCore/QJsonArray>
#else

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

    qDeleteAll(m_friends);
    qDeleteAll(m_likes);
    qDeleteAll(m_work);
    qDeleteAll(m_education);

    m_friends.clear();
    m_likes.clear();
    m_work.clear();
    m_education.clear();
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

#ifdef USING_QT5
void Facebook::onPopulateDataReplyReceived()
{
    qDebug("[Facebook::onPopulateDataReplyReceived]");

    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    QByteArray rcv = reply->readAll();

    reply->deleteLater();

    // extract json object from data received
    QJsonObject jsonObj = jsonObject(rcv);

    if (jsonObj.isEmpty()) {
        // error occured
        return;
    }

    // populate user data
    m_userInfo->birthday = QDate::fromString(jsonObj.value("birthday").toString(), "MM/dd/yyyy");
    m_userInfo->email = jsonObj.value("email").toString();
    m_userInfo->firstName = jsonObj.value("first_name").toString();
    m_userInfo->gender = jsonObj.value("gender").toString();
    m_userInfo->id = jsonObj.value("id").toString();
    m_userInfo->lastName = jsonObj.value("last_name").toString();
    m_userInfo->link = jsonObj.value("link").toString();
    m_userInfo->name = jsonObj.value("name").toString();
    m_userInfo->relationshipStatus = jsonObj.value("relationship_status").toString();
    m_userInfo->username = jsonObj.value("username").toString();
    m_userInfo->verified = jsonObj.value("verified").toString();
    m_userInfo->bio = jsonObj.value("bio").toString();
    m_userInfo->locale = jsonObj.value("locale").toString();

//     qDebug() << m_userInfo->id << m_userInfo->email << m_userInfo->birthday.toString() << m_userInfo->birthday << m_userInfo->locale;


    // hometown
    qDebug() << jsonObj.value("hometown");
    QJsonObject hometownObj = jsonObj.value("hometown").toObject();
    if (!hometownObj.isEmpty()) {
        m_userInfo->hometown.id = hometownObj.value("id").toString();
        m_userInfo->hometown.name = hometownObj.value("name").toString();
        qDebug() << "HOME TOWN: " << m_userInfo->hometown.name << m_userInfo->hometown.id;
    }

    // location
    qDebug() << jsonObj.value("location");
    QJsonObject locationObj = jsonObj.value("location").toObject();
    if (!locationObj.isEmpty()) {
        m_userInfo->location.id = locationObj.value("id").toString();
        m_userInfo->location.name = locationObj.value("name").toString();
        qDebug() << "LOCATION: " << m_userInfo->location.name << m_userInfo->location.id;
    }

    // status (for now, just the last one)
    QJsonObject  statusesObj = jsonObj.value("statuses").toObject();
    if (!statusesObj.isEmpty()) {
        QJsonArray statusArray = statusesObj.value("data").toArray();
        m_userInfo->status = statusArray[0].toObject().value("message").toString();
    }

    // user avatar
    QJsonObject pictureObj = jsonObj.value("picture").toObject();
    if (!pictureObj.isEmpty()) {
        QJsonObject pictureDataObj = pictureObj.value("data").toObject();
        m_userInfo->picture = pictureDataObj.value("url").toString();
    }


    // user likes
    QJsonObject likesObj = jsonObj.value("likes").toObject();
    if (!likesObj.isEmpty()) {
        QJsonArray likesArray = likesObj.value("data").toArray();

        for (int i = 0; i < likesArray.size(); ++i) {
            QJsonObject likeDetail = likesArray.at(i).toObject();

            Like *like = new Like;

            like->category = likeDetail.value("category").toString();
            like->createdTime = likeDetail.value("created_time").toString();
            like->id = likeDetail.value("id").toString();
            like->name = likeDetail.value("name").toString();

            // add to hash
            m_likes.insert(like->id, like);
            qDebug() << "NEW LIKE: " << like->id << " - " << like->category << " - " << like->name;
        }
    }


    // friends
    QJsonObject friendsObj = jsonObj.value("friends").toObject();
    if (!friendsObj.isEmpty()) {
        QJsonArray friendsArray = friendsObj.value("data").toArray();

        for (int i = 0; i < friendsArray.size(); ++i) {
            QJsonObject friendDetail = friendsArray.at(i).toObject();

            Friend *myFriend = new Friend;

            myFriend->about = friendDetail.value("about").toString();
            myFriend->birthday = friendDetail.value("birthday").toString();
            myFriend->firstName = friendDetail.value("first_name").toString();
            myFriend->gender = friendDetail.value("gender").toString();
            myFriend->id = friendDetail.value("id").toString();
            myFriend->lastName = friendDetail.value("last_name").toString();
            myFriend->link = friendDetail.value("").toString();
            myFriend->locale = friendDetail.value("locale").toString();
            myFriend->name = friendDetail.value("name").toString();
            myFriend->picture = friendDetail.value("").toString();
            myFriend->relationshipStatus = friendDetail.value("relationship_status").toString();
            myFriend->username = friendDetail.value("username").toString();

            // picture
            QJsonObject friendPictureObj = friendDetail.value("picture").toObject();
            if (!friendPictureObj.isEmpty()) {
                QJsonObject pictureDataObj = friendPictureObj.value("data").toObject();
                myFriend->picture = friendPictureObj.value("url").toString();
            }

            // TODO education/work

            m_friends.insert(myFriend->id, myFriend);
            qDebug() << "new friend: " << myFriend->picture << " - " << myFriend->firstName << " - " << myFriend->lastName << " - " << myFriend->id;
        }
    }


    // work
    QJsonArray workArray = jsonObj.value("work").toArray();

    for (int i = 0; i < workArray.size(); ++i) {
        Work *work = new Work;
        QJsonObject workObj = workArray.at(i).toObject();

        work->description = workObj.value("description").toString();
        work->endDate = QDate::fromString(workObj.value("end_date").toString(), "yyyy-MM-dd");
        work->startDate = QDate::fromString(workObj.value("start_date").toString(), "yyyy-MM-dd");

        // employer
        QJsonObject workEmployer = workObj.value("employer").toObject();
        if (!workEmployer.isEmpty()) {
            work->employer.id = workEmployer.value("id").toString();
            work->employer.name = workEmployer.value("name").toString();
        }

        // location
        QJsonObject workLocation = workObj.value("location").toObject();
        if (!workLocation.isEmpty()) {
            work->location.id = workLocation.value("id").toString();
            work->location.name = workLocation.value("name").toString();
        }

        // position
        QJsonObject workPosition = workObj.value("position").toObject();
        if (!workPosition.isEmpty()) {
            work->position.id = workPosition.value("id").toString();
            work->position.name = workPosition.value("name").toString();
        }

        m_work.append(work);
        qDebug() << "WORK - " << work->employer.name << " - " << work->description << " - " << work->startDate.toString();
    }


    // education
    QJsonArray educationArray = jsonObj.value("education").toArray();

    for (int i = 0; i < educationArray.size(); ++i) {
        Education *education = new Education;

        QJsonObject educationObj = educationArray.at(i).toObject();
        education->type = educationObj.value("type").toString();

        QJsonObject educationSchool = educationObj.value("school").toObject();
        education->school.id = educationSchool.value("id").toString();;
        education->school.name = educationSchool.value("name").toString();

        m_education.append(education);
        qDebug() << "EDUCATION: " << education->type << " - " << education->school.id << " - " << education->school.name;
    }
}
#else
void Facebook::onPopulateDataReplyReceivedQt4()
{
//     qDebug("[Facebook::onPopulateDataReplyReceived]");
//     
//     QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
//     QByteArray rcv = reply->readAll();
//     
//     reply->deleteLater();
//     
//     // extract json object from data received
//     QJsonObject jsonObj = jsonObject(rcv);
//     
//     if (jsonObj.isEmpty()) {
//         // error occured
//         return;
//     }
//     
//     // populate user data
//     m_userInfo->birthday = QDate::fromString(jsonObj.value("birthday").toString(), "MM/dd/yyyy");
//     m_userInfo->email = jsonObj.value("email").toString();
//     m_userInfo->firstName = jsonObj.value("first_name").toString();
//     m_userInfo->gender = jsonObj.value("gender").toString();
//     m_userInfo->id = jsonObj.value("id").toString();
//     m_userInfo->lastName = jsonObj.value("last_name").toString();
//     m_userInfo->link = jsonObj.value("link").toString();
//     m_userInfo->name = jsonObj.value("name").toString();
//     m_userInfo->relationshipStatus = jsonObj.value("relationship_status").toString();
//     m_userInfo->username = jsonObj.value("username").toString();
//     m_userInfo->verified = jsonObj.value("verified").toString();
//     m_userInfo->bio = jsonObj.value("bio").toString();
//     m_userInfo->locale = jsonObj.value("locale").toString();
//     
//     //     qDebug() << m_userInfo->id << m_userInfo->email << m_userInfo->birthday.toString() << m_userInfo->birthday << m_userInfo->locale;
//     
//     
//     // hometown
//     qDebug() << jsonObj.value("hometown");
//     QJsonObject hometownObj = jsonObj.value("hometown").toObject();
//     if (!hometownObj.isEmpty()) {
//         m_userInfo->hometown.id = hometownObj.value("id").toString();
//         m_userInfo->hometown.name = hometownObj.value("name").toString();
//         qDebug() << "HOME TOWN: " << m_userInfo->hometown.name << m_userInfo->hometown.id;
//     }
//     
//     // location
//     qDebug() << jsonObj.value("location");
//     QJsonObject locationObj = jsonObj.value("location").toObject();
//     if (!locationObj.isEmpty()) {
//         m_userInfo->location.id = locationObj.value("id").toString();
//         m_userInfo->location.name = locationObj.value("name").toString();
//         qDebug() << "LOCATION: " << m_userInfo->location.name << m_userInfo->location.id;
//     }
//     
//     // status (for now, just the last one)
//     QJsonObject  statusesObj = jsonObj.value("statuses").toObject();
//     if (!statusesObj.isEmpty()) {
//         QJsonArray statusArray = statusesObj.value("data").toArray();
//         m_userInfo->status = statusArray[0].toObject().value("message").toString();
//     }
//     
//     // user avatar
//     QJsonObject pictureObj = jsonObj.value("picture").toObject();
//     if (!pictureObj.isEmpty()) {
//         QJsonObject pictureDataObj = pictureObj.value("data").toObject();
//         m_userInfo->picture = pictureDataObj.value("url").toString();
//     }
//     
//     
//     // user likes
//     QJsonObject likesObj = jsonObj.value("likes").toObject();
//     if (!likesObj.isEmpty()) {
//         QJsonArray likesArray = likesObj.value("data").toArray();
//         
//         for (int i = 0; i < likesArray.size(); ++i) {
//             QJsonObject likeDetail = likesArray.at(i).toObject();
//             
//             Like *like = new Like;
//             
//             like->category = likeDetail.value("category").toString();
//             like->createdTime = likeDetail.value("created_time").toString();
//             like->id = likeDetail.value("id").toString();
//             like->name = likeDetail.value("name").toString();
//             
//             // add to hash
//             m_likes.insert(like->id, like);
//             qDebug() << "NEW LIKE: " << like->id << " - " << like->category << " - " << like->name;
//         }
//     }
//     
//     
//     // friends
//     QJsonObject friendsObj = jsonObj.value("friends").toObject();
//     if (!friendsObj.isEmpty()) {
//         QJsonArray friendsArray = friendsObj.value("data").toArray();
//         
//         for (int i = 0; i < friendsArray.size(); ++i) {
//             QJsonObject friendDetail = friendsArray.at(i).toObject();
//             
//             Friend *myFriend = new Friend;
//             
//             myFriend->about = friendDetail.value("about").toString();
//             myFriend->birthday = friendDetail.value("birthday").toString();
//             myFriend->firstName = friendDetail.value("first_name").toString();
//             myFriend->gender = friendDetail.value("gender").toString();
//             myFriend->id = friendDetail.value("id").toString();
//             myFriend->lastName = friendDetail.value("last_name").toString();
//             myFriend->link = friendDetail.value("").toString();
//             myFriend->locale = friendDetail.value("locale").toString();
//             myFriend->name = friendDetail.value("name").toString();
//             myFriend->picture = friendDetail.value("").toString();
//             myFriend->relationshipStatus = friendDetail.value("relationship_status").toString();
//             myFriend->username = friendDetail.value("username").toString();
//             
//             // picture
//             QJsonObject friendPictureObj = friendDetail.value("picture").toObject();
//             if (!friendPictureObj.isEmpty()) {
//                 QJsonObject pictureDataObj = friendPictureObj.value("data").toObject();
//                 myFriend->picture = friendPictureObj.value("url").toString();
//             }
//             
//             // TODO education/work
//             
//             m_friends.insert(myFriend->id, myFriend);
//             qDebug() << "new friend: " << myFriend->picture << " - " << myFriend->firstName << " - " << myFriend->lastName << " - " << myFriend->id;
//         }
//     }
//     
//     
//     // work
//     QJsonArray workArray = jsonObj.value("work").toArray();
//     
//     for (int i = 0; i < workArray.size(); ++i) {
//         Work *work = new Work;
//         QJsonObject workObj = workArray.at(i).toObject();
//         
//         work->description = workObj.value("description").toString();
//         work->endDate = QDate::fromString(workObj.value("end_date").toString(), "yyyy-MM-dd");
//         work->startDate = QDate::fromString(workObj.value("start_date").toString(), "yyyy-MM-dd");
//         
//         // employer
//         QJsonObject workEmployer = workObj.value("employer").toObject();
//         if (!workEmployer.isEmpty()) {
//             work->employer.id = workEmployer.value("id").toString();
//             work->employer.name = workEmployer.value("name").toString();
//         }
//         
//         // location
//         QJsonObject workLocation = workObj.value("location").toObject();
//         if (!workLocation.isEmpty()) {
//             work->location.id = workLocation.value("id").toString();
//             work->location.name = workLocation.value("name").toString();
//         }
//         
//         // position
//         QJsonObject workPosition = workObj.value("position").toObject();
//         if (!workPosition.isEmpty()) {
//             work->position.id = workPosition.value("id").toString();
//             work->position.name = workPosition.value("name").toString();
//         }
//         
//         m_work.append(work);
//         qDebug() << "WORK - " << work->employer.name << " - " << work->description << " - " << work->startDate.toString();
//     }
//     
//     
//     // education
//     QJsonArray educationArray = jsonObj.value("education").toArray();
//     
//     for (int i = 0; i < educationArray.size(); ++i) {
//         Education *education = new Education;
//         
//         QJsonObject educationObj = educationArray.at(i).toObject();
//         education->type = educationObj.value("type").toString();
//         
//         QJsonObject educationSchool = educationObj.value("school").toObject();
//         education->school.id = educationSchool.value("id").toString();;
//         education->school.name = educationSchool.value("name").toString();
//         
//         m_education.append(education);
//         qDebug() << "EDUCATION: " << education->type << " - " << education->school.id << " - " << education->school.name;
//     }
}
#endif


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
    reqStr += ",friends.fields(id,name,username,first_name,last_name,locale,gender,picture.type(large))";                  // friends
//                 ",games.fields(id,link,website,name,picture.type(large))";                           // games
//                 ",music.fields(id,bio,description,hometown,link,name,picture.type(large),website)";  // music
    reqStr += "&access_token=" + m_authToken;

    qDebug() << "[Facebook::populateData] requesting: " << reqStr;

    req.setUrl(QUrl(reqStr));
    netRep = m_networkAccessManager->get(req);

    // connect
    connect(netRep, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onNetReplyError(QNetworkReply::NetworkError)));

#ifdef USING_QT5
    connect(netRep, SIGNAL(finished()), this, SLOT(onPopulateDataReplyReceived()));
#else
    connect(netRep, SIGNAL(finished()), this, SLOT(onPopulateDataReplyReceivedQt4()));
#endif
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

    return m_friends.values();
}


QList<Facebook::Work*> Facebook::work() const
{
    qDebug("[Facebook::work]");

    return m_work;
}


QList<Facebook::Education*> Facebook::education() const
{
    qDebug("[Facebook::education]");

    return m_education;
}


QList<Facebook::Like*> Facebook::userLikes() const
{
    qDebug("[Facebook::userLikes]");

    return m_likes.values();
}
