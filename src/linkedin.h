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

    QString obtainAuthPageUrl();

private:
};

};

#endif  // LINKEDIN_H