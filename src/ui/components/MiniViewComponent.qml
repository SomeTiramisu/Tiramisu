import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.0

Item {
    id: container
    property url bookFilename: ""
    signal imageSelected(int index)
    QtObject { //workaround for private property
        id: p
        property int bookSize: 0
    }
    onBookFilenameChanged: {
        p.bookSize = backend.getBookSize(bookFilename)
    }
    Drawer {
        id: drawer
        width: 0.33*container.width
        height: container.height
        edge: Qt.RightEdge
        ListView {
            id: listView
            orientation: ListView.Vertical
            anchors.fill: parent
            model: backend.getBookSize(container.bookFilename)
            delegate: Column {
                Image {
                    id: img
                    //mipmap: true
                    width: listView.width
                    fillMode: Image.PreserveAspectFit
                    source: "image://pages/" + genId(container.bookFilename, index, 100, 100, "1", p.bookSize-1, "simple")
                }
                Label {
                    id: txt
                    text: index
                }
                TapHandler {
                    id: thandler
                    gesturePolicy: TapHandler.WithinBounds | TapHandler.ReleaseWithinBounds
                    onSingleTapped: {
                        eventPoint.accepted = true
                        imageSelected(index)
                    }
                }
            }
        }
    }
}
