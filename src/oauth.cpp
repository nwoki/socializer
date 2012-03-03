/*
 * oauth.cpp
 *
 * This file is part of the Socializer library
 * Copyright (C) 2012 Ispirata <info@ispirata.com>
 *
 * Author Francesco Nwokeka <francesco@ispirata.com>
 */

#include "oauth.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QtCore/QSettings>

using namespace Socializer;

OAuth::OAuth()
    : m_accessManager(0)
{
}


OAuth::~OAuth()
{
}


