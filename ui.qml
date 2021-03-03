import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import Qt.labs.folderlistmodel 2.15
import QtQuick.Window 2.15

ApplicationWindow {
    id: root
    width:500; height: 500
    //width:1080; height: 1920
    visible: true
    visibility: chooseVisibility()
    property url book_filename: ""
    function chooseVisibility() {
        if (backend.productName === "android") {
            return "FullScreen"
        }
        return "Windowed"
    }
    function genId(book_filename: string, index: int, width: int, height: int) {
        //console.log(JSON.stringify({book_filename, index ,width, height}))
        return JSON.stringify({book_filename, index ,width, height})
    }
    Drawer {
        id: drawer
        width: 0.66*root.width
        height: root.height

        ColumnLayout {
            anchors.fill: parent
            ListView {
                id: folderView
                Layout.fillWidth: true
                Layout.fillHeight: true

                FolderListModel {
                    id: folderModel
                    nameFilters: ["*.cbz", "*.cbr", "*.pdf"]
                    showDirs: false
                    folder: backend.bookDir
                }
                Component {
                    id: fileDelegate
                    Button {
                        text: fileName
                        width: folderView.width
                        height: folderView.height/10
                        onClicked: {
                            console.log(fileUrl)
                            root.book_filename = fileUrl
                            page.source = "image://pages/" + root.genId(root.book_filename, 0, root.width, root.height)
                        }

                    }
                }

                model: folderModel
                delegate: fileDelegate
            }/*
            Button {
                id: settings
                Layout.fillWidth: true
                Layout.preferredHeight: folderView.height/10
                text: "Settings"
            }*/
        }
    }
    Image {
        id: background
        source: backend.bgFilename
        anchors.fill: parent
        fillMode: Image.Tile
        horizontalAlignment: Image.AlignLeft
        verticalAlignment: Image.AlignRight
    }
    Image {
        id: page
        anchors.fill: parent
        fillMode: Image.Pad
        smooth: false
        source: "image://pages/" + root.genId(root.book_filename, 0, root.width, root.height)
        TapHandler {
            id: tHandler
            onTapped: {
                if (eventPoint.position.x > parent.width / 2) {
                    backend.pageIndex++
                } else if (backend.pageIndex > 0) {
                    backend.pageIndex--
                }
                parent.source = "image://pages/" + genId(root.book_filename, backend.pageIndex, root.width, root.height)
            }
        }
    }
}

