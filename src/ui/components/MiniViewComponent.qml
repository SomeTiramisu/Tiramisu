import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: container
    property url bookFilename: ""
    signal imageSelected(int index)

    ListView {
        orientation: ListView.Horizontal
        anchors.fill: parent
        model: 10
        delegate: Image {
            fillMode: Image.Pad
            smooth: false
            source: "image://pages/" + genId(container.bookFilename, index, 100, 100, "1")
        }
    }
}
