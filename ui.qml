import QtQuick 2.15

Item {
    id: root
    //width:500; height: 500
    width:1080; height: 1920
    onWidthChanged: {backend.height = height}
    onHeightChanged: {backend.width = width}
    Image {
        id: page
        anchors.fill: parent
        sourceSize.width: root.width; sourceSize.height: root.height
        fillMode: Image.Pad
        source: "image://pages/first"
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
