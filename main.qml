import QtQuick 2.12
import QtQuick.Window 2.12
import SerialManager 1.0
Window {
    visible: true
    width: 1080
    height: width/16*9
    title: qsTr("Control")
    Serial{
        id:manager
    }
    Component.onCompleted: {
        manager.setParms("115200","COM3");
    }

    Tabwindows {
        id: tabs
        width: parent.width; height: parent.height
        Rectangle {
            property string title: "欢迎"
            anchors.fill: parent
            color: "#e3e3e3"
            SplashLayout {
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
            color: "#e3e3e3"
            MaprunLayout{
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
