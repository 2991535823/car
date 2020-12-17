import QtQuick 2.0
import QtQuick.Controls 2.13

Rectangle {
    id:root
    signal btnclick(bool val)
    property var content: ["进入系统","退出系统"]
    Image {
        id: image
        width: root.width*0.6
        height: image.width/16*9
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        source: "img/xndx.jpg"
        fillMode: Image.PreserveAspectFit
    }

    Label {
        id: label
        y: image.height+image.y+20
        width: root.width
        text: qsTr("自动导航人机交互系统")
        font.bold: false
        font.pointSize: 36
        font.family: "Tahoma"
        horizontalAlignment: Text.AlignHCenter
    }

    Button {
        id: start
        x: 339
        text: content[0]
        anchors.topMargin: 30
        anchors.top: label.bottom
        anchors.right: out.left
        anchors.rightMargin: 60

        contentItem: Text {
            text: start.text
            font: start.font
            opacity: enabled ? 1.0 : 0.3
            color: "black"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

        background: Rectangle {
            implicitWidth: 100
            implicitHeight: 40
            opacity: enabled ? 1 : 0.3
            border.color: "#A6A6A6"
            border.width: 1
            radius: 2
        }
        onClicked: root.btnclick(true)
    }

    Button {
        id: out
        x: 483
        text: content[1]
        anchors.top: start.top
        anchors.right: parent.right
        anchors.rightMargin: 60
        contentItem: Text {
            text: out.text
            font: out.font
            opacity: enabled ? 1.0 : 0.3
            color: "black"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

        background: Rectangle {
            implicitWidth: 100
            implicitHeight: 40
            opacity: enabled ? 1 : 0.3
            border.color: "#A6A6A6"
            border.width: 1
            radius: 2
        }
        onClicked: root.btnclick(false)
    }

}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:3;anchors_y:326}D{i:6;anchors_y:325}
}
##^##*/
