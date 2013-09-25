/*
 * main.cpp
 *
 * This file is part of the Socializer library
 * Copyright (C) 2012 Ispirata <info@ispirata.com>
 *
 * Author Francesco Nwokeka <francesco@ispirata.com>
 */

#include <Facebook>

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    if (argc == 2) {
        Socializer::Facebook *fb = new Socializer::Facebook(argv[1]);
        Q_UNUSED(fb)
    } else {
        Socializer::Facebook *fb = new Socializer::Facebook("CAAGEQS531rkBAOijB9Fx0HwpSyR8LgTuUBLSHm0G2fVkY8X8jBGaOfFedwKn89CmkjzoKODmgF0aSNovCokyPJCZCMKHi5BKZAhGb884jRtYLdqbQ3lNvLsRdXR4nubTlzQqkg8Dji4ZAFlwVz5s7RTqJWGzW7RwIM9NlphZADHBU1TRL7Eijm4zr3uczH0ZD");
        Q_UNUSED(fb)
    }

    return app.exec();
}
