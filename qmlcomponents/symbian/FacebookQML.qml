import QtQuick 1.1
import QtWebKit 1.0
import com.nokia.symbian 1.1

/**
 * Facebook QML component
 */

/// TODO set the value Facebook to be default used by lib. Is this possibile?
Page {
    WebView {
        id: fbWebView;
//        anchors.fill: parent;
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
}
