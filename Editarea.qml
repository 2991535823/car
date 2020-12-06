import QtQuick 2.0
import QtQuick.Controls 2.5

FocusScope{
    id:root
    height: Math.max(Math.max(label.height,box.height),40)
    property string prompttext: "Text:"
    property string _text: "wait edit"
    property alias  text: box.text
    property bool editable: true
    signal edited()

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
            font.pointSize: 12
            color: editable?"black":"grey"
            width:parent.width-label.width
            font.family: "Tahoma"
            focus: true
            onAccepted: {
                root.edited()
            }
        }
    }

}

