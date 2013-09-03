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
        Socializer::LinkedIn *link = new Socializer::LinkedIn("AQVaxUFW7YypexPx7tSxWig-NjUHCffYyAEA57VXuF1wePH7y3s6BKRxw0VLN4T_9BVUuR82s09yI27IyFNsEs9mRp7NNqfzJBRZPU_bHEbnd6lZZupVRB7zmFMGsrCkJ20c4gLocSk6qBQVe9fkglwZQDGHKLgrOuMHlRIVEKhOpxuOddk");
        Q_UNUSED(link)
    }

    return app.exec();
}
