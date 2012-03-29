import Qt 4.7
import QtWebKit 1.0

/**
 * Facebook QML component
 */

/// TODO set the value Facebook to be default used by lib. Is this possibile?

WebView {
    id: fbWebView;
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

}
