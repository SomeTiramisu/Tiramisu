import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import Qt.labs.folderlistmodel 2.15

Item {
    id: container
    signal fileSelected(url fn)
    Drawer {
        id: drawer
        width: 0.33*container.width
        height: container.height
        ListView {
            id: folderView
            anchors.fill: parent
            FolderListModel {
                id: folderModel
                nameFilters: ["*.cbz", "*.cbr"/*, "*.pdf"*/]
                showDirs: true
                showDotAndDotDot: true
                folder: backend.bookDir
            }
            Component {
                id: fileDelegate
                MenuItem {
                    width: folderView.width
                    text: fileName
                    onTriggered: {
                        if (fileIsDir) {
                            folderModel.folder = fileUrl
                        } else {

                            fileSelected(fileUrl)
                        }
                    }
                }
            }
            model: folderModel
            delegate: fileDelegate
        }
    }
}

