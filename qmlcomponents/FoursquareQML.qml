/*
 * FoursquareQML.qml
 *
 * This file is part of the Socializer library
 *
 * Author Francesco Nwokeka <francesco.nwokeka@gmail.com>
 */

import QtQuick 1.1
import QtWebKit 1.0

/**
 * Foursquare QML component
 */
Item {
    id: root;

    signal tokenChanged();

    // set these if screen size is different!
    property int screenHeight: 600;
    property int screenWidth: 480;

WebView {
    id: fqWebView;

//     signal tokenChanged();

//     anchors.fill: parent;
    preferredHeight: root.screenHeight;
    preferredWidth: root.screenWidth;

    /**
     * on load finished, send new url to c++ to extract various
     * errors or, if present, the auth token
     */
    onLoadFinished: {
        console.log("NEW URL : " + fqWebView.url);
        Foursquare.parseNewUrl(fqWebView.url);
    }

    Component.onCompleted: {
        if (!Foursquare.authToken == "") {
            console.log("TOKEN NULL");
            Foursquare.obtainAuthPageUrl();
        } else {
            console.log("not null " + Foursquare.authToken);
        }
    }

    // used to connect to the Foursquare component set via ContextProperty to read when the auth token changes
    Connections {
        target: Foursquare;
        onAuthTokenChanged: {
            console.log("[FoursquareQML] new auth token: " + Foursquare.authToken);
            root.tokenChanged();
        }

        onAuthPageUrlReady: {
            console.log("[FoursquareQML] auth page url is: " + authPageUrl);
            fqWebView.url = authPageUrl;
        }
    }
}
}
