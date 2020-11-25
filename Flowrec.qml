import QtQuick 2.0

Item {
    width: 240
    height: 180
    property var textsize: 14
    property int _radius: 7
    property string _text: "value"
    property string _color: "ffff00"
    Rectangle{
        id: rectangle

        width: parent.width
        height: parent.height

        radius: _radius
        border{
            width: 2
            color: _color
        }

        Rectangle{
            id: rectangle1
            width: _text.length*(escape(_text).indexOf('%u')<0?textsize:textsize*1.5)
            height: textsize
            anchors.bottom: parent.top
            anchors.bottomMargin: -textsize/2
            anchors.left: parent.left
            anchors.leftMargin: _radius*2
            Text {
                text: _text
                font.pointSize: textsize
                font.family: "Tahoma"
                anchors.centerIn: parent
            }
        }

    }
}


/*##^##
Designer {
    D{i:1;anchors_height:20}
}
##^##*/
