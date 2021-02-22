import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import Qt.labs.folderlistmodel 2.15

ApplicationWindow {
    id: root
    width:500; height: 500
    //width:1080; height: 1920
    visible: true
    visibility: chooseVisibility()
    function chooseVisibility() {
        if (backend.productName === "android") {
             return "FullScreen"
        }
        return "Windowed"
    }
    Component.onCompleted: {
        //backend.width = 500
        //backend.height = 500
        page.source = "image://pages/first"
    }
    ListView {
        id: folderView
        anchors.fill: parent
        //visible: false
        FolderListModel {
            id: folderModel
            nameFilters: ["*.cbz", "*.cbr"]
            showDirs: false
            //folder: "/home/guillaume/reader/"
            folder: "/storage/emulated/0/"
        }
        Component {
            id: fileDelegate
            Button {
                text: fileName
                width: folderView.width
                height: folderView.height/10
                onClicked: {
                    console.log(fileUrl)
                    backend.bookFilename = fileUrl
                    page.source = "image://pages/new"
                    page.visible = true
                }

            }
        }
        model: folderModel
        delegate: fileDelegate
    }

        Image {
            id: page
            visible: false
            sourceSize.width: root.width; sourceSize.height: root.height
            anchors.fill: parent
            TapHandler {
                onTapped: {
                    if (eventPoint.position.x > root.width / 2) {
                        backend.pageIndex = backend.pageIndex + 1
                    } else if (backend.pageIndex > 0) {
                        backend.pageIndex = backend.pageIndex - 1
                    }
                    page.source = "image://pages/" + backend.pageIndex
                }
            }
        }

}
