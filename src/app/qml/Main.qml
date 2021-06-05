import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import Qt.labs.folderlistmodel 2.15
import QtQuick.Window 2.15


Window {
    id: root
    width:900; height: 900
    visible: true
    visibility: chooseVisibility()
    function chooseVisibility() {
        if (productName == "android") {
            return "FullScreen"
        }
        return "Windowed"
    }

    NavigationDrawer {
        id: drawer
        anchors.fill: parent
        onFileSelected: (fn) => {
            reader.bookFilename = fn
        }
    }
    ReaderView {
        id: reader
        visible: true
        anchors.fill: parent
        bgFilename: "qrc:/qml/background.png"
    }

    /*MiniViewComponent {
        id: minidrawer
        bookFilename: reader.bookFilename
        anchors.fill: parent
        onImageSelected: reader.pageIndex = index
    }*/
}

