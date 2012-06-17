/*
 * twitter.h
 *
 * This file is part of the Socializer library
 * Copyright (C) 2012 Ispirata <info@ispirata.com>
 *
 * Author Francesco Nwokeka <francesco@ispirata.com>
 */

#ifndef TWITTER_H
#define TWITTER_H

#include "oauth.h"

#include <QtCore/QByteArray>

class QDeclarativeView;

namespace Socializer
{

/**
 * Twitter interface
 */

class Twitter : public OAuth
{
    Q_OBJECT

public:
    Twitter(const QByteArray &appId, const QByteArray &redirectUrl, const QByteArray &consumerSecret, QObject *parent = 0);
    ~Twitter();

    /** obtain twitter auth page for authentication once the request token has been recieved */
    Q_INVOKABLE  void obtainAuthPageUrl();


    /**
     * used to parse urls given by the QML Facebook component. It parses
     * the url to extract various errors and/or the auth token if all goes well
     * @param url the new url
     */
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

#endif  // TWITTER_H