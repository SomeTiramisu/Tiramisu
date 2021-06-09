import QtQuick 2.15
import QtQuick.Controls 2.15
import tiramisu 1.0

Item {
    id: container
    property url bgFilename: ""
    property url bookFilename: ""

    onBookFilenameChanged: {
        slider.value = 0
    }

    Image {
        id: background
        source: container.bgFilename
        anchors.fill: parent
        fillMode: Image.Tile
        horizontalAlignment: Image.AlignLeft
        verticalAlignment: Image.AlignRight
    }

    PageView {
        id: page
        anchors.fill: parent
        filename: bookFilename
        index: slider.value
    }

    TapHandler {
        id: tHandler
        onTapped: (eventPoint) => {
                      if (eventPoint.position.x > 2*parent.width/3 && page.index < page.bookSize-1) {
                          slider.value++
                      } else if (eventPoint.position.x < parent.width/3 && page.index > 0) {
                          slider.value--
                      }
                      if (slider.visible && page.index > 0 && page.index < page.bookSize-1) {
                          slider.visible = false
                      } else if ((eventPoint.position.x > parent.width/3 && eventPoint.position.x < 2*parent.width/3) || page.index == 0 || page.index == page.bookSize) {
                          slider.visible = true
                      }
                  }
    }
    Slider {
        id: slider
        visible: false
        anchors.bottom: container.bottom
        height: implicitHeight
        width: container.width
        from: 0
        to: page.bookSize-1
        live: false
        snapMode: Slider.SnapAlways
        stepSize: 1
        Label {
            id: label
            text: parent.valueAt(parent.position)
            style: Text.Outline
            color: "white"
            styleColor: "black"
            anchors.bottom: parent.handle.top
            anchors.horizontalCenter: parent.handle.horizontalCenter
        }
    }
    ProgressBar {
        id: progress
        visible: true
        anchors.bottom: container.bottom
        width: container.width
        height: implicitHeight
        from: 0
        to: page.bookSize
        value: page.preloaderProgress
        onPositionChanged: {
            if (position == 1.0 || position == 0.0) {
                visible = false
            } else {
                visible = true
            }
        }
    }
}
