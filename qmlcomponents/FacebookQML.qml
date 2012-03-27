import Qt 4.7
import QtWebKit 1.0

WebView {
    id: fbWebView;
    anchors.fill: parent;
    preferredHeight: 600;
    preferredWidth: 480;

    url: Facebook.authPageUrl;

    Component.onCompleted: {
        console.log("ASDASD: " + Facebook.authPageUrl);
    }

}
