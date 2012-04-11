/*
 * FacebookQML.qml
 *
 * This file is part of the Socializer library
 * Copyright (C) 2012 Ispirata <info@ispirata.com>
 *
 * Author Francesco Nwokeka <francesco@ispirata.com>
 */

import QtQuick 1.1
import QtWebKit 1.0
import com.nokia.symbian 1.1

/**
 * Facebook QML component for Symbian
 */

Page {
    WebView {
        id: fbWebView;

        signal tokenChanged();

        preferredHeight: parent.height;
        preferredWidth: parent.width;

        url: Facebook.authPageUrl;

        /**
         * on load finished, send new url to c++ to extract various
         * errors or, if present, the auth token
         */
        onLoadFinished: {
            Facebook.parseNewUrl(fbWebView.url);
        }
    }

    // used to connect to the Facebook component set via ContextProperty to read when the auth token changes
    Connections {
        target: Facebook;
        onAuthTokenChanged: {
            console.log("[FacebookQML] new auth token: " + Facebook.authToken);
            fbWebView.tokenChanged();

            // pop back to previous page
            pageStack.pop();
        }
    }
}
