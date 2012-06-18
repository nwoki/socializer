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
 * Linkedin QML component
 */

WebView {
    id: linkWebView;

    signal tokenChanged();

    anchors.fill: parent;
    preferredHeight: 600;
    preferredWidth: 480;

    /**
     * on load finished, send new url to c++ to extract various
     * errors or, if present, the auth token
     */
    onLoadFinished: {
        LinkedIn.parseNewUrl(linkWebView.url);
    }

    Component.onCompleted: {
        if (!LinkedIn.authToken == "") {
            console.log("TOKEN NULL");
            LinkedIn.obtainAuthPageUrl();
        } else {
            console.log("not null " + LinkedIn.authToken);
        }
    }

    // used to connect to the LinkedIn component set via ContextProperty to read when the auth token changes
    Connections {
        target: LinkedIn;
        onAuthTokenChanged: {
            console.log("[LinkedinQML] new auth token: " + LinkedIn.authToken);
            linkWebView.tokenChanged();
        }

        onAuthPageUrlReady: {
            console.log("[LinkedinQML] auth page url is: " + authPageUrl);
            linkWebView.url = authPageUrl;
        }
    }
}
