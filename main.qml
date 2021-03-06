﻿import QtQuick 2.12
import QtQuick.Window 2.12
import SerialManager 1.0
import FileManager 1.0
import CmdManager 1.0
import MapAssist 1.0
import QtWebChannel 1.0
import QtWebEngine 1.8
Window {
    visible: true
    width: 1080
    height: width/16*9
    title: qsTr("自动导航人机交互系统")
    Serial{
        id:manager
    }
    File{
        id:file
    }
    Cmd{
        id:cmd
    }

    WebChannel{
        id:myChannel
        registeredObjects: [mapassist]
    }
    MapAssist{
        id:mapassist
        WebChannel.id:"mapAssistObject"
    }
    Component.onCompleted: {
        file.serial=manager
        cmd.mapServer(file)
        cmd.serialServer(manager)
        mapassist.serial=manager
        mapassist.file=file

    }

    Tabwindows {
        id: tabs
        width: parent.width; height: parent.height
        Rectangle {
            property string title: "欢迎"
            anchors.fill: parent
            color: "#e3e3e3"
            SplashLayout{
                anchors.fill: parent
                anchors.margins: 10
                onBtnclick: {
                    switch(val)
                    {
                    case true:
                        tabs.current=1;
                        break;
                    case false:
                        Qt.quit();
                        break;
                    }
                }
            }
        }
        Rectangle {
            property string title: "运行监控"
            anchors.fill: parent
            color:"#e3e3e3"
            MaprunLayout{
                id: maprunLayout
                anchors.fill: parent
                anchors.margins: 10
            }
        }
        Rectangle {
            property string title: "创建地图"
            anchors.fill: parent
            color: "#e3e3e3"
            MapcreateLayout{
                anchors.fill: parent
                anchors.margins: 10
            }
        }
        Rectangle {
            property string title: "设置及调试"
            anchors.fill: parent
            color: "#e3e3e3"
            SetDebugLayout{
                anchors.fill: parent
                anchors.margins: 10
            }
        }
    }
}
