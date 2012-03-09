/*
 * oauth.cpp
 *
 * This file is part of the Socializer library
 * Copyright (C) 2012 Ispirata <info@ispirata.com>
 *
 * Author Francesco Nwokeka <francesco@ispirata.com>
 */

#include "oauth.h"


#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <stdio.h>
#include <QtCore/QCryptographicHash>
#include <QtCore/QDateTime>
#include <QDebug>

#define NONCE_SIZE 16

using namespace Socializer;


OAuth::OAuth(const QByteArray &appId, const QByteArray &redirectUrl, QObject *parent)
    : QObject(parent)
    , m_appId(appId)
    , m_redirectUrl(redirectUrl)
{
}


OAuth::~OAuth()
{
}


QByteArray OAuth::appId() const
{
    return m_appId;
}


QByteArray OAuth::authToken() const
{
    return m_authToken;
}


QByteArray OAuth::hmacsha1Encode(const QByteArray &baseStr, QByteArray key)
{
    int blockSize = 64; // HMAC-SHA-1 block size, defined in SHA-1 standard

    if (key.length() > blockSize) { // if key is longer than block size (64), reduce key length with SHA-1 compression
        key = QCryptographicHash::hash(key, QCryptographicHash::Sha1);
    }

    QByteArray innerPadding(blockSize, char(0x36)); // initialize inner padding with char "6"
    QByteArray outerPadding(blockSize, char(0x5c)); // initialize outer padding with char "\"
    // ascii characters 0x36 ("6") and 0x5c ("\") are selected because they have large
    // Hamming distance (http://en.wikipedia.org/wiki/Hamming_distance)

    for (int i = 0; i < key.length(); i++) {
        innerPadding[i] = innerPadding[i] ^ key.at(i); // XOR operation between every byte in key and innerpadding, of key length
        outerPadding[i] = outerPadding[i] ^ key.at(i); // XOR operation between every byte in key and outerpadding, of key length
    }

    // result = hash ( outerPadding CONCAT hash ( innerPadding CONCAT baseString ) ).toBase64
    QByteArray total = outerPadding;
    QByteArray part = innerPadding;

    part.append(baseStr);
    total.append(QCryptographicHash::hash(part, QCryptographicHash::Sha1));

    QByteArray hashed = QCryptographicHash::hash(total, QCryptographicHash::Sha1);

    return hashed.toBase64();
}


QByteArray OAuth::timeStamp()
{
    return QByteArray::number(QDateTime::currentDateTime().toTime_t());
}


QByteArray OAuth::nonce()
{
    QByteArray chars("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
    int max = chars.size();

    QByteArray nonce;

    for(int i = 0; i < NONCE_SIZE; ++i){
        nonce.append(chars[qrand() % max]);
    }

    return nonce;
}


QByteArray OAuth::redirectUrl() const
{
    return m_redirectUrl;
}


void OAuth::setAppId(const QByteArray &appId)
{
    m_appId = appId;
}


void OAuth::setAuthToken(const QByteArray &authToken)
{
    m_authToken = authToken;
}


void OAuth::setRedirectUrl(const QByteArray &redirectUrl)
{
    m_redirectUrl = redirectUrl;
}




