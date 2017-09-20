import QtQuick 2.1
import QtQuick.Controls 1.0

Item {
    id: item1
    objectName: "tabItemContainer"
    anchors.fill: parent

    Component {
        id: percentageViewDelegate
        Item {
            Text {
                id: numericViz
                anchors.leftMargin: 5
                anchors.rightMargin: 10
                anchors.topMargin: 2
                anchors.fill: parent
                horizontalAlignment: Text.AlignRight
                text: typeof(styleData.value) == "number" ? styleData.value.toFixed(2) + " %" : styleData.value;

                Rectangle {
                    id: percentageViz
                    width: typeof(styleData.value) == "number" ? (styleData.value / 100) * parent.width  : 0
                    height: parent.height
                    color: "Lightgreen"
                    z: -1
                }
            }
        }

    }

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
            id: coreColumn
            title: "Core"
            width: 100
            resizable: false
            delegate: Text {
                    anchors.leftMargin: 5
                    anchors.topMargin: 2
                    anchors.fill: parent
                    id: rowText
                    text: "Core " +  styleData.row
                }
        }

        TableViewColumn {
            role: "display"
            title: "User Space"
            resizable: false
            width: (tableView.width - coreColumn.width) / 4 - 1
            delegate: percentageViewDelegate
        }

        TableViewColumn {
            role: "decoration"
            title: "Kernel Space"
            resizable: false
            width: (tableView.width - coreColumn.width) / 4 - 1
            delegate: percentageViewDelegate
        }

        TableViewColumn {
            role: "toolTip"
            title: "Other Operations"
            resizable: false
            width: (tableView.width - coreColumn.width) / 4 - 1
            delegate: percentageViewDelegate
        }

        TableViewColumn {
            role: "statusTip"
            title: "Idle Time"
            resizable: false
            width: (tableView.width - coreColumn.width) / 4 - 1
            delegate: percentageViewDelegate
        }

        model: myCustomModel
    }
}
