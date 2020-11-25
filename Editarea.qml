import QtQuick 2.0
import QtQuick.Controls 2.5

FocusScope{
    height: Math.max(Math.max(label.height,box.height),40)
    property string prompttext: "Text:"
    property string _text: "wait edit"
    property bool editable: true
    id:root
    Row {
        id: row
        width: parent.width
        height: Math.max(label.height,box.height)


        Label {
            id: label
            text: prompttext
            anchors.verticalCenter: parent.verticalCenter
            font.family: "Tahoma"
            font.pixelSize: 16
        }
        TextInput{
            id:box
            enabled: editable
            text:_text
            anchors.verticalCenter: parent.verticalCenter
            font.pointSize: 16
            width:parent.width-label.width
            font.family: "Tahoma"
            focus: true

        }
    }

}

