/*
 * main.cpp
 *
 * This file is part of the Socializer library
 *
 *
 * Author Francesco Nwokeka <francesco.nwokeka@gmail.com>
 */

#include <Foursquare>

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    if (argc == 2) {
        Socializer::Foursquare *fq = new Socializer::Foursquare(argv[1]);
        Q_UNUSED(fq);
    } else {
        Socializer::Foursquare *fq = new Socializer::Foursquare("T53ZTLQSG1CVGKHYRA3OPEUVDGQ5ICFXMMHB01XNEESMSJQP"
                                                            , "http://www.testwebsite.com"
                                                            , "QOMLRRLTNM3L0CRLNGMBM1MZTCUFNKUFIFOKAU2QZBS3RPPR");
        Q_UNUSED(fq);
    }

    return app.exec();
}
