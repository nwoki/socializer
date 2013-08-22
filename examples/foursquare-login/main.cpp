/*
 * main.cpp
 *
 * This file is part of the Socializer library
 * Copyright (C) 2013 Ispirata <info@ispirata.com>
 *
 * Author Francesco Nwokeka <francesco.nwokeka@ispirata.com>
 */

#include <Foursquare>

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    Socializer::Foursquare *fq  =new Socializer::Foursquare("0RLFOCFCQZE3PDXNBG4I3GK4O43OZKI3B4IWFAD3SFUQAEPR");
    Q_UNUSED(fq)
    return app.exec();
}