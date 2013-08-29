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


bool LinkedIn::isResponseValid(const QByteArray &msg)
{
    qDebug("[LinkedIn::isResponseValid]");

    QXmlStreamReader xmlReader(msg);

    while (!xmlReader.atEnd()) {
        if (xmlReader.readNextStartElement()) {

            // TODO save error code and string
            if (xmlReader.name() == "error") {
                // extract error code and message
                while (!xmlReader.atEnd()) {
                    if (xmlReader.readNextStartElement()) {
                        if (xmlReader.name() == "status") {
                            qDebug() << "[LinkedIn::isResponseValid] ERROR CODE: " << xmlReader.readElementText();
                            return false;
                        }
                    }
                }
            }
        }
    }

    return true;
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
    // TODO
    Q_UNUSED(error);

    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    reply->deleteLater();
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


void LinkedIn::parseEducationXml(QXmlStreamReader &xmlStrReader)
{
    qDebug("[LinkedIn::parseEducationXml]");

    xmlStrReader.readNextStartElement();

    QString id;
    QString schoolName;
    QString notes;
    QString activities;
    QString degree;
    QString fieldOfStudy;
    QString startDate;
    QString endDate;

    while (xmlStrReader.name() != "educations") {
        if (xmlStrReader.isStartElement()) {
            if (xmlStrReader.name() == "id") {
                id = xmlStrReader.readElementText();
            } else if (xmlStrReader.name() == "school-name") {
                schoolName = xmlStrReader.readElementText();
            } else if (xmlStrReader.name() == "notes") {
                notes = xmlStrReader.readElementText();
            } else if (xmlStrReader.name() == "activities") {
                activities = xmlStrReader.readElementText();
            } else if (xmlStrReader.name() == "degree") {
                degree = xmlStrReader.readElementText();
            } else if (xmlStrReader.name() == "field-of-study") {
                fieldOfStudy = xmlStrReader.readElementText();
            } else if (xmlStrReader.name() == "start-date") {
                // move to next tag
                xmlStrReader.readNextStartElement();
                startDate = xmlStrReader.readElementText();
            } else if (xmlStrReader.name() == "end-date") {
                // move to next tag
                xmlStrReader.readNextStartElement();
                endDate = xmlStrReader.readElementText();
            }
        } else if (xmlStrReader.name() == "education") {
            // add to hash
            LinkedInUser::Education education;
            education.activities = activities;
            education.degree = degree;
            education.endDate = endDate;
            education.fieldOfStudy = fieldOfStudy;
            education.notes = notes;
            education.schoolName = schoolName;
            education.startDate = startDate;
            education.id = id;

            m_linkedinUser->addEducation(id, education);

            // clear
            id.clear();
            schoolName.clear();
            notes.clear();
            activities.clear();
            degree.clear();
            fieldOfStudy.clear();
            startDate.clear();
            endDate.clear();
        }

        xmlStrReader.readNext();
    }
}


void LinkedIn::parseLanguageXml(QXmlStreamReader &xmlStrReader)
{
    qDebug("[LinkedIn::parseLanguageXml]");

    bool flag = true;
    QString id;
    QString langName;

    xmlStrReader.readNextStartElement();

    while (xmlStrReader.name() != "languages") {
        if (xmlStrReader.isStartElement()) {
            if (xmlStrReader.name() == "id") {
                id = xmlStrReader.readElementText();
            } else if (xmlStrReader.name() == "name") {
                langName = xmlStrReader.readElementText();
            }
        } else {
            if (xmlStrReader.name() == "language" && !flag) {
                flag = true;
                LinkedInUser::Language lang;
                lang.language = langName;

                // add to hash
                m_linkedinUser->addLanguage(id, lang);
            } else if (xmlStrReader.name() == "language" && flag) {
                flag = false;
            }
        }

        xmlStrReader.readNext();
    }
}


void LinkedIn::parsePositionsXml(QXmlStreamReader &xmlStrReader)
{
    qDebug("[LinkedIn::parsePositionsXml]");

    xmlStrReader.readNextStartElement();

    QString id;
    QString title;
    QString startDate;
    QString endDate;
    bool isCurrent = false;
    QString summary;

    QString companyId;
    QString companyName;
    QString companySize;
    QString companyType;
    QString companyIndustry;

    bool inStartDate = false;
    bool inEndDate = false;

    // TODO start/end date

    while (xmlStrReader.name() != "positions") {
        if (xmlStrReader.isStartElement()) {
            if (xmlStrReader.name() == "id") {
                id = xmlStrReader.readElementText();
            } else if (xmlStrReader.name() == "title") {
                title = xmlStrReader.readElementText();
            } else if (xmlStrReader.name() == "is-current") {
                isCurrent = xmlStrReader.readElementText() == "true" ? true : false;
            } else if (xmlStrReader.name() == "summary") {
                summary = xmlStrReader.readElementText();
            } else if (xmlStrReader.name() == "start-date") {
                inStartDate = true;
            } else if (xmlStrReader.name() == "end-date") {
                inEndDate = true;
            } else if (xmlStrReader.name() == "company") {
                // save company data
                xmlStrReader.readNextStartElement();    // move to id
                companyId = xmlStrReader.readElementText();
                xmlStrReader.readNextStartElement();
                companyName = xmlStrReader.readElementText();
                xmlStrReader.readNextStartElement();
                companySize = xmlStrReader.readElementText();
                xmlStrReader.readNextStartElement();
                companyType = xmlStrReader.readElementText();
                xmlStrReader.readNextStartElement();
                companyIndustry = xmlStrReader.readElementText();
            } else if (xmlStrReader.name() == "year") {
                if (inStartDate) {
                    startDate += xmlStrReader.readElementText();
                } else if (inEndDate) {
                    endDate += xmlStrReader.readElementText();
                }
            } else if (xmlStrReader.name() == "month") {
                if (inStartDate) {
                    startDate.prepend(xmlStrReader.readElementText() + "-");
                } else if (inEndDate) {
                    endDate.prepend(xmlStrReader.readElementText() + "-");
                }
            } else if (xmlStrReader.name() == "day") {
                if (inStartDate) {
                    startDate.prepend(xmlStrReader.readElementText() + "-");
                } else if (inEndDate) {
                    endDate.prepend(xmlStrReader.readElementText() + "-");
                }
            }
        } else {
            if (xmlStrReader.name() == "position") {
                // add position

                LinkedInUser::Position position;
                position.isCurrent = isCurrent;
                position.startDate = startDate;
                position.endDate = endDate;
                position.summary = summary;
                position.title = title;

                // company
                position.company.id = companyId;
                position.company.industry = companyIndustry;
                position.company.name = companyName;
                position.company.size = companySize;
                position.company.type = companyType;

                m_linkedinUser->addPosition(id, position);

                // clear data
                id.clear();
                title.clear();
                startDate.clear();
                endDate.clear();
                isCurrent = false;
                summary.clear();

                companyId.clear();
                companyName.clear();
                companySize.clear();
                companyType.clear();
                companyIndustry.clear();
            } else if (xmlStrReader.name() == "start-date") {
                inStartDate = false;
            } else if (xmlStrReader.name() == "end-date") {
                inEndDate = false;
            }
        }

        xmlStrReader.readNext();
    }
}


void LinkedIn::parseRecommendationsXml(QXmlStreamReader &xmlStrReader)
{
    qDebug("[LinkedIn::parseRecommendationsXml]");

    QString id;
    QString recommendationText;
    QString recommendationType;
    QString recommenderId;
    QString recommenderFirstName;
    QString recommenderLastName;

    xmlStrReader.readNextStartElement();

    while (!xmlStrReader.isEndDocument() && xmlStrReader.name() != "recommendations-received") {
        if (xmlStrReader.isStartElement()) {
            if (xmlStrReader.name() == "id") {
                id = xmlStrReader.readElementText();
            } else if (xmlStrReader.name() == "recommendation-text") {
                recommendationText = xmlStrReader.readElementText();
            } else if (xmlStrReader.name() == "recommendation-type") {
                // move to next tag
                xmlStrReader.readNextStartElement();
                recommendationType = xmlStrReader.readElementText();
            } else if (xmlStrReader.name() == "recommender") {
                // store recommender info
                xmlStrReader.readNextStartElement();
                recommenderId = xmlStrReader.readElementText();
                xmlStrReader.readNextStartElement();
                recommenderFirstName = xmlStrReader.readElementText();
                xmlStrReader.readNextStartElement();
                recommenderLastName = xmlStrReader.readElementText();
            }
        } else if (xmlStrReader.name() == "recommendation" && xmlStrReader.isEndElement()) {
            // add to has
            LinkedInUser::Recommendation recommendation;
            recommendation.text = recommendationText;
            recommendation.type = recommendationType;
            recommendation.recommender.id = recommenderId;
            recommendation.recommender.firstName = recommenderFirstName;
            recommendation.recommender.lastName = recommenderLastName;

            m_linkedinUser->addRecommendation(id, recommendation);

            // clear data
            id.clear();
            recommendationText.clear();
            recommendationType.clear();
            recommenderId.clear();
            recommenderFirstName.clear();
            recommenderLastName.clear();
        }

        xmlStrReader.readNext();
    }
}


void LinkedIn::parseSkillsXml(QXmlStreamReader &xmlStrReader)
{
    qDebug("[LinkedIn::parseSkillsXml]");

    bool flag = true;
    QString id;
    QString skill;

    xmlStrReader.readNextStartElement();

    while (xmlStrReader.name() != "skills") {
        if (xmlStrReader.isStartElement()) {
            if (xmlStrReader.name() == "id") {
                id = xmlStrReader.readElementText();
            } else if (xmlStrReader.name() == "name") {
                skill = xmlStrReader.readElementText();
            }
        } else {
            if (xmlStrReader.name() == "skill" && !flag) {
                flag = true;

                // add to hash
                m_linkedinUser->addSkill(id, skill);
            } else if (xmlStrReader.name() == "skill" && flag) {
                flag = false;
            }
        }

        xmlStrReader.readNext();
    }
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
    QByteArray rcv = rep->readAll();

    rep->deleteLater();

    qDebug() << "[LinkedIn::profileInfoReceived] xml received: " << rcv;

    // parse
    QXmlStreamReader xmlParser(rcv);

    while (!xmlParser.atEnd()) {
        if (xmlParser.readNextStartElement()) {
            QString startTag = xmlParser.name().toString();
            qDebug() << "start elem: " << startTag;

            if (startTag == "id") {
                m_linkedinUser->setProfileId(xmlParser.readElementText());
            }

            if (startTag == "first-name") {
                m_linkedinUser->setFirstName(xmlParser.readElementText());
            }

            if (startTag == "last-name") {
                m_linkedinUser->setLastName(xmlParser.readElementText());
            }

            if (startTag == "email-address") {
                m_linkedinUser->setEmail(xmlParser.readElementText());
            }

            if (startTag == "num-recommenders") {
                m_linkedinUser->setNumberOfRecommenders(xmlParser.readElementText().toInt());
            }

            if (startTag == "num-connections") {
                m_linkedinUser->setNumberOfConnections(xmlParser.readElementText().toInt());
            }

            if (startTag == "headline") {
                m_linkedinUser->setHeadLine(xmlParser.readElementText());
            }

            if (startTag == "industry") {
                m_linkedinUser->setIndustry(xmlParser.readElementText());
            }

            if (startTag == "picture-url") {
                m_linkedinUser->setProfilePictureUrl(xmlParser.readElementText());
            }

            // languages
            if (startTag == "languages") {
                parseLanguageXml(xmlParser);
            }

            // skills
            if (startTag == "skills") {
                parseSkillsXml(xmlParser);
            }

            // recommendations
            if (startTag == "recommendations-received") {
                parseRecommendationsXml(xmlParser);
            }

            // positions
            if (startTag == "positions") {
                parsePositionsXml(xmlParser);
            }

            // education
            if (startTag == "educations") {
                parseEducationXml(xmlParser);
            }
        }
    }

    Q_EMIT profileUpdated();
    qDebug() << "\n\nUSE IFNO: " << m_linkedinUser->profileId() <<  m_linkedinUser->firstName() << " " << m_linkedinUser->lastName() << " " << m_linkedinUser->headline();
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
    infoStr += "id,first-name,last-name,email-address";

    // full profile info
    infoStr += ",associations,interests,languages,skills,certifications,educations,num-recommenders,recommendations-received";

    // linkedin gen data
    infoStr += ",num-connections,headline,location:(name,country:(code)),industry,summary,picture-url,public-profile-url,positions";

    // close
    infoStr += ")";

    reqUrl.append(infoStr);

    reqUrl.append("?oauth2_access_token=" + authToken());

    QNetworkRequest req(reqUrl);
    QNetworkReply *netRep = m_networkAccessManager->get(req);

    connect(netRep, SIGNAL(finished()), this, SLOT(profileInfoReceived()));
    connect(netRep, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onNetReplyError(QNetworkReply::NetworkError)));
}

}       // Socializer




