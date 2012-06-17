/*
 * TwitterQML.qml
 *
 * This file is part of the Socializer library
 * Copyright (C) 2012 Ispirata <info@ispirata.com>
 *
 * Author Francesco Nwokeka <francesco@ispirata.com>
 */

import Qt 4.7
import QtWebKit 1.0

/**
 * Twitter QML component
 */

WebView {
    id: twitterWebView;

    signal tokenChanged();

    anchors.fill: parent;
    preferredHeight: 600;
    preferredWidth: 480;

    /**
     * on load finished, send new url to c++ to extract various
     * errors or, if present, the auth token
     */
    onLoadFinished: {
        Twitter.parseNewUrl(twitterWebView.url);
    }

    Component.onCompleted: {
        if (!Twitter.authToken == "") {
            console.log("TOKEN NULL");
            Twitter.obtainAuthPageUrl();
        } else {
            console.log("not null " + Twitter.authToken);
        }
    }

    // used to connect to the Twitter component set via ContextProperty to read when the auth token changes
    Connections {
        target: Twitter;
        onAuthTokenChanged: {
            console.log("[FacebookQML] new auth token: " + Twitter.authToken);
            twitterWebView.tokenChanged();
        }

        onAuthPageUrlReady: {
            console.log("[TwitterQML] auth page url is: " + authPageUrl);
            twitterWebView.url = authPageUrl;
        }
    }
}
