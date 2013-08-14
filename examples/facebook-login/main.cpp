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
#include "../include/LinkedIn"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

//     // GoWar info
//     Socializer::Foursquare *foursquare = new Socializer::Foursquare("UINBCI54PE04OECTALA03CFOO5YEGIOL5Q1EZ24CTXKJX4Z3"
//                                                                     , "http://www.playgowar.com");
//     qDebug() << foursquare->obtainAuthPageUrl();


    // QML component test
//     QApplication app(argc, argv);
//     QDeclarativeView *view = new QDeclarativeView;

    // FACEBOOK
//     Socializer::Facebook *fb = new Socializer::Facebook("253316734751940"
//                                                       , "http://www.2s2h.com");
//     fb->enableScopeUserInfo(true);           // TODO test this after setting context property
// //     fb->enableScopePublishActions(true);        // TODO test this after setting context property
//     fb->setContextProperty(view);
//     view->setSource(QUrl("qmlcomponents/FacebookQML.qml"));

    // with auth token
    Socializer::Facebook *fb = new Socializer::Facebook("CAACEdEose0cBAIW5kPJCF2r2hWFTOKM91vGqCFzf2IMKg7nCC4zx6OPVQRZCoKqq8IUqkwsx1qlaOGm3TrCaZB1FxoAKCWZAdMgNgniuQslLKdxtZA9pmlSk61gv3tGcZBh2NJdZCAvw8Weh4FZA3yqZC4s6eRQAXAkZD");
    fb->enableScopeUserInfo(true);

    // TWITTER
//     Socializer::Twitter *twitter = new Socializer::Twitter("ihWJWsvkIJrTNmMAssh1ZA"
//                                                          , "http://www.2s2h.com"
//                                                          , "MuQN5yLBWjG5D7CWJ0YCL5O7AfOOhWsDJyq1iSGfb0");
//     twitter->obtainAuthPageUrl();       // TODO test this after setting context property
//     twitter->setContextProperty(view);
//     view->setSource(QUrl("qm/*l*/components/TwitterQML.qml"));

    // LINKEDIN
//     Socializer::LinkedIn *link = new Socializer::LinkedIn("1fx5y8590q6r", "1tTwdCxMuiycme2R", "http://www.2s2h.com");
// 
//     link->setFullProfileScope(true);
//     link->setEmailProfileScope(true);
// 
//     link->obtainAuthPageUrl();          // TODO test this after setting context property
//     link->setContextProperty(view);
//     view->setSource(QUrl("/home/nwoki/GitProjects/Ispirata/R&D/socializer/qmlcomponents/LinkedinQML.qml"));



//     view->setFixedSize(480, 600);
//     view->show();



    return app.exec();
}