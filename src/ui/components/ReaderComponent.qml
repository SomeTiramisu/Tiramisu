import QtQuick 2.15
import QtQuick.Controls 2.15
import components 1.0

Item {
    id: container
    property url bgFilename: ""
    property url bookFilename: ""

    property int pageIndex: 0
    function genId(book_filename: string, index: int, width: int, height: int, controller_id: string) {
        //console.log(JSON.stringify({book_filename, index ,width, height}))
        return JSON.stringify({book_filename, index ,width, height, controller_id})
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
        source: "image://pages/" + genId(container.bookFilename, container.pageIndex, container.width, container.height, "0")
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
            page.source = "image://pages/" + genId(container.bookFilename, container.pageIndex, root.width, root.height, "0")
        }
    }
    onBookFilenameChanged: {
        container.pageIndex = 0
        page.source = "image://pages/" + genId(container.bookFilename, container.pageIndex, container.width, container.height, "0")
        p.bookSize = backend.getBookSize(bookFilename)
    }

    MiniViewComponent {
        bookFilename: container.bookFilename
        anchors.bottom: container.bottom
        height: 100
        width: container.width
    }

/*
    Slider {
        anchors.bottom: container.bottom
        height: implicitHeight
        width: container.width
        from: 0
        to: p.bookSize-1
        snapMode: Slider.SnapAlways
        stepSize: 1
        value: container.pageIndex
        onMoved: {
            container.pageIndex = value
            page.source = "image://pages/" + genId(container.bookFilename, container.pageIndex, 100, 100, "1")
        }
    }*/
}
