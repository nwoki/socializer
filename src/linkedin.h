/*
 * linkedin.h
 *
 * This file is part of the Socializer library
 * Copyright (C) 2012 Ispirata <info@ispirata.com>
 *
 * Author Francesco Nwokeka <francesco@ispirata.com>
 */

#ifndef LINKEDIN_H
#define LINKEDIN_H

#include "oauth.h"

#include <QtNetwork/QNetworkReply>

class QDeclarativeView;
class QNetworkAccessManager;

namespace Socializer
{

/**
 * LinkedIn interface
 */
class LinkedIn : public OAuth
{
    Q_OBJECT

public:
    LinkedIn(const QByteArray &appId, const QByteArray &redirectUrl, const QByteArray &consumerSecret, QObject *parent = 0);
    ~LinkedIn();

    Q_INVOKABLE void obtainAuthPageUrl();

    Q_INVOKABLE void parseNewUrl(const QString &url);


    /**
     * Use this to set the correct context property in order to use the
     * Socializer::Facebook object with the included FacebookQML component
     * @param view pointer to the Declarative view used in the application
     */
    void setContextProperty(QDeclarativeView *view);

private Q_SLOTS:
    void prepareAuthPageUrl();
};

};

#endif  // LINKEDIN_H