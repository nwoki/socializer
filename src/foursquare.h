/*
 * foursquare.h
 *
 * This file is part of the Socializer library
 * Copyright (C) 2012 Ispirata <info@ispirata.com>
 *
 * Author Francesco Nwokeka <francesco@ispirata.com>
 */

#ifndef FOURSQUARE_H
#define FOURSQUARE_H

#include "oauth.h"

#include <QtCore/QByteArray>

class FoursquareUser;
class QDeclarativeView;

namespace Socializer
{

/**
* Foursquare interface
*/

class Foursquare : public OAuth
{
    Q_OBJECT

public:
    /// TODO should redirectUrl be a needed param?
    Foursquare(const QByteArray &appId, const QByteArray &redirectUrl, QObject *parent = 0);
    Foursquare(const QByteArray &appId, const QByteArray &redirectUrl, const QByteArray &consumerSecret, QObject *parent = 0);
    ~Foursquare();

    /** returns url to access for authentication */
    Q_INVOKABLE void obtainAuthPageUrl();

    Q_INVOKABLE void parseNewUrl(const QString &url);

    /**
     * Use this to set the correct context property in order to use the
     * Socializer::Facebook object with the included FacebookQML component
     * @param view pointer to the Declarative view used in the application
     */
    void setContextProperty(QDeclarativeView *view);

private Q_SLOTS:
    void onAuthTokenChanged();
    void onNetReplyError(QNetworkReply::NetworkError error);
    void onPopulateDataReplyReceived();
    void parseAccessToken();

private:
    QString dateVerifier() const;
    void populateData();                /** retreives user info from the social network */

    FoursquareUser *m_fqUser;           /** the account of the user authed with fq */
    QNetworkReply *m_networkReply;
};

};

#endif  // FOURSQUARE_H