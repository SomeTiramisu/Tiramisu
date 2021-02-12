import QtQuick 2.15

Item {
    id: root
    width: 480; height: 854
    //width: 1080; height: 1920
    //width: 1200; height: 1920

    Image {
        id: page
        width: root.width; height: root.height
        sourceSize.width: root.width; sourceSize.height: root.height
        fillMode: Image.Pad
        source: "image://pages/" + backend.pageIndex
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
