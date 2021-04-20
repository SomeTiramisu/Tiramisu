import QtQuick 2.15

Item {
    id: container
    property url bgFilename: ""
    property url bookFilename: ""

    property int pageIndex: 0
    function genId(book_filename: string, index: int, width: int, height: int) {
        //console.log(JSON.stringify({book_filename, index ,width, height}))
        return JSON.stringify({book_filename, index ,width, height})
    }
    QtObject { //workaround for private property
        id: p
        property int bookSize: 0
    }

    Image {
        id: background
        source: container.bgFilename
        anchors.fill: parent
        fillMode: Image.Tile
        horizontalAlignment: Image.AlignLeft
        verticalAlignment: Image.AlignRight
    }
    Image {
        id: page
        anchors.fill: parent
        fillMode: Image.Pad
        smooth: false
        source: "image://pages/" + genId(container.bookFilename, container.pageIndex, container.width, container.height)
    }
    TapHandler {
        id: tHandler
        onTapped: {
            if (eventPoint.position.x > parent.width / 2) {
                if (container.pageIndex < p.bookSize-1) {
                    container.pageIndex++
                }
            } else if (container.pageIndex > 0) {
                container.pageIndex--
            }
            page.source = "image://pages/" + genId(container.bookFilename, container.pageIndex, root.width, root.height)
        }
    }
    onBookFilenameChanged: {
        container.pageIndex = 0
        page.source = "image://pages/" + genId(container.bookFilename, container.pageIndex, container.width, container.height)
        p.bookSize = backend.getBookSize(bookFilename)
    }
}
