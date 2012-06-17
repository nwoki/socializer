/*
 * facebook.h
 *
 * This file is part of the Socializer library
 * Copyright (C) 2012 Ispirata <info@ispirata.com>
 *
 * Author Francesco Nwokeka <francesco@ispirata.com>
 */

#ifndef FACEBOOK_H
#define FACEBOOK_H

#include "oauth.h"

class QDeclarativeView;

namespace Socializer
{

/**
 * Facebook interface
 */

class Facebook : public OAuth
{
    Q_OBJECT

    Q_PROPERTY (bool scopeEmail             READ scopeEmail             WRITE enableScopeEmail)
    Q_PROPERTY (bool scopePublishActions    READ scopePublishActions    WRITE enableScopePublishActions)
    Q_PROPERTY (bool scopePublishCheckins   READ scopePublishCheckins   WRITE enableScopePublishCheckins)
    Q_PROPERTY (bool scopePublishStream     READ scopePublishStream     WRITE enableScopePublishStream)
    Q_PROPERTY (bool scopeReadStream        READ scopeReadStream        WRITE enableScopeReadStream)
    Q_PROPERTY (bool scopeUserAboutMe       READ scopeUserAboutMe       WRITE enableScopeUserAboutMe)

public:
    Facebook(const QByteArray &appId, const QByteArray &redirectUrl, QObject *parent = 0);
    ~Facebook();

    void enableScopeEmail(bool enable);
    void enableScopePublishActions(bool enable);
    void enableScopePublishCheckins(bool enable);
    void enableScopePublishStream(bool enable);
    void enableScopeReadStream(bool enable);
    void enableScopeUserAboutMe(bool enable);

    Q_INVOKABLE void obtainAuthPageUrl();

    Q_INVOKABLE void parseNewUrl(const QString &url);

    /**
     * Use this to set the correct context property in order to use the
     * Socializer::Facebook object with the included FacebookQML component
     * @param view pointer to the Declarative view used in the application
     */
    void setContextProperty(QDeclarativeView *view);

    bool scopeEmail() const;
    bool scopePublishActions() const;
    bool scopePublishCheckins() const;
    bool scopePublishStream() const;
    bool scopeReadStream() const;
    bool scopeUserAboutMe() const;

private:
    QString createScope();          /** generates the scopes to add to the request url */

    // facebook scopes
    bool m_scopeEmail;
    bool m_scopePublishAcions;      /** Enables your app to perform checkins on behalf of the user.*/
    bool m_scopePublishCheckins;    /** Enables your app to post content, comments, and likes to a user's stream and to the streams of the user's friends.*/
    bool m_scopePublishStream;      /** Allows user to publish stories to their ticker, timeline, and news feed using Built-in Actions, Achievements, Scores, or Custom Actions.*/
    bool m_scopeReadStream;         /** Provides access to all the posts in the user's News Feed and enables your application to perform searches against the user's News Feed */
    bool m_scopeUserAboutMe;        /** Provides info about the user */
};

};

#endif  // FACEBOOK_H