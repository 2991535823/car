import QtQuick 2.0
import QtQuick.Controls 2.13
//import MapManager 1.0
import QtWebEngine 1.8
import QtWebChannel 1.15
import MapAssist 1.0
Rectangle{
    id: root
    property int s2e:0X14
    property int e2s:0X15
    property int c2e:0X16
    property int e2c:0X17
    property int start: 0x1b
    property int stop: 0x1c
    property int resume: 0x1d
    property var mode: ["起点-终点","终点-起点","当前-终点","终点-当前"]
    Row {
        id: row
        width: root.width
        height: root.height

        Item {
            id: column
            width: row.width*0.3
            height: row.height*0.86

            Flowrec {
                id: mrload
                _text:"地图载入"
                width: column.width*0.96
                height: column.height*0.45
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: mrload.textsize
                anchors.top: parent.top
                Column{
                    id: mrc
                    width: parent.width*0.9
                    height: parent.height*0.8
                    spacing: 2
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.topMargin: mrload.textsize
                    anchors.top: parent.top
                    Selectbox {
                        id: selectboxmap
                        prompttext: "地图选择:"
                        promptmodel: file.maplist
                        width: parent.width
                    }

                    Selectbox {
                        id:mrmapbackswitch
                        prompttext: "返航开关:"
                        promptmodel: ["开","关"]
                        width: parent.width
                    }
                    Selectbox {
                        id: mrbtnmode
                        promptmodel: mode
                        width: parent.width
                        prompttext: qsTr("模式选择:")
                    }
                }

                Button{
                    id: mrbtnload
                    x: 9
                    y: 146
                    width: mrc.width
                    text: "载入地图"
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 30
                }



            }

            Flowrec {
                id: mrcontrol
                _text:"运动控制"
                width: column.width*0.96
                height: column.height*0.45
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: mrcontrol.textsize
                anchors.top: mrload.bottom
                Column{
                    width: parent.width*0.9
                    height: parent.height*0.8
                    spacing: 2
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.topMargin: mrcontrol.textsize
                    anchors.top: parent.top
                    Button{
                        id: mrbtnstart
                        width: parent.width
                        text: "开始导航"
                    }
                    Button{
                        id: mrbtnstop
                        width: parent.width
                        text: "停止导航"
                    }
                    Button{
                        id: mrbtncontinue
                        width: parent.width
                        text: "恢复导航"
                    }
                }

            }
        }

        Item {
            id: column1
            width: row.width-column.width
            height: row.height

            Flowrec {
                id: mrview
                _text:"地图显示"
                width: column1.width*0.95
                height: column1.height*0.8
                anchors.topMargin: mrview.textsize
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                Rectangle{
                    id:monitor
                    anchors.fill: parent
                    anchors.margins: 15
                    WebEngineView{
                        url: "qrc:/index.html"
                        id:runmap
                        anchors.fill: parent
                        webChannel: myChannel
                    }

                }


            }

            Item {
                id: mrbtnrow
                width: column1.width
                height: column1.height*0.12
                anchors.topMargin: (column1.height-mrview.height-mrview.textsize-mrbtnrow.height)/2
                anchors.top: mrview.bottom

                Button {
                    id: mrbtnmap
                    text: qsTr("显示地图")
                    checkable: true
                    font.family: "Tahoma"
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.rightMargin: 20
                    anchors.right: mrbtncar.left
                }

                Button {
                    id: mrbtncar
                    text: qsTr("显示小车")
                    font.family: "Tahoma"
                    checkable: true
                    anchors.rightMargin: 20
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                }
            }
        }
    }

    Connections {
        target: mrbtncar
        onClicked: {
            mapassist.carpoint={"x":"106.2459461","y":"29.4899794"};
            mapassist.mapdata=[{"x":"106.2471654","y":"29.4898980"},
                               {"x":"106.2470473","y":"29.4897025"},
                               {"x":"106.2470356","y":"29.4898333"},
                               {"x":"106.2470877","y":"29.4899972"},
                               {"x":"106.2469209","y":"29.4896956"},
                               {"x":"106.2466642","y":"29.4896896"},
                               {"x":"106.2464395","y":"29.4897550"},]
        }
    }

    Connections {
        target: mrbtnmap
        onClicked: {
            file.editfile=selectbox.selectitem
            //            monitormap.viewmap=mrbtnmap.checked
        }
    }
    Component.onCompleted: {
        //        monitormap.serial=manager;
        //        monitormap.file=file;
    }
    Connections {
        target: selectboxmap
        onEdited: {
            file.editfile=msg;
        }
    }

    Connections {
        target: mrmapbackswitch
        onEdited: {
            if(msg===mrmapbackswitch.promptmodel[0])
            {
                cmd.orNotBack=true;
            }else
            {
                cmd.orNotBack=false;
            }
        }
    }

    Connections {
        target: mrbtnload
        onClicked: {
            file.editfile=selectboxmap.selectitem
//            cmd.orNotBack=mrmapbackswitch.selectitem===mrmapbackswitch.promptmodel[0];
//            console.log(selectboxmap.selectitem)
            cmd.sendtostm32(manager);
        }
    }

    Connections {
        target: mrbtnstart
        onClicked: {
            cmd.setCmd(start)
            cmd.sendtostm32(manager,false)
        }
    }

    Connections {
        target: mrbtnstop
        onClicked: {
            cmd.setCmd(stop)
            cmd.sendtostm32(manager,false)
        }
    }

    Connections {
        target: mrbtncontinue
        onClicked: {
            cmd.setCmd(resume)
            cmd.sendtostm32(manager,false)
        }
    }

    Connections {
        target: mrbtnmode
        onEdited: {
            if(msg===mode[0]){
                cmd.setMode(s2e)
            }else if(msg===mode[1])
            {
                cmd.setMode(e2s)
            }else if(msg=== mode[2])
            {
                cmd.setMode(c2e)
            }else if(msg===mode[3])
            {
                cmd.setMode(e2c)
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
