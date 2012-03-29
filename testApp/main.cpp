/*
 * main.cpp
 *
 * This file is part of the Socializer library
 * Copyright (C) 2012 Ispirata <info@ispirata.com>
 *
 * Author Francesco Nwokeka <francesco@ispirata.com>
 */

#include "../include/Foursquare"
#include "../include/Facebook"
#include "../include/Twitter"

// #include <QtCore/QCoreApplication>
#include <QtGui/QApplication>
#include <QtCore/QDebug>
#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeContext>

int main(int argc, char *argv[])
{
//     QCoreApplication app(argc, argv);

//     // GoWar info
//     Socializer::Foursquare *foursquare = new Socializer::Foursquare("UINBCI54PE04OECTALA03CFOO5YEGIOL5Q1EZ24CTXKJX4Z3"
//                                                                     , "http://www.playgowar.com");
//     qDebug() << foursquare->obtainAuthPageUrl();
//
//     // Nwoki TestApp
//     Socializer::Facebook *face = new Socializer::Facebook("253316734751940"
//                                                         , "http://www.2s2h.com");
//     face->enableScopePublishActions(true);
//     face->enableScopePublishStream(true);
//     qDebug() << face->obtainAuthPageUrl();


//     // Twitter Nwoki testApp
//     Socializer::Twitter *twitter = new Socializer::Twitter("ihWJWsvkIJrTNmMAssh1ZA"
//                                                         , "http://www.2s2h.com"
//                                                         , "MuQN5yLBWjG5D7CWJ0YCL5O7AfOOhWsDJyq1iSGfb0");
//     twitter->obtainAuthPageUrl();

    // QML component test
    QApplication app(argc, argv);
    Socializer::Facebook *fb = new Socializer::Facebook("253316734751940"
                                                        , "http://www.2s2h.com");
    fb->enableScopeUserAboutMe(true);
    fb->enableScopePublishActions(true);

    QDeclarativeView *view = new QDeclarativeView;

    fb->setContextProperty(view);
    view->setSource(QUrl("/home/z3r0/GitProjects/Ispirata/socializer/qmlcomponents/FacebookQML.qml"));
    view->setFixedSize(480, 600);
    view->show();

    return app.exec();
}