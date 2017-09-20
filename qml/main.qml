import QtQuick 2.1
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.5

Window {
    visible: true
    width: 660
    height: 480
    title: qsTr("CPU Information Visualizer")

    TabView {
        objectName: "tabView"
        id: tabView
        x: 20
        y: 90
        anchors.right: parent.right
        anchors.rightMargin: 15
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 15
        anchors.top: parent.top
        anchors.topMargin: 90
        anchors.left: parent.left
        anchors.leftMargin: 15

    }

    Text {
        id: text1
        x: 80
        y: 26
        width: 340
        height: 60
        text: qsTr("CPU Information Visualizer")
        font.bold: true
        anchors.top: parent.top
        anchors.topMargin: 26
        anchors.left: parent.left
        anchors.leftMargin: 80
        font.family: "Verdana"
        font.pixelSize: 20
    }

    Image {
        id: image2
        x: 20
        y: 24
        width: 42
        height: 44
        transformOrigin: Item.Center
        anchors.top: parent.top
        anchors.topMargin: 24
        anchors.left: parent.left
        anchors.leftMargin: 20
        fillMode: Image.PreserveAspectFit
        antialiasing: true
        z: 0
        visible: true
        clip: true
        sourceSize.height: 200
        sourceSize.width: 200
        source: "unnamed.png"
    }

    Text {
        id: text2
        x: 81
        y: 55
        width: 340
        height: 20
        text: qsTr("Linux OS CPU Information Visualization")
        font.pixelSize: 12
    }
}
