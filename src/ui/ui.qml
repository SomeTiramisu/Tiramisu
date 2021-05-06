import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import Qt.labs.folderlistmodel 2.15
import QtQuick.Window 2.15
import components 1.0


Window {
    id: root
    width:900; height: 900
    visible: true
    visibility: chooseVisibility()
    function chooseVisibility() {
        if (backend.getProductName() === "android") {
            return "FullScreen"
        }
        return "Windowed"
    }
    function genId(book_filename: string, index: int, width: int, height: int, controller_id: string, controller_preload: int, runnable_type: string) {
        //console.log(JSON.stringify({book_filename, index ,width, height}))
        return JSON.stringify({book_filename, index ,width, height, controller_id, controller_preload, runnable_type})
    }

    NavigationDrawer {
        id: drawer
        anchors.fill: parent
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

    /*MiniViewComponent {
        id: minidrawer
        bookFilename: reader.bookFilename
        anchors.fill: parent
        onImageSelected: reader.pageIndex = index
    }*/
}

