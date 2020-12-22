import QtQuick 2.0
import QtQuick.Controls 2.13
import SerialManager 1.0
import Qt.labs.platform 1.1
Rectangle{
    id: root
    property int serialConnect: 3
    property int serialDisconnect: 4
    property var analysisMsg
    property bool analysisresult: false
    property bool format: selectdataformat.selectitem!="字符串"
    Row {
        id: row
        width: root.width
        height: root.height
        spacing: width*0.01/2

        Item {
            id: column
            width: row.width*0.2
            height: row.height

            Flowrec {
                id: msseting
                _text:"配置"
                width: column.width*0.96
                height: column.height*0.9
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: msseting.textsize
                anchors.top: parent.top
                Column{
                    id: mssetone
                    width: parent.width*0.9
                    height: parent.height*0.8
                    spacing: 2
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.topMargin: msseting.textsize
                    anchors.top: parent.top

                    Selectbox {
                        id: selectcom
                        width: parent.width
                        prompttext: "串口选择:"
                        promptmodel: manager.ports
                    }

                    Selectbox {
                        id: selectbaudrate
                        width: parent.width
                        prompttext: "波 特 率 :"
                        promptmodel: [4800,7200,9600,14400,19200,38400,76800,115200,128000,921600]
                    }
                    Selectbox {
                        id: selectdataformat
                        width: parent.width
                        prompttext: "数据格式:"
                        promptmodel: ["字符串","16进制"]
                    }

                    Editarea {
                        id: mappath
                        width: parent.width
                        prompttext: "地图路径:"
                        editable: false
                        text: file.mappath
                    }
                    Button{
                        width: parent.width
                        text: "修改地图路径"
                        ToolTip.delay: 100
                        ToolTip.timeout: 5000
                        ToolTip.visible: hovered
                        ToolTip.text: qsTr("软件重启后生效")
                        onClicked: folderDialog.open()
                    }

                }

                Row {
                    id: row1
                    width: parent.width*0.96
                    height: parent.height*0.13
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: width*0.1
                    anchors.top: mssetone.bottom

                    Button {
                        id: msconnet
                        width: parent.width*0.45
                        text: qsTr("连接")
                        checkable: true
                    }

                    Button {
                        id: msdisconnect
                        width: parent.width*0.45
                        text: qsTr("断开")
                    }
                }




            }
        }

        Item {
            id: column1
            width: row.width*0.5
            height: row.height

            Flowrec {
                id: msdebug
                _text:"数据显示"
                width: column1.width*0.95
                height: column1.height*0.9
                anchors.topMargin: msdebug.textsize
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                Column{
                    id: column3
                    spacing: 10
                    anchors.fill: parent
                    anchors.margins: 20
                    Item {
                        width: parent.width
                        height: msdatarecive.textsize
                    }
                    Flowrec{
                        id: msdatarecive
                        width: parent.width
                        height: parent.height*0.4
                        _text:"数据接收"
                        textsize: 10
                        TextInput{
                            id:msdatarecivetext
                            font.family: "Tahoma"
                            wrapMode: TextEdit.WrapAnywhere
                            anchors.fill: parent
                            anchors.margins: 10
                            focus: true


                        }
                    }
                    Button{
                        id: msdataon
                        text: "显示"
                        anchors.right: msdatarecive.right
                        checkable: true
                        KeyNavigation.tab: datasend
                    }


                    Flowrec{
                        id: msdatasend
                        width: parent.width
                        height: parent.height*0.35
                        _text:"数据发送"

                        textsize: 10
                        FocusScope{
                            id:datasend
                            anchors.fill: parent
                            anchors.margins: 10
                            KeyNavigation.tab:mssend
                            TextInput{
                                id:datasendtext
                                font.family: "Tahoma"
                                wrapMode: TextEdit.WrapAnywhere
                                anchors.fill: parent
                                anchors.margins: 10
                                focus: true

                            }
                        }


                    }
                    Button{
                        id:mssend
                        text: qsTr("发送")
                        anchors.right: msdatasend.right
                    }
                }

            }
        }

        Item {
            id:column2
            width: parent.width*0.28
            height: parent.height
            Flowrec {
                id: msparse
                _text:"数据解析"
                width: column2.width
                height: column2.height*0.9
                anchors.topMargin: msparse.textsize
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                ScrollView{
                    id: scrollView
                    clip: true
                    width:parent.width*0.95
                    height: parent.height*0.9
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.topMargin: msparse.textsize
                    Column{
                        width: scrollView.width
                        anchors.horizontalCenter: parent.horizontalCenter
                        spacing: 15
                        Item {
                            width: parent.width
                            height: msxyz.textsize
                        }
                        Flowrec {
                            id: msreliable
                            width: parent.width
                            height: 100
                            textsize: 10
                            _text:"解算质量"
                            Column{
                                anchors.fill: parent
                                anchors.margins: 10
                                Editarea {
                                    id: carsatellite
                                    width: parent.width
                                    prompttext: "解状态:"
                                    text:msdataon.checked?analysisMsg[15]:""
                                    editable: false
                                }
                                Editarea {
                                    id: carresult
                                    width: parent.width
                                    prompttext: "解有效性:"
                                    text:analysisresult?"有效":"无效"
                                    editable: false
                                }
                            }
                        }
                        Flowrec {
                            id: msglobal
                            width: parent.width
                            _text:"地球坐标"
                            textsize: 10
                            height: 140
                            Column{
                                anchors.fill: parent
                                anchors.margins: 10
                                Editarea {
                                    id: cargx
                                    width: parent.width
                                    prompttext: "经度:"
                                    text:msdataon.checked?analysisMsg[9]:""
                                    editable: false
                                }
                                Editarea {
                                    id: cargy
                                    width: parent.width
                                    prompttext: "维度:"
                                    text:msdataon.checked?analysisMsg[10]:""
                                    editable: false
                                }
                                Editarea {
                                    id: cargz
                                    width: parent.width
                                    prompttext: "高程:"
                                    text:msdataon.checked?analysisMsg[11]:""
                                    editable: false
                                }
                            }
                        }
                        Flowrec {
                            id: msattitude
                            width: parent.width
                            _text:"姿态"
                            textsize: 10
                            height: 100
                            Column{
                                anchors.fill: parent
                                anchors.margins: 10
                                Editarea {
                                    id: carcourse
                                    width: parent.width
                                    prompttext: "航向角:"
                                    text:msdataon.checked?analysisMsg[5]:""
                                    editable: false
                                }
                                Editarea {
                                    id: carpitch
                                    width: parent.width
                                    prompttext: "俯仰角:"
                                    text:msdataon.checked?analysisMsg[6]:""
                                    editable: false
                                }
                            }
                        }
                        Flowrec {
                            id: msxyz
                            width: parent.width
                            height: 140
                            _text:"平面坐标"
                            textsize: 10
                            Column{
                                anchors.fill: parent
                                anchors.margins: 10
                                Editarea {
                                    id: carx
                                    width: parent.width
                                    prompttext: "X:"
                                    text:msdataon.checked?analysisMsg[12]:""
                                    editable: false
                                }
                                Editarea {
                                    id: cary
                                    width: parent.width
                                    prompttext: "Y:"
                                    text:msdataon.checked?analysisMsg[13]:""
                                    editable: false
                                }
                                Editarea {
                                    id: cayz
                                    width: parent.width
                                    prompttext: "Z:"
                                    text:msdataon.checked?analysisMsg[14]:""
                                    editable: false
                                }
                            }

                        }
                    }
                }
            }

        }

    }
    FolderDialog {
        id: folderDialog
        currentFolder: "file:///"+file.mappath//要求
        options :FolderDialog.ReadOnly
        onAccepted: {
            file.mappath=folderDialog.currentFolder
        }
    }
    Connections {
        target: msconnet
        onClicked: {

            selectbaudrate.enabled=!msconnet.checked;
            selectcom.enabled=!msconnet.checked;
            if(msconnet.checked)
            {
                manager.setParms(selectbaudrate.selectitem,selectcom.selectitem);

                manager.doCmd(serialConnect);
//                console.log(serialConnect)
//                manager.doCmd(manager.SerialConnect)

            }else{
                manager.doCmd(serialDisconnect);
            }
        }

    }

    Connections {
        target: msdisconnect
        onClicked: {
            selectbaudrate.enabled=true;
            selectcom.enabled=true;
            msconnet.checked=false;
            manager.doCmd(serialDisconnect);
        }
    }

    Connections {
        target: manager
        onReadDone:{
            msdatarecivetext.text=msdataon.checked?msg:""
            if(msg.length>100)
            {
                analysisMsg=msg.split(',')
            }
            analysisdata(msg);
        }
    }

    Connections {
        target: mssend
        //改进写法
        function onClicked(){
            if(datasendtext.text!=""){
                manager.sendMsg(datasendtext.text,format);
            }
            datasendtext.text=""
        }
//        onClicked: {
//            if(datasendtext.text!=""){
//                manager.sendMsg(datasendtext.text,format);
//            }
//            datasendtext.text=""
//        }
    }
    function analysisdata(data){
        var a=data[1].charCodeAt();
        for(var i=2;i<data.length-5;i++)
        {
            a=a ^ data[i].charCodeAt();
        }
        var c=parseInt( data.slice(-4),16);
        analysisresult= (a=== parseInt(data.slice(-4),16))
    }
}




/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
