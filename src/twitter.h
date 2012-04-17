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

class QNetworkAccessManager;
class QNetworkReply;

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

private Q_SLOTS:
    void prepareAuthPageUrl();
};

};

#endif  // TWITTER_H