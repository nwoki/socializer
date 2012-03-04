/*
 * main.cpp
 *
 * This file is part of the Socializer library
 * Copyright (C) 2012 Ispirata <info@ispirata.com>
 *
 * Author Francesco Nwokeka <francesco@ispirata.com>
 */

#include "../include/Foursquare"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // GoWar info
    Socializer::Foursquare *foursquare = new Socializer::Foursquare("UINBCI54PE04OECTALA03CFOO5YEGIOL5Q1EZ24CTXKJX4Z3"
                                                                    , "http://www.playgowar.com");
    qDebug() << foursquare->obtainAuthPageUrl();

    return app.exec();
}