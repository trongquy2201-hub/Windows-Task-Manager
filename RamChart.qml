import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle{
    id: root
    width: 300
    height: 120
    color: "#2d2d2d"
    radius: 8

    // 1. Tạo biến chứa màu linh hoạt (mặc định cho màu xanh)
    property color statusColor: "#00FF7F"
    property int ramUsage: 0

    // 2. Dùng biến statusColor này cho viền và màu chữ % RAM
    border.color: root.statusColor
    border.width: 1
    Column {
        anchors.centerIn: parent
        spacing: 8

        Text {
            text: "QML RAM MONITOR"
            color: "#888888"
            font.pixelSize: 12
            font.bold: true
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Text{
            text: root.ramUsage + "%"
            color: statusColor
            font.pixelSize: 32
            font.bold: true
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}

