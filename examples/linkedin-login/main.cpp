/*
 * main.cpp
 *
 * This file is part of the Socializer library
 * Copyright (C) 2013 Ispirata <info@ispirata.com>
 *
 * Author Francesco Nwokeka <francesco.nwokeka@ispirata.com>
 */

#include <LinkedIn>

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    if (argc == 2) {
        // use auth CODE
        Socializer::LinkedIn *link = new Socializer::LinkedIn(argv[1], "1fx5y8590q6r", "1tTwdCxMuiycme2R", "http://www.2s2h.com");
        Q_UNUSED(link)
    } else {
        Socializer::LinkedIn *link = new Socializer::LinkedIn("AQUs_qmfAzNjDJfUFTuUKWCcNeYoHq2GIhENwkaUwLYLVHW0Iv8XzT1hEO_dftDSJDrstpVAEGZM2TipLwkY932fr1HuGmgXJF_W7w0QwWKMW2LPX8olotsYYg8YB3g60bkO3fwVqeUmz0V3ks64aarkNBG9gvc1uNEhheNOqc6kz-tmEqE");
        Q_UNUSED(link)
    }

    return app.exec();
}
