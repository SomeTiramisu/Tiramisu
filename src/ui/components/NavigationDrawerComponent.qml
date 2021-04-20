import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import Qt.labs.folderlistmodel 2.15

Item {
    id: container
    signal fileSelected(url fn)
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
                    nameFilters: ["*.cbz", "*.cbr"/*, "*.pdf"*/]
                    showDirs: false
                    folder: backend.bookDir
                }
                Component {
                    id: fileDelegate
                    MenuItem {
                        text: fileName
                        width: folderView.width
                        //height: folderView.height/10
                        height: implicitHeight
                        onTriggered: {
                            //console.log(fileUrl)
                            //page.book_filename = fileUrl
                            //page.book_size = backend.getBookSize(page.book_filename)
                            //backend.pageIndex = 0
                            //page.source = "image://pages/" + root.genId(page.book_filename, 0, root.width, root.height)
                            fileSelected(fileUrl)
                        }

                    }
                }

                model: folderModel
                delegate: fileDelegate
            }
        }
    }
}
