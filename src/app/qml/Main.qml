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
        if (backend.getProductName() === "android") {
            return "FullScreen"
        }
        return "Windowed"
    }
    function genId(filename: string, index: int, width: int, height: int, schedulerId: string, schedulerPreload: int, runnableType: string) {
        //console.log(JSON.stringify({book_filename, index ,width, height}))
        return JSON.stringify({filename, index ,width, height, schedulerId, schedulerPreload, runnableType})
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
        bgFilename: ""
    }

    /*MiniViewComponent {
        id: minidrawer
        bookFilename: reader.bookFilename
        anchors.fill: parent
        onImageSelected: reader.pageIndex = index
    }*/
}

