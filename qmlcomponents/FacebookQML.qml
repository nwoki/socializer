/*
 * FacebookQML.qml
 *
 * This file is part of the Socializer library
 * Copyright (C) 2012 Ispirata <info@ispirata.com>
 *
 * Author Francesco Nwokeka <francesco@ispirata.com>
 */

import Qt 4.7
import QtWebKit 1.0

/**
 * Facebook QML component
 */

WebView {
    id: fbWebView;

    signal tokenChanged();

    anchors.fill: parent;
    preferredHeight: 600;
    preferredWidth: 480;

    url: Facebook.authPageUrl;

    /**
     * on load finished, send new url to c++ to extract various
     * errors or, if present, the auth token
     */
    onLoadFinished: {
        Facebook.parseNewUrl(fbWebView.url);
    }

    // used to connect to the Facebook component set via ContextProperty to read when the auth token changes
    Connections {
        target: Facebook;
        onAuthTokenChanged: {
            console.log("[FacebookQML] new auth token: " + Facebook.authToken);
            fbWebView.tokenChanged();
        }
    }
}
