import QtQuick 2.1
import QtQuick.Controls 1.0

Item {
    id: item1
    objectName: "tabItemContainer"
    anchors.fill: parent

    TableView {
        id: tableView
        horizontalScrollBarPolicy: 0
        objectName: "CoreContentsTableView"
        anchors.rightMargin: 1
        anchors.leftMargin: 1
        anchors.bottomMargin: 1
        anchors.topMargin: 1
        highlightOnFocus: true
        currentRow: 0
        sortIndicatorVisible: true
        anchors.fill: parent

        TableViewColumn {
            id: keyColumn
            role: "display"
            title: "Key"
            width: 150
            resizable: false
        }

        TableViewColumn {
            role: "decoration"
            title: "Value"
            width: item1.width - keyColumn.width
        }

        model: myCustomModel
    }
}
