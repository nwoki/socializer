/*
 * main.cpp
 *
 * This file is part of the Socializer library
 * Copyright (C) 2013 Ispirata <info@ispirata.com>
 *
 * Author Francesco Nwokeka <francesco.nwokeka@ispirata.com>
 */

#include "../include/LinkedIn"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
//     Socializer::LinkedIn *link = new Socializer::LinkedIn("4b05dbf3-36cf-4539-8936-0754854f6303");
    qDebug() << "aaaaaaaa - " << argv[1];

//     Socializer::LinkedIn *link = new Socializer::LinkedIn(argv[1], "1fx5y8590q6r", "1tTwdCxMuiycme2R", "http://www.2s2h.com");
    Socializer::LinkedIn *link = new Socializer::LinkedIn("AQUkEMYehs5HFfh-3A6x9s6vEgD_Oh9I6_DIYm_yClbFF9cbgpLk7jDkRwMa-jwjRgfN3UO_9suywnwF_-yEMgET3MtB8z6pExBf5gFAQunD_rdxIAA9YkQqquVq5XjbPxzZR5ckS3SQd2-FDH0-C7SCIKNObzUKnbifqqpPGZM1juZvhcY");
/*
 * AQUKV6dAQGnBIcP3ZRwo1ZKpVdsfKD-3UzC9UL6Z3T4h_eFHVfeaFZOpdDX7T3oI3bPLzPbj6_L276NpEpKo6QGUHNg9kcv6cUS2lwt__3ciGo-LSyb2BI3celhEMKnV2u80kry7EwdctF8yDxxjP6avi8Sx6r8xBfZHf3OnwcMC0OWJplA
 */
//     link->setFullProfileScope(true);
//     link->setEmailProfileScope(true);

//     link->obtainAuthPageUrl();          // TODO test this after setting context property

// link->requestAuthToken("AQR-K9faN0XAcUoe-s57WR8YmpMNrHG2MRrokmCI0W9THERgvJ0aIXyQRqnHriJDW7pmhhq7ANPQuRAA59Hs6bjLYlNepK7u6c85zfbTNZFYMTzYLEI");

    return app.exec();
}