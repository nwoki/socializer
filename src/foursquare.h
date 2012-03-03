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

namespace Socializer
{

/**
* Foursquare interface
*/

class Foursquare
{
public:
    Foursquare();
    Foursquare(OAuth *oauth);
    ~Foursquare();

private:
    OAuth *m_oauth;
};


};

#endif  // FOURSQUARE_H