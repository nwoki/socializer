/*
 * linkedin.cpp
 *
 * This file is part of the Socializer library
 *
 * Author Francesco Nwokeka <francesco.nwokeka@gmail.com>
 */

#include "linkedin.h"
#include "linkedinuser.h"

#include <QtCore/QDebug>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>

#include <qabstractitemmodel.h>

#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeContext>

#define AUTH_URL "https://www.linkedin.com/uas/oauth2/authorization?"
#define ACCESS_TOKEN_URL "https://www.linkedin.com/uas/oauth2/accessToken?"
#define UPDATE_INFO_URL "https://api.linkedin.com/v1/people/~"

namespace Socializer {


LinkedIn::LinkedIn(const QByteArray &authToken, bool updateOnCreate, QObject *parent)
    : OAuth(authToken, parent)
    , m_linkedinUser(new LinkedInUser(this))
{
    if (updateOnCreate) {
        populateData();
    }
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


void LinkedIn::checkLastUpdateTime() const
{
    qDebug("[LinkedIn::checkLastUpdateTime]");

    QString reqUrl(UPDATE_INFO_URL);
    QString infoStr(":(last-modified-timestamp)?format=json");

    reqUrl.append(infoStr);
    reqUrl.append("&oauth2_access_token=" + authToken());

    QNetworkRequest req(reqUrl);
    QNetworkReply *netRep = m_networkAccessManager->get(req);

    connect(netRep, SIGNAL(finished()), this, SLOT(onLastUpdatedTimeReceived()));
    connect(netRep, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onNetReplyError(QNetworkReply::NetworkError)));
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


    for (const QString &scope : scopeList) {
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


    QJsonObject jsonObj = jsonObject(rcv);

    if (jsonObj.isEmpty()) {
        // error occured
        return;
    }

    // extract auth token
    setAuthToken(jsonObj.value("access_token").toString().toLatin1());
}


void LinkedIn::onAuthTokenChanged()
{
    qDebug("[LinkedIn::onAuthTokenChanged]");
    qDebug() << "[LinkedIn::onAuthTokenChanged] new token: " << m_authToken;

    populateData();
}


void LinkedIn::onLastUpdatedTimeReceived()
{
    qDebug("[LinkedIn::onLastUpdatedTimeReceived]");

    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    if (reply->error() != QNetworkReply::NoError) {
        return;
    }

    QByteArray rcv = reply->readAll();
    reply->deleteLater();

    QJsonObject jsonObj = jsonObject(rcv);

    // check if new time differs from old. If so, update the profile data
    if (QString::number((jsonObj.value("lastModifiedTimestamp").toVariant().toULongLong())) != m_linkedinUser->lastUpdatedTime()) {
        populateData();
    } else {
        Q_EMIT profileUpdated();
    }
}


void LinkedIn::onNetReplyError(QNetworkReply::NetworkError error)
{
    qDebug("[LinkedIn::onNetReplyError]");
    Q_UNUSED(error);

    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    QByteArray rcv = reply->readAll();
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    reply->deleteLater();

    QJsonObject jsonObj = jsonObject(rcv);

    if (jsonObj.isEmpty()) {
        qWarning("[LinkedIn::profileInfoReceived] error parsing json");
        return;
    }

    // problem with access token
    if (statusCode == 401) {
        if (jsonObj.value("message").toString().contains("invalid", Qt::CaseInsensitive)) {
            Q_EMIT authTokenError(Invalid);
        } else if (jsonObj.value("message").toString().contains("expired", Qt::CaseInsensitive)) {
            Q_EMIT authTokenError(Expired);
        } else {
            Q_EMIT authTokenError(Unknown);
        }
    }
}


void LinkedIn::populateData()
{
    qDebug("[LinkedIn::populateData]");

    QString reqUrl(UPDATE_INFO_URL);
    QString infoStr(":(");

    // personal data
    infoStr += "id,first-name,last-name,email-address,date-of-birth,group-memberships,last-modified-timestamp";

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
        Q_EMIT authTokenError(Unknown);
        return;
    }

    QByteArray rcv = rep->readAll();

    rep->deleteLater();

    qDebug() << "[LinkedIn::profileInfoReceived] json received: " << rcv;

    QJsonObject jsonObj = jsonObject(rcv);

    if (jsonObj.isEmpty()) {
        qWarning("[LinkedIn::profileInfoReceived] error parsing json");
        return;
    }

    m_linkedinUser->setEmail(jsonObj.value("emailAddress").toString());
    m_linkedinUser->setFirstName(jsonObj.value("firstName").toString());
    m_linkedinUser->setLastName(jsonObj.value("lastName").toString());
    m_linkedinUser->setHeadLine(jsonObj.value("headline").toString());
    m_linkedinUser->setIndustry(jsonObj.value("industry").toString());

    m_linkedinUser->setNumberOfConnections(jsonObj.value("numConnections").toVariant().toInt());
    m_linkedinUser->setNumberOfRecommenders(jsonObj.value("numReccomenders").toVariant().toInt());
    m_linkedinUser->setLastUpdatedTime(QString::number(jsonObj.value("lastModifiedTimestamp").toVariant().toULongLong()));

    m_linkedinUser->setProfileId(jsonObj.value("id").toString());
    m_linkedinUser->setProfilePictureUrl(jsonObj.value("pictureUrl").toString());
    m_linkedinUser->setPublicProfileUrl(jsonObj.value("publicProfileUrl").toString());


    // BIRTHDAY
    QJsonObject birthdayObj = jsonObj.value("dateOfBirth").toObject();
    m_linkedinUser->setBirthday(QDate(birthdayObj.value("year").toVariant().toInt()
                                    , birthdayObj.value("month").isUndefined() ? 1 : birthdayObj.value("month").toVariant().toInt()
                                    , birthdayObj.value("day").isUndefined() ? 1 : birthdayObj.value("month").toVariant().toInt()));

    // EDUCATION
    QJsonObject educationsObj = jsonObj.value("educations").toObject();
    QJsonArray educationsList = educationsObj.value("values").toArray();

    for (const QJsonValue &eduValue : educationsList) {
        LinkedInUser::Education edu;
        QJsonObject eduObj = eduValue.toObject();
        QJsonObject eduStartDate = eduObj.value("startDate").toObject();
        QJsonObject eduEndDate = eduObj.value("endDate").toObject();

        edu.id = QString::number(eduObj.value("id").toVariant().toInt());

        if (!eduStartDate.value("year").isNull()) {
            edu.startDate = QDate(eduStartDate.value("year").toVariant().toInt(),
                                  eduStartDate.value("month").isNull() ? 1 : eduStartDate.value("month").toVariant().toInt(),
                                  eduStartDate.value("day").isNull() ? 1 : eduStartDate.value("day").toVariant().toInt());
        }

        if (!eduEndDate.value("year").isNull()) {
            edu.endDate = QDate(eduEndDate.value("year").toVariant().toInt(),
                                eduEndDate.value("month").isNull() ? 1 : eduEndDate.value("month").toVariant().toInt(),
                                eduEndDate.value("day").isNull() ? 1 : eduEndDate.value("day").toVariant().toInt());
        }

        edu.activities = eduObj.value("activities").toString();
        edu.degree = eduObj.value("degree").toString();
        edu.fieldOfStudy = eduObj.value("fieldOfStudy").toString();
        edu.notes = eduObj.value("notes").toString();
        edu.schoolName = eduObj.value("schoolName").toString();

        m_linkedinUser->addEducation(edu.id, edu);
    }


    // GROUPS
    QJsonObject groupMembershipsObj = jsonObj.value("groupMemberships").toObject();
    QJsonArray groupMembershipsList = groupMembershipsObj.value("values").toArray();

    for (const QJsonValue &groupValue : groupMembershipsList) {
        LinkedInUser::Group group;
        QJsonObject groupObj = groupValue.toObject();

        QJsonObject groupData = groupObj.value("group").toObject();
        QJsonObject membershipData = groupObj.value("membershipState").toObject();

        group.id = QString::number(groupData.value("id").toVariant().toInt());

        group.name = groupData.value("name").toString();
        group.membershipState = membershipData.value("code").toString();

        m_linkedinUser->addGroup(group.id, group);
    }


    // LANGUAGES
    QJsonObject languageObj = jsonObj.value("languages").toObject();
    QJsonArray languageList = languageObj.value("values").toArray();

    for (const QJsonValue &langVal : languageList) {
        LinkedInUser::Language lang;
        lang.language = langVal.toObject().value("language").toObject().value("name").toString();
        lang.id = QString::number(langVal.toObject().value("id").toVariant().toInt());
        m_linkedinUser->addLanguage(lang.id, lang);
    }


    // LOCATION
    m_linkedinUser->setLocation(jsonObj.value("location").toObject().value("name").toString());


    // POSITIONS
    QJsonObject positionsObj = jsonObj.value("positions").toObject();
    QJsonArray positionsList = positionsObj.value("values").toArray();

    for (const QJsonValue &positionVal : positionsList) {
        LinkedInUser::Position position;

        QJsonObject positionObj = positionVal.toObject();
        QJsonObject companyObj = positionObj.value("company").toObject();
        QJsonObject startDateObj = positionObj.value("startDate").toObject();

        position.id = QString::number(positionObj.value("id").toVariant().toInt());
        position.company.id = QString::number(companyObj.value("id").toVariant().toInt());
        position.isCurrent = companyObj.value("isCurrent").toBool();

        if (!position.isCurrent) {
            QJsonObject endDateObj = positionObj.value("endDate").toObject();

            if (!endDateObj.value("year").isNull()) {
                position.endDate = QDate(endDateObj.value("year").toVariant().toInt(),
                                         endDateObj.value("month").isUndefined() ? 1 : endDateObj.value("month").toVariant().toInt(),
                                         endDateObj.value("day").isUndefined() ? 1 : endDateObj.value("day").toVariant().toInt());
            }

        }

        if (!startDateObj.value("year").isNull()) {
            position.startDate = QDate(startDateObj.value("year").toVariant().toInt(),
                                       startDateObj.value("month").isUndefined() ? 1 : startDateObj.value("month").toVariant().toInt(),
                                       startDateObj.value("day").isUndefined() ? 1 : startDateObj.value("day").toVariant().toInt());
        }

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
    QJsonObject recommendationObj = jsonObj.value("recommendationsReceived").toObject();
    QJsonArray recommendationList = recommendationObj.value("values").toArray();

    for (const QJsonValue &recommendationValue : recommendationList) {
        LinkedInUser::Recommendation recommendation;
        QJsonObject recommendationObj = recommendationValue.toObject();

        recommendation.id = QString::number(recommendationObj.value("id").toVariant().toInt());
        recommendation.text = recommendationObj.value("text").toString();
        recommendation.type = recommendationObj.value("recommendationType").toObject().value("code").toString();

        recommendation.recommender.id = QString::number(recommendationObj.value("recommender").toObject().value("id").toVariant().toInt());
        recommendation.recommender.firstName = recommendationObj.value("recommender").toObject().value("firstName").toString();
        recommendation.recommender.lastName = recommendationObj.value("recommender").toObject().value("lastName").toString();

        m_linkedinUser->addRecommendation(recommendation.id, recommendation);
    }


    // SKILLS
    QJsonObject skillsObj = jsonObj.value("skills").toObject();
    QJsonArray skillsList = skillsObj.value("values").toArray();

    for (const QJsonValue &skillValue : skillsList) {
        m_linkedinUser->addSkill(QString::number(skillValue.toObject().value("id").toVariant().toInt())
                                , skillValue.toObject().value("skill").toObject().value("name").toString());
    }


    Q_EMIT profileUpdated();

    qDebug() << "\n\nUSE IFNO: " << m_linkedinUser->profileId() <<  m_linkedinUser->firstName() << " " << m_linkedinUser->lastName() << " " << m_linkedinUser->headline()
            << " " << m_linkedinUser->location() << " " << m_linkedinUser->locationCountryCode() << " " << m_linkedinUser->lastUpdatedTime();

}


void LinkedIn::update()
{
    // get new "lastUpdatedTime" from the fb server. We don't know if the user has modified
    // any info
    m_linkedinUser->lastUpdatedTime().isEmpty() ? populateData() : checkLastUpdateTime();
}


// DEPRECATED
// void LinkedIn::setContextProperty(QDeclarativeView *view)
// {
//     qDebug("[LinkedIn::setContextProperty]");
//
//     view->rootContext()->setContextProperty("LinkedIn", this);
// }


}       // Socializer




