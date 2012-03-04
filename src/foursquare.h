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

namespace Socializer
{

/**
* Foursquare interface
*/

class Foursquare : public OAuth
{
public:
    /// TODO should redirectUrl be a needed param?
    Foursquare(const QByteArray &appId, const QByteArray &redirectUrl = QByteArray(), QObject *parent = 0);
    ~Foursquare();

    /** returns url to access for authentication */
    QString obtainAuthPageUrl();
};

};

#endif  // FOURSQUARE_H