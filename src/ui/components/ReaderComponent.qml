import QtQuick 2.15
import QtQuick.Controls 2.15
import components 1.0

Item {
    id: container
    property url bgFilename: ""
    property url bookFilename: ""

    property int pageIndex: 0
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
        source: "image://pages/" + genId(container.bookFilename, container.pageIndex, container.width, container.height, "0", 10, "classic")
    }
    TapHandler {
        id: tHandler
        onTapped: (eventPoint) => {
            if (eventPoint.position.x > 2*parent.width/3 && container.pageIndex < p.bookSize-1) {
                container.pageIndex++
               slider.visible = false

            } else if (eventPoint.position.x < parent.width/3 && container.pageIndex > 0) {
                container.pageIndex--
                slider.visible = false
            } else {
                if (slider.visible) {
                    slider.visible = false
                } else {
                    slider.visible = true
                }
            }
        }
    }
    onBookFilenameChanged: {
        container.pageIndex = 0
        page.source = "image://pages/" + genId(container.bookFilename, container.pageIndex, container.width, container.height, "0", 10, "classic")
        p.bookSize = backend.getBookSize(bookFilename)
    }

    Timer {
        id: update
        interval: 1000
        onTriggered: page.source = "image://pages/" + genId(container.bookFilename, container.pageIndex, container.width, container.height, "0", 10, "classic")
    }

    onWidthChanged: update.restart()
    onPageIndexChanged: page.source = "image://pages/" + genId(container.bookFilename, container.pageIndex, container.width, container.height, "0", 10, "classic")


    Slider {
        id: slider
        visible: false
        anchors.bottom: container.bottom
        height: implicitHeight
        width: container.width
        from: 0
        to: p.bookSize-1
        live: false
        snapMode: Slider.SnapAlways
        stepSize: 1
        value: container.pageIndex
        onValueChanged: {
            container.pageIndex = value
        }
    }
}
