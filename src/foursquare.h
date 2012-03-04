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

class Foursquare
{
public:
    /// TODO redirectUrl should be a needed param
    Foursquare(const QByteArray &appId, const QByteArray &redirectUrl = QByteArray());
    Foursquare(OAuth *oauth);
    ~Foursquare();

    /** returns url to access for authentication */
    /// TODO if the creation of this url is the same for all social networks, move it to OAuth
    QString obtainAuthPageUrl();

    void setAuthToken(const QByteArray &token);

private:
    OAuth *m_oauth;
};

};

#endif  // FOURSQUARE_H