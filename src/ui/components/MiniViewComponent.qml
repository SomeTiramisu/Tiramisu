import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.0

Item {
    id: container
    property url bookFilename: ""
    signal imageSelected(int index)
    function genSimpleId(book_filename: string, index: int) {
        return JSON.stringify({book_filename, index ,width: -1, height: -1, controller_id: "", controller_preload: -1})
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
                    mipmap: true
                    width: listView.width
                    fillMode: Image.PreserveAspectFit
                    //source: "image://simplePages/" + genSimpleId(container.bookFilename, index, 0, 0, "1", 0)
                }
                Label {
                    id: txt
                    text: index
                }
            }
        }
    }
}
