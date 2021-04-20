import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import Qt.labs.folderlistmodel 2.15
import QtQuick.Window 2.15
import components 1.0


ApplicationWindow {
    id: root
    width:900; height: 900
    //width:1080; height: 1920
    visible: true
    visibility: chooseVisibility()
    function chooseVisibility() {
        if (backend.getProductName() === "android") {
            return "FullScreen"
        }
        return "Windowed"
    }

    NavigationDrawer {
        id: drawer
        onFileSelected: {
            reader.bookFilename = fn
        }
    }
    Reader {
        id: reader
        visible: true
        anchors.fill: parent
        bgFilename: backend.bgFilename
    }
}

