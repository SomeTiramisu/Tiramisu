import QtQuick 2.15
import QtQuick.Controls 2.15
import components 1.0

Item {
    id: container
    property url bgFilename: ""
    property url bookFilename: ""

    property int pageIndex: 0
    function genId(book_filename: string, index: int, width: int, height: int, controller_id: string, controller_preload: int) {
        //console.log(JSON.stringify({book_filename, index ,width, height}))
        return JSON.stringify({book_filename, index ,width, height, controller_id, controller_preload})
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
        cache: false
        source: "image://pages/" + genId(container.bookFilename, container.pageIndex, container.width, container.height, "0", 10)
    }
    TapHandler {
        id: tHandler
        onTapped: {
            if (eventPoint.position.x > 2*parent.width/3 && container.pageIndex < p.bookSize-1) {
                container.pageIndex++

            } else if (eventPoint.position.x < parent.width/3 && container.pageIndex > 0) {
                container.pageIndex--
            } else {
            }

            page.source = "image://pages/" + genId(container.bookFilename, container.pageIndex, container.width, container.height, "0", 10)
        }
    }
    onBookFilenameChanged: {
        container.pageIndex = 0
        page.source = "image://pages/" + genId(container.bookFilename, container.pageIndex, container.width, container.height, "0", 10)
        p.bookSize = backend.getBookSize(bookFilename)
    }

    /*MiniViewComponent {
        id: minidrawer
        bookFilename: container.bookFilename
        anchors.fill: parent
    }*/

    Timer {
        id: update
        interval: 1000
        onTriggered: page.source = "image://pages/" + genId(container.bookFilename, container.pageIndex, container.width, container.height, "0", 10)
    }

    onWidthChanged: update.start()

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
