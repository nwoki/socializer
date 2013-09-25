/*
 * main.cpp
 *
 * This file is part of the Socializer library
 * Copyright (C) 2012 Ispirata <info@ispirata.com>
 *
 * Author Francesco Nwokeka <francesco@ispirata.com>
 */

#include <Facebook>
#include <../src/facebookuser.h>

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    if (argc == 2) {
        Socializer::Facebook *fb = new Socializer::Facebook(argv[1]);
        Q_UNUSED(fb)
    } else {
        Socializer::Facebook *fb = new Socializer::Facebook("CAAGEQS531rkBANQjiK4aHDMrxZBvAdal3WX1tZAZAgbrt8Kyf0gFvaYAYddwRbhslEpvB8wq6r027NZAHd3ZBkO1DuaVY0wUiQtsqdcOvGBlvwtpZCIr3qwwtOXMugPTPcP9eppgRiVEwQLebnJG21WXz3yfzOfXj7CwbReERLOxk3omSW0YDyGHD2NnPyCOIZD"
                                                            , false);
        fb->facebookUser()->setLastUpdatedTime("2013-09-23T08:09:43+0000");

        fb->update();
    }

    return app.exec();
}
