import QtQuick 2.0
import QtQuick.Controls 2.5

FocusScope{
    height: Math.max(Math.max(label.height,box.height),40)
    property string prompttext: "Text:"
    property var promptmodel: ["one","two","three"]
    property bool bcheak: true
    property var _color: ["#EBEBEB","#ebdedd"]
    property string selectitem: box.currentText
    signal edited(var msg)
    id:root
    Row {
        width: parent.width
        height: Math.max(label.height,box.height)


        Label {
            id: label
            y: 11
            text: prompttext
            font.family: "Tahoma"
            font.pixelSize: 16
        }
        ComboBox{
            id:box
            y: 0
            focus: true
            enabled: bcheak
            model: promptmodel
            width:parent.width-label.width
            font.family: "Tahoma"
            onActivated: {
                root.edited(selectitem);
            }
        }
    }

}

