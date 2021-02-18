import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    id: root
    width:500; height: 500
    //width:1080; height: 1920
    visible: true
    visibility: chooseVisibility()
    function chooseVisibility() {
        if (backend.productName === "android") {
             return "FullScreen"
        }
        return "Windowed"
    }
    Component.onCompleted: {
        //backend.width = width
        //backend.height = height
        page.source = "image://pages/first"
    }

    Image {
        id: page
        anchors.fill: parent
        sourceSize.width: root.width; sourceSize.height: root.height
        fillMode: Image.Pad
        //source: "image://pages/first"
        TapHandler {
            onTapped: {
                if (eventPoint.position.x > root.width / 2) {
                    backend.pageIndex = backend.pageIndex + 1
                } else if (backend.pageIndex > 0) {
                    backend.pageIndex = backend.pageIndex - 1
                }
                page.source = "image://pages/" + backend.pageIndex
            }
        }
    }
}
