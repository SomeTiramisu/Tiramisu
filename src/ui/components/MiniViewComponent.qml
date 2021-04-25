import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.0

Item {
    id: container
    property url bookFilename: ""
    signal imageSelected(int index)
    Drawer {
        id: drawer
        width: 0.33*container.width
        height: container.height
        edge: Qt.RightEdge
        ListView {
            orientation: ListView.Vertical
            anchors.fill: parent
            model: backend.getBookSize(container.bookFilename)
            delegate: Column {
                //anchors.fill: parent
                Image {
                    id: img
                    fillMode: Image.Pad
                    smooth: false
                    source: "image://pages/" + genId(container.bookFilename, index, 100, 100, "1", -1)
                }
                Label {
                    id: txt
                    text: index
                }
            }
        }
    }
}
