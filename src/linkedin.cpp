/*
 * linkedin.cpp
 *
 * This file is part of the Socializer library
 * Copyright (C) 2012 Ispirata <info@ispirata.com>
 *
 * Author Francesco Nwokeka <francesco@ispirata.com>
 */

#include "linkedin.h"
#include "linkedinuser.h"

#include <QtCore/QDebug>

#ifdef USING_QT5
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#else
#include <qjson/parser.h>
#endif

#include <qabstractitemmodel.h>

#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeContext>

#define AUTH_URL "https://www.linkedin.com/uas/oauth2/authorization?"
#define ACCESS_TOKEN_URL "https://www.linkedin.com/uas/oauth2/accessToken?"
#define UPDATE_INFO_URL "https://api.linkedin.com/v1/people/~"

namespace Socializer {


LinkedIn::LinkedIn(const QByteArray &authToken, QObject *parent)
    : OAuth(authToken, parent)
    , m_linkedinUser(new LinkedInUser(this))
{
    populateData();
}


LinkedIn::LinkedIn(const QByteArray &authCode, const QByteArray &appId, const QByteArray &consumerSecret, const QByteArray &redirectUrl, QObject *parent)
    : OAuth(appId, redirectUrl, consumerSecret, parent)
    , m_linkedinUser(new LinkedInUser(this))
{
    // scopes TODO set in constructor

    connect(this, SIGNAL(authTokenChanged()), this, SLOT(onAccessTokenChanged()));

    if (authCode.isEmpty()) {
        obtainAuthPageUrl();
    } else {
        requestAuthToken(authCode);
    }
}


LinkedIn::~LinkedIn()
{
}


bool LinkedIn::basicProfileScope() const
{
    qDebug("[LinkedIn::basicProfileScope]");

    return m_basicProfileScope;
}


bool LinkedIn::contactInfoScope() const
{
    qDebug("[LinkedIn::contactInfoScope]");

    return m_contactInfoScope;
}


bool LinkedIn::emailProfileScope() const
{
    qDebug("[LinkedIn::emailProfileScope]");

    return m_emailAddressScope;
}


bool LinkedIn::fullProfileScope() const
{
    qDebug("[LinkedIn::fullProfileScope]");

    return m_fullProfileScope;
}


LinkedInUser *LinkedIn::userInfo() const
{
    qDebug("[LinkedIn::userInfo]");

    return m_linkedinUser;
}


bool LinkedIn::networkScope() const
{
    qDebug("[LinkedIn::networkScope]");

    return m_networkScope;
}


void LinkedIn::setBasicProfileScope(bool enable)
{
    qDebug("[LinkedIn::setBasicProfileScope]");

    m_basicProfileScope = enable;
}


void LinkedIn::setContactInfoScope(bool enable)
{
    qDebug("[LinkedIn::setContactInfoScope]");

    m_contactInfoScope = enable;
}


void LinkedIn::setEmailProfileScope(bool enable)
{
    qDebug("[LinkedIn::setEmailProfileScope]");

    m_emailAddressScope = enable;
}


void LinkedIn::setFullProfileScope(bool enable)
{
    qDebug("[LinkedIn::setFullProfileScope]");

    m_fullProfileScope = enable;
}


void LinkedIn::setNetworkScope(bool enable)
{
    qDebug("[LinkedIn::setNetworkScope]");

    m_networkScope = enable;
}


QString LinkedIn::createScope()
{
    qDebug("[LinkedIn::createScope]");

    QList<QString> scopeList;
    QString scopeLine;
    bool isFirst = true;

    if (m_basicProfileScope && !m_fullProfileScope) {
        scopeList.append("r_basicprofile");
    } else if (!m_basicProfileScope && m_fullProfileScope) {
        scopeList.append("r_fullprofile");
    }

    if (m_contactInfoScope) {
        scopeList.append("r_contactinfo");
    }

    if (m_emailAddressScope) {
        scopeList.append("r_emailaddress");
    }

    if (m_networkScope) {
        scopeList.append("r_network");
    }


    Q_FOREACH (const QString &scope, scopeList) {
        if (!isFirst) {
            scopeLine.append(' ');
        } else {
            isFirst = false;
        }

        scopeLine.append(scope);
    }

    if (!isFirst) {
        return "&scope=" + QUrl::toPercentEncoding(scopeLine);
    } else {
        return QString();
    }
}


void LinkedIn::obtainAuthPageUrl()
{
    qDebug("[LinkedIn::obtainAuthPageUrl]");

    QString urlStr(AUTH_URL);

    urlStr.append("response_type=code");
    urlStr.append("&client_id=");
    urlStr.append(m_appId);
    urlStr.append(createScope());
    urlStr.append("&state=");
    urlStr.append(nonce());             // here any value will suffice. To prevent CSRF
    urlStr.append("&redirect_uri=");
    urlStr.append(m_redirectUrl);

    qDebug() << "[LinkedIn::obtainAuthPageUrl] " << urlStr;
    Q_EMIT authPageUrlReady(urlStr);
}


void LinkedIn::onAccessTokenChanged()
{
    qDebug("[LinkedIn::onAccessTokenChanged]");
    populateData();
}


void LinkedIn::onAccessTokenReceived()
{
    qDebug("[LinkedIn::onAccessTokenReceived]");

    QNetworkReply *netReply = qobject_cast<QNetworkReply*>(sender());
    QByteArray rcv = netReply->readAll();

    qDebug() << "[LinkedIn::onAccessTokenReceived] got: " << rcv;

    netReply->deleteLater();

#ifdef USING_QT5
    QJsonObject jsonObj = jsonObject(rcv);

    if (jsonObj.isEmpty()) {
        // error occured
        return;
    }

    // extract auth token
    setAuthToken(jsonObj.value("access_token").toString().toLatin1());
#else
    QJson::Parser parser;
    bool ok;

    QVariantMap jsonMap = parser.parse(rcv, &ok).toMap();

    if (!ok) {
        // error occored
        return;
    }

    // extract auth token
    setAuthToken(jsonMap.value("access_token").toString().toLatin1());
#endif
}


void LinkedIn::onAuthTokenChanged()
{
    qDebug("[LinkedIn::onAuthTokenChanged]");
    qDebug() << "[LinkedIn::onAuthTokenChanged] new token: " << m_authToken;

    populateData();
}


void LinkedIn::onNetReplyError(QNetworkReply::NetworkError error)
{
    qDebug("[LinkedIn::onNetReplyError]");
    Q_UNUSED(error);

    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    QByteArray rcv = reply->readAll();
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    reply->deleteLater();

#ifdef USING_QT5
    QJsonObject jsonObj = jsonObject(rcv);

    if (jsonObj.isEmpty()) {
        qWarning("[LinkedIn::profileInfoReceived] error parsing json");
        return;
    }
#else
    QVariantMap jsonObj = jsonObject(rcv);
#endif

    // problem with access token
    if (statusCode == 401) {
        if (jsonObj.value("message").toString().contains("invalid", Qt::CaseInsensitive)) {
            Q_EMIT authTokenInvalid();
        } else if (jsonObj.value("message").toString().contains("expired", Qt::CaseInsensitive)) {
            Q_EMIT authTokenExpired();
        }
    }
}


void LinkedIn::populateData()
{
    qDebug("[LinkedIn::populateData]");

    // for every scope, update use info
    updateProfileInfo();
}


void LinkedIn::requestAuthToken(const QString &code)
{
    qDebug("[LinkedIn::requestAuthToken]");
    qDebug() << "[LinkedIn::requestAuthToken] with code: " << code;

    QNetworkRequest req;
    QNetworkReply *netRep;
    QString reqUrl(ACCESS_TOKEN_URL);

    reqUrl.append("grant_type=authorization_code");
    reqUrl.append("&code=" + code);
    reqUrl.append("&redirect_uri=" + redirectUrl());
    reqUrl.append("&client_id=" + m_appId);
    reqUrl.append("&client_secret=" + m_consumerSecret);

    qDebug() << reqUrl;

    req.setUrl(reqUrl);

    netRep = m_networkAccessManager->post(req, QByteArray());

    connect(netRep, SIGNAL(finished()), this, SLOT(onAccessTokenReceived()));
    connect(netRep, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onNetReplyError(QNetworkReply::NetworkError)));
}


void LinkedIn::prepareAuthPageUrl()
{
    // once i have the access token, return the authentication url
    QString authPageUrl(AUTH_URL);
    authPageUrl += "?oauth_token=" + m_authToken;

    qDebug() << "[LinkedIn::prepareAuthPageUrl] Emitting auth page url: " << authPageUrl;

    Q_EMIT authPageUrlReady(authPageUrl);
}


void LinkedIn::profileInfoReceived()
{
    qDebug("[LinkedIn::profileInfoReceived]");

    QNetworkReply *rep = qobject_cast<QNetworkReply*>(sender());

    if (rep->error() != QNetworkReply::NoError) {
        return;
    }

    QByteArray rcv = rep->readAll();

    rep->deleteLater();

    qDebug() << "[LinkedIn::profileInfoReceived] json received: " << rcv;

#ifdef USING_QT5
    QJsonObject jsonObj = jsonObject(rcv);

    if (jsonObj.isEmpty()) {
        qWarning("[LinkedIn::profileInfoReceived] error parsing json");
        return;
    }
#else
    QJson::Parser parser;
    QVariantMap jsonObj = jsonObject(rcv);
#endif

    m_linkedinUser->setEmail(jsonObj.value("emailAddress").toString());
    m_linkedinUser->setFirstName(jsonObj.value("firstName").toString());
    m_linkedinUser->setLastName(jsonObj.value("lastName").toString());
    m_linkedinUser->setHeadLine(jsonObj.value("headline").toString());
    m_linkedinUser->setIndustry(jsonObj.value("industry").toString());

#ifdef USING_QT5
    m_linkedinUser->setNumberOfConnections(jsonObj.value("numConnections").toVariant().toInt());
    m_linkedinUser->setNumberOfRecommenders(jsonObj.value("numReccomenders").toVariant().toInt());
#else
    m_linkedinUser->setNumberOfConnections(jsonObj.value("numConnections").toInt());
    m_linkedinUser->setNumberOfRecommenders(jsonObj.value("numReccomenders").toInt());
#endif

    m_linkedinUser->setProfileId(jsonObj.value("id").toString());
    m_linkedinUser->setProfilePictureUrl(jsonObj.value("pictureUrl").toString());
    m_linkedinUser->setPublicProfileUrl(jsonObj.value("publicProfileUrl").toString());


    // BIRTHDAY
#ifdef USING_QT5
    QJsonObject birthdayObj = jsonObj.value("dateOfBirth").toObject();
    m_linkedinUser->setBirthday(QDate(birthdayObj.value("year").toVariant().toInt()
                                    , birthdayObj.value("month").isUndefined() ? 1 : birthdayObj.value("month").toVariant().toInt()
                                    , birthdayObj.value("day").isUndefined() ? 1 : birthdayObj.value("month").toVariant().toInt()));
#else
    QVariantMap birthdayObj = jsonObj.value("dateOfBirth").toMap();
    m_linkedinUser->setBirthday(QDate(birthdayObj.value("year").toInt()
                                    , birthdayObj.value("month").isNull() ? 1 : birthdayObj.value("month").toInt()
                                    , birthdayObj.value("day").isNull() ? 1 : birthdayObj.value("day").toInt()));
#endif


    // EDUCATION
#ifdef USING_QT5
    QJsonObject educationsObj = jsonObj.value("educations").toObject();
    QJsonArray educationsList = educationsObj.value("values").toArray();

    Q_FOREACH (QJsonValue eduValue, educationsList) {
        LinkedInUser::Education edu;
        QJsonObject eduObj = eduValue.toObject();
        QJsonObject eduStartDate = eduObj.value("startDate").toObject();
        QJsonObject eduEndDate = eduObj.value("endDate").toObject();

        edu.id = QString::number(eduObj.value("id").toVariant().toInt());

        edu.startDate = QDate(eduStartDate.value("year").toVariant().toInt()
        , eduStartDate.value("month").toVariant().toInt()
        , eduStartDate.value("day").toVariant().toInt()).toString();

        edu.endDate = QDate(eduEndDate.value("year").toVariant().toInt()
        , eduEndDate.value("month").toVariant().toInt()
        , eduEndDate.value("day").toVariant().toInt()).toString();
#else
    QVariantMap educationsObj = jsonObj.value("educations").toMap();
    QList<QVariant> educationsList = educationsObj.value("values").toList();

    Q_FOREACH (QVariant eduValue, educationsObj.value("values").toList()) {

        LinkedInUser::Education edu;
        QVariantMap eduObj = eduValue.toMap();
        QVariantMap eduStartDate = eduObj.value("startDate").toMap();
        QVariantMap eduEndDate = eduObj.value("endDate").toMap();

        edu.id = QString::number(eduObj.value("id").toInt());

        edu.startDate = QDate(eduStartDate.value("year").toInt()
                            , eduStartDate.value("month").isNull() ? 1 : eduStartDate.value("month").toInt()
                            , eduStartDate.value("day").isNull() ? 1 : eduStartDate.value("day").toInt()).toString();

        edu.endDate = QDate(eduEndDate.value("year").toInt()
                            , eduEndDate.value("month").isNull() ? 1 : eduEndDate.value("month").toInt()
                            , eduEndDate.value("day").isNull() ? 1 : eduEndDate.value("day").toInt()).toString();

#endif

        edu.activities = eduObj.value("activities").toString();
        edu.degree = eduObj.value("degree").toString();
        edu.fieldOfStudy = eduObj.value("fieldOfStudy").toString();
        edu.notes = eduObj.value("notes").toString();
        edu.schoolName = eduObj.value("schoolName").toString();

        m_linkedinUser->addEducation(edu.id, edu);
    }


    // GROUPS
#ifdef USING_QT5
    QJsonObject groupMembershipsObj = jsonObj.value("groupMemberships").toObject();
    QJsonArray groupMembershipsList = groupMembershipsObj.value("values").toArray();

    Q_FOREACH (QJsonValue groupValue, groupMembershipsList) {
        LinkedInUser::Group group;
        QJsonObject groupObj = groupValue.toObject();

        QJsonObject groupData = groupObj.value("group").toObject();
        QJsonObject membershipData = groupObj.value("membershipState").toObject();

        group.id = QString::number(groupData.value("id").toVariant().toInt());
#else
    QVariantMap groupMembershipsObj = jsonObj.value("groupMemberships").toMap();

    Q_FOREACH (QVariant groupValue, groupMembershipsObj.value("values").toList()) {
        LinkedInUser::Group group;
        QVariantMap groupObj = groupValue.toMap();

        QVariantMap groupData = groupObj.value("group").toMap();
        QVariantMap membershipData = groupObj.value("membershipState").toMap();

        group.id = QString::number(groupData.value("id").toInt());
#endif

        group.name = groupData.value("name").toString();
        group.membershipState = membershipData.value("code").toString();

        m_linkedinUser->addGroup(group.id, group);
    }


    // LANGUAGES
#ifdef USING_QT5
    QJsonObject languageObj = jsonObj.value("languages").toObject();
    QJsonArray languageList = languageObj.value("values").toArray();

    Q_FOREACH (QJsonValue langVal, languageList) {
        LinkedInUser::Language lang;
        lang.language = langVal.toObject().value("language").toObject().value("name").toString();
        lang.id = QString::number(langVal.toObject().value("id").toVariant().toInt());
#else
    QVariantMap languageObj = jsonObj.value("languages").toMap();

    Q_FOREACH (QVariant langVal, languageObj.value("values").toList()) {
        LinkedInUser::Language lang;
        lang.language = langVal.toMap().value("language").toMap().value("name").toString();
        lang.id = QString::number(langVal.toMap().value("id").toInt());
#endif

        m_linkedinUser->addLanguage(lang.id, lang);
    }


    // LOCATION
#ifdef USING_QT5
    m_linkedinUser->setLocation(jsonObj.value("location").toObject().value("name").toString());
#else
    m_linkedinUser->setLocation(jsonObj.value("location").toMap().value("name").toString());
#endif


    // POSITIONS
#ifdef USING_QT5
    QJsonObject positionsObj = jsonObj.value("positions").toObject();
    QJsonArray positionsList = positionsObj.value("values").toArray();

    Q_FOREACH (QJsonValue positionVal, positionsList) {
        LinkedInUser::Position position;

        QJsonObject positionObj = positionVal.toObject();
        QJsonObject companyObj = positionObj.value("company").toObject();
        QJsonObject startDateObj = positionObj.value("startDate").toObject();

        position.id = QString::number(positionObj.value("id").toVariant().toInt());
        position.company.id = QString::number(companyObj.value("id").toVariant().toInt());
        position.isCurrent = companyObj.value("isCurrent").toBool();

        if (position.isCurrent) {
            QJsonObject endDateObj = positionObj.value("endDate").toObject();

            position.endDate = QDate(endDateObj.value("year").toVariant().toInt()
                                    , endDateObj.value("month").isUndefined() ? 1 : endDateObj.value("month").toVariant().toInt()
                                    , endDateObj.value("day").isUndefined() ? 1 : endDateObj.value("day").toVariant().toInt()).toString();

        }

        position.startDate = QDate(startDateObj.value("year").toVariant().toInt()
                                , startDateObj.value("month").isUndefined() ? 1 : startDateObj.value("month").toVariant().toInt()
                                , startDateObj.value("day").isUndefined() ? 1 : startDateObj.value("day").toVariant().toInt()).toString();

#else
    QVariantMap positionsObj = jsonObj.value("positions").toMap();

    Q_FOREACH (QVariant positionVal, positionsObj.value("values").toList()) {
        LinkedInUser::Position position;

        QVariantMap positionObj = positionVal.toMap();
        QVariantMap companyObj = positionObj.value("company").toMap();
        QVariantMap startDateObj = positionObj.value("startDate").toMap();

        position.id = QString::number(positionObj.value("id").toInt());
        position.company.id = QString::number(companyObj.value("id").toInt());
        position.isCurrent = companyObj.value("isCurrent").toBool();

        if (position.isCurrent) {
            QVariantMap endDateObj = positionObj.value("endDate").toMap();

            position.endDate = QDate(endDateObj.value("year").toInt()
            , endDateObj.value("month").isNull() ? 1 : endDateObj.value("month").toInt()
            , endDateObj.value("day").isNull() ? 1 : endDateObj.value("day").toInt()).toString();

        }

        position.startDate = QDate(startDateObj.value("year").toInt()
        , startDateObj.value("month").isNull() ? 1 : startDateObj.value("month").toInt()
        , startDateObj.value("day").isNull() ? 1 : startDateObj.value("day").toInt()).toString();
#endif

        position.isCurrent = positionObj.value("isCurrent").toBool();
        position.summary = positionObj.value("summary").toString();
        position.title = positionObj.value("title").toString();

        // company
        position.company.industry = companyObj.value("industry").toString();
        position.company.name = companyObj.value("name").toString();
        position.company.size = companyObj.value("size").toString();
        position.company.ticker = companyObj.value("ticker").toString();
        position.company.type = companyObj.value("type").toString();

        m_linkedinUser->addPosition(position.id, position);
    }


    // RECCOMENDATIONS
#ifdef USING_QT5
    QJsonObject recommendationObj = jsonObj.value("recommendationsReceived").toObject();
    QJsonArray recommendationList = recommendationObj.value("values").toArray();

    Q_FOREACH (QJsonValue recommendationValue, recommendationList) {
        LinkedInUser::Recommendation recommendation;
        QJsonObject recommendationObj = recommendationValue.toObject();

        recommendation.id = QString::number(recommendationObj.value("id").toVariant().toInt());
        recommendation.text = recommendationObj.value("text").toString();
        recommendation.type = recommendationObj.value("recommendationType").toObject().value("code").toString();

        recommendation.recommender.id = QString::number(recommendationObj.value("recommender").toObject().value("id").toVariant().toInt());
        recommendation.recommender.firstName = recommendationObj.value("recommender").toObject().value("firstName").toString();
        recommendation.recommender.lastName = recommendationObj.value("recommender").toObject().value("lastName").toString();
#else
    QVariantMap recommendationObj = jsonObj.value("recommendationsReceived").toMap();

    Q_FOREACH (QVariant recommendationValue, recommendationObj.value("values").toList()) {
        LinkedInUser::Recommendation recommendation;
        QVariantMap recommendationObj = recommendationValue.toMap();

        recommendation.id = QString::number(recommendationObj.value("id").toInt());
        recommendation.text = recommendationObj.value("text").toString();
        recommendation.type = recommendationObj.value("recommendationType").toMap().value("code").toString();

        recommendation.recommender.id = QString::number(recommendationObj.value("recommender").toMap().value("id").toInt());
        recommendation.recommender.firstName = recommendationObj.value("recommender").toMap().value("firstName").toString();
        recommendation.recommender.lastName = recommendationObj.value("recommender").toMap().value("lastName").toString();
#endif

        m_linkedinUser->addRecommendation(recommendation.id, recommendation);
    }


    // SKILLS
#ifdef USING_QT5
    QJsonObject skillsObj = jsonObj.value("skills").toObject();
    QJsonArray skillsList = skillsObj.value("values").toArray();

    Q_FOREACH (QJsonValue skillValue, skillsList) {
        m_linkedinUser->addSkill(QString::number(skillValue.toObject().value("id").toVariant().toInt())
                                , skillValue.toObject().value("skill").toObject().value("name").toString());
#else
    QVariantMap skillsObj = jsonObj.value("skills").toMap();

    Q_FOREACH (QVariant skillValue, skillsObj.value("values").toList()) {
        m_linkedinUser->addSkill(QString::number(skillValue.toMap().value("id").toInt())
                                , skillValue.toMap().value("skill").toMap().value("name").toString());
#endif
    }


    Q_EMIT profileUpdated();

    qDebug() << "\n\nUSE IFNO: " << m_linkedinUser->profileId() <<  m_linkedinUser->firstName() << " " << m_linkedinUser->lastName() << " " << m_linkedinUser->headline()
            << " " << m_linkedinUser->location() << " " << m_linkedinUser->locationCountryCode();

}


// DEPRECATED
// void LinkedIn::setContextProperty(QDeclarativeView *view)
// {
//     qDebug("[LinkedIn::setContextProperty]");
//
//     view->rootContext()->setContextProperty("LinkedIn", this);
// }


void LinkedIn::updateProfileInfo()
{
    qDebug("[LinkedIn::updateProfileInfo]");

    QString reqUrl(UPDATE_INFO_URL);

    // INFO STRING
    QString infoStr(":(");

    // personal data
    infoStr += "id,first-name,last-name,email-address,date-of-birth,group-memberships";

    // full profile info
    infoStr += ",associations,interests,languages,skills,certifications,educations,num-recommenders,recommendations-received";

    // linkedin gen data
    infoStr += ",num-connections,headline,location:(name,country:(code)),industry,summary,picture-url,public-profile-url,positions";

    // close and use json
    infoStr += ")?format=json";

    reqUrl.append(infoStr);

    reqUrl.append("&oauth2_access_token=" + authToken());

    QNetworkRequest req(reqUrl);
    QNetworkReply *netRep = m_networkAccessManager->get(req);

    connect(netRep, SIGNAL(finished()), this, SLOT(profileInfoReceived()));
    connect(netRep, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onNetReplyError(QNetworkReply::NetworkError)));
}

}       // Socializer




