import QtQuick 2.0
import QtQuick.Controls 2.13
import Qt.labs.platform 1.1
Rectangle{
    id: root
    property int start: 0 /*file.Start*/
    property int stop: 1 /*file.Stop*/
    property int done: 2 /*file.Done*/
    property int deletemap: 3
    Row {
        id: row
        width: root.width
        height: root.height

        Rectangle {
            id: mclayout
            width: row.width*0.3
            height: row.height
            color: "#ffffff"

            Flowrec {
                id: mcparm
                width: mclayout.width*0.96
                height: mclayout.height*0.85
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: mcparm.textsize
                anchors.top: parent.top
                _text:"地图建立"
                Column{
                    id: column
                    width: parent.width*0.9
                    height: parent.height*0.8
                    spacing: 5

                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.topMargin: mcparm.textsize
                    anchors.top: parent.top

                    Editarea {
                        id: mcname
                        width: parent.width
                        prompttext: "地 图 名 称 :"
                        KeyNavigation.tab:mcmode

                    }

                    Selectbox {
                        id: mcmode
                        width: parent.width
                        prompttext: "采 集 模 式 :"
                        promptmodel: ["单点","连续"]
                    }

                    Selectbox {
                        id: mctime
                        width: parent.width
                        prompttext: "采集时间(s):"
                        promptmodel: [0.2,0.5,1,2,4,8]
                    }

                    Editarea {
                        id: mccount
                        width: parent.width
                        prompttext: "采集点个数:"
                        editable:false
                    }

                    Item {
                        id: capturelayout
                        width: parent.width
                        height: 40

                        Button {
                            id: mccapon
                            width: capturelayout.width*0.3
                            text: qsTr("采集")
                            anchors.left: parent.left
                            anchors.leftMargin: 0
                        }

                        Button {
                            id: mccapoff
                            width: capturelayout.width*0.6
                            text: qsTr("停止采集")
                            anchors.right: parent.right
                        }
                    }
                }

                Button {
                    id: button
                    x: 9
                    y: 245
                    width: column .width
                    text: qsTr("保存地图")
                    anchors.bottomMargin: 50
                    anchors.bottom: parent.bottom
                }
            }

        }

        Rectangle {
            id: mvelayout
            width: row.width-mclayout.width
            height: row.height
            color: "#ffffff"

            Flowrec {
                id: mve
                width: mvelayout.width*0.95
                height: mvelayout.height*0.85
                anchors.topMargin: mve.textsize
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                _text:"地图显示与编辑"

                Column {
                    id: column1
                    width: parent.width*0.95
                    height: parent.height*0.9
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter

                    Row {
                        id: row1
                        width: parent.width
                        height: parent.height*0.15
                        spacing: width-mcselect.width-mcviewbtn.width

                        Selectbox {
                            id: mcselect
                            width: parent.width*0.6
                            height: parent.height
                            prompttext:"地图选择:"
                            promptmodel: file.maplist
                        }

                        Button {
                            id: mcviewbtn
                            width: parent.width*0.3
                            text: qsTr("显示")
                        }
                    }
                }
            }

            Row {
                id: mvebtngroup
                width: parent.width*0.95
                height: parent.height*0.1
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: parent.height*0.05-20
                anchors.top: mve.bottom
                spacing: width*0.1/2
                Button {
                    id: button1
                    width: parent.width*0.3
                    text: qsTr("编辑")
                }

                Button {
                    id: button2
                    width: parent.width*0.3
                    text: qsTr("删除")

                }

                Button {
                    id: button3
                    width: parent.width*0.3
                    text: qsTr("保存")
                    KeyNavigation.tab: mcname
                }

            }
        }


    }

    Connections {
        target: mccapon
        onClicked: {
            file.doCmd(start)
        }
    }
    Connections {
        target: mcname
        onEdited:{
            file.setParms(mcname.text)
        }
    }


    Connections {
        target: button2
        onClicked: {
            file.editfile=mcselect.selectitem
            file.doCmd(deletemap)
        }
    }

    Connections {
        target: button
        onClicked: {
            file.doCmd(done)
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
