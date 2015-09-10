/*
 * FacebookQML.qml
 *
 * This file is part of the Socializer library
 *
 * Author Francesco Nwokeka <francesco.nwokeka@gmail.com>
 */

import QtQuick 1.1
import QtWebKit 1.0

/**
 * Facebook QML component
 */
Item {
    id: root;

    signal tokenChanged();

    // set these if screen size is different!
    property int screenHeight: 600;
    property int screenWidth: 480;

WebView {
    id: fbWebView;

//     signal tokenChanged();

//     anchors.fill: parent;
    preferredHeight: root.screenHeight;
    preferredWidth: root.screenWidth;

    /**
     * on load finished, send new url to c++ to extract various
     * errors or, if present, the auth token
     */
    onLoadFinished: {
        Facebook.parseNewUrl(fbWebView.url);
    }

    Component.onCompleted: {
        if (!Facebook.authToken == "") {
            console.log("TOKEN NULL");
            Facebook.obtainAuthPageUrl();
        } else {
            console.log("not null " + Facebook.authToken);
        }
    }

    // used to connect to the Facebook component set via ContextProperty to read when the auth token changes
    Connections {
        target: Facebook;
        onAuthTokenChanged: {
            console.log("[FacebookQML] new auth token: " + Facebook.authToken);
            root.tokenChanged();
        }

        onAuthPageUrlReady: {
            console.log("[FacebookQML] auth page url is: " + authPageUrl);
            fbWebView.url = authPageUrl;
        }
    }
}
}
