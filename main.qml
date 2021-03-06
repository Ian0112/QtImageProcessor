import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.1
import yan.qt.Dip 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("DIP")
/*
Always Visibale elements
*/

    Button {
        id: exit
        x: 100
        y: 0
        width: 100
        height: 40
        text: "Exit"
        onClicked: {
            Qt.quit()
        }
    }

    Button {
        id: openFile
        x: 0
        y: 0
        width: 100
        height: 40
        text: "Open"
        onClicked: {
            console.log("click open")
            fileDialogs.visible = true
        }
    }
    /*
    Text {
        id: test_text
        x: 34
        y: 51
        text: imageViewer.status == imageViewer.Ready ? 'Loaded' : 'Not loaded'
    }
    */
    Image {
        objectName: "imageViewer";
        id: imageViewer;
        anchors.topMargin: 41
        //source: "./1.jpg"
        asynchronous: true;
        anchors.fill: parent;
        fillMode: Image.PreserveAspectFit;

        onStatusChanged:{
            console.log("\n onStateChanged \n")
            if (imageViewer.status === Image.Loading){
                busyProcessImage.running = true
            }
            else if(imageViewer.status === Image.Ready){
                busyProcessImage.running = false
            }

        }

 /*       onStatusChanged: {
            if (imageViewer.status === Image.Loading) {
            //    busy.running = true;
                stateLabel.visible = false;
            }
            else if(imageViewer.status === Image.Ready){
          //      busy.running = false;
            }
            else if(imageViewer.status === Image.Error){
        //        busy.running = false;
                stateLabel.visible = true;
                stateLabel.text = "ERROR";
            }

        }
*/
    }


    ToolBar {
        id: toolBar
        x: 206
        y: 0
        width: 434
        height: 40
        position: ToolBar.Header
        contentWidth: 0

        ToolButton {
            id: gray
            x: 0
            y: 0
            width: 94
            height: 40
            text: qsTr("gray")
            onClicked: {
                console.log("qml")
                // if this button is clicked, the method of Dip object will be called
                processor.process(fileDialogs.fileUrl, Dip.Gray)
            }
        }

        ToolButton {
            id: reverse
            x: 75
            y: 0
            width: 75
            text: qsTr("reverse")
            onClicked: {
                console.log("reverse")
                processor.process(fileDialogs.fileUrl, Dip.Reverse)
            }
        }

        ToolButton {
            id: histEqu
            x: 150
            y: 0
            width: 75
            text: qsTr("histEqu")
            onClicked: {
                console.log("reverse")
                processor.process(fileDialogs.fileUrl, Dip.HistEqu)
            }
        }

        ToolButton {
            id: smooth
            x: 225
            y: 0
            width: 75
            text: qsTr("smooth")
            onClicked: {
                console.log("smooth")
                processor.process(fileDialogs.fileUrl, Dip.Smooth)
            }
        }
    }


/*
    Temporary visibale elements
*/
    BusyIndicator{
        id: busyProcessImage
        running: false
        anchors.centerIn: parent
        z: 2
    }


    // The element used to get file url
    FileDialog {
        id: fileDialogs;
        title: "Please choose a file";
        nameFilters: ["Image Files (*.jpg *.png *.gif)"];
        onAccepted: {
            console.log(fileDialogs.fileUrl);
            imageViewer.source = fileDialogs.fileUrl;
        }
    }
/*
Logic elements
*/
    Dip {
        id: processor;
        onFinished: {
            console.log("Process is finished.")
            imageViewer.source = "file:/" ; // if no this line, the picture will not refreshed.
            // If the picture is small, the above line doesn't work to refresh the picture.
            imageViewer.source = "file:///" +newFile;
        }
    }
}
