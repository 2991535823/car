import QtQuick 2.0
import Qt.labs.platform 1.1
Item {
    id:root
    property bool options: false
    function open(msg){
       diag.informativeText=msg
       diag.open();
    }
    signal clickmsg(bool info)
    MessageDialog {
        id:diag
        buttons: options?MessageDialog.Yes|MessageDialog.Cancel:MessageDialog.Yes
        text: "提示消息"
        informativeText: ""
        onYesClicked: {
            root.clickmsg(true)
        }
        onCancelClicked: {
            root.clickmsg(false);
        }
    }
}
