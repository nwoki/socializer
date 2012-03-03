/*
 * foursquare.cpp
 *
 * This file is part of the Socializer library
 * Copyright (C) 2012 Ispirata <info@ispirata.com>
 *
 * Author Francesco Nwokeka <francesco@ispirata.com>
 */

#include "foursquare.h"

#define AUTH_URL "https://foursquare.com/oauth2/authenticate?"

using namespace Socializer;


Foursquare::Foursquare()
    : m_oauth(new OAuth)
{

}


Foursquare::Foursquare(OAuth *oauth)
    : m_oauth(oauth)
{

}


Foursquare::~Foursquare()
{

}
