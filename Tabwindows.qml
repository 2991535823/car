import QtQuick 2.0

Item {
    id: tabWidget
    default property alias content: stack.children

    property int current: 0

    onCurrentChanged: setOpacities()
    Component.onCompleted: setOpacities()

    function setOpacities() {
        for (var i = 0; i < stack.children.length; ++i) {
            stack.children[i].visible  = (i === current ? true : false)
        }
    }

    Row {
        id: header

        Repeater {
            model: stack.children.length
            delegate: Rectangle {
                width: tabWidget.width / stack.children.length/2; height: 36
                BorderImage {
                    anchors { fill: parent; leftMargin: 2; topMargin: 5; rightMargin: 1 }
                    border { left: 7; right: 7 }
                    source: "img/tab.png"
                    visible: tabWidget.current == index
                }
                Text {
                    horizontalAlignment: Qt.AlignHCenter; verticalAlignment: Qt.AlignVCenter
                    anchors.fill: parent
                    text: stack.children[index].title
                    elide: Text.ElideRight
                    font.bold: tabWidget.current == index
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: tabWidget.current = index
                }
            }
        }
    }

    Item {
        id: stack
        width: tabWidget.width
        anchors.top: header.bottom; anchors.bottom: tabWidget.bottom
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
