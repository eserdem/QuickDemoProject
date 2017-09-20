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
            //width: item1.width - keyColumn.width
        }

        Component.onCompleted:
        {
//            tableView.getColumn(1).resizeToContents();
//            tableView.getColumn(1).size = 5 * tableView.getColumn(1).size;

//            var columnz = tableView.getColumn(1).resizeToContents();

//            for(var child in columnz) {
//                console.log(child.)
//            }

//            var rowCountThis = tableView.getColumn(1);
//            for (var i = 0; i<rowCountThis; ++i)
//            {
//                 var info = tableView.model.data(i)

//                console.log(info)

//                cola.at(1);
//                console.log(cola.at(1).length().toString())
//            }

//            var col = getColumn(1)
//            col.resizeToContents();
        }

        model: myCustomModel
    }
}
