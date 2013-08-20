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

    if (argc == 2) {
        // use auth CODE
        Socializer::LinkedIn *link = new Socializer::LinkedIn(argv[1], "1fx5y8590q6r", "1tTwdCxMuiycme2R", "http://www.2s2h.com");
    } else {
        Socializer::LinkedIn *link = new Socializer::LinkedIn("AQUkEMYehs5HFfh-3A6x9s6vEgD_Oh9I6_DIYm_yClbFF9cbgpLk7jDkRwMa-jwjRgfN3UO_9suywnwF_-yEMgET3MtB8z6pExBf5gFAQunD_rdxIAA9YkQqquVq5XjbPxzZR5ckS3SQd2-FDH0-C7SCIKNObzUKnbifqqpPGZM1juZvhcY");
    }


    return app.exec();
}