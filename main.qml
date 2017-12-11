import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1
import OSGQuick.OSGItem 1.0

Window {
	id:mainWindow;
    visible: true;
    width: 640;
    height: 480;
	color: "red";
    title: qsTr("Hello World");
	OSGItem{
        id:rect;
        anchors.fill: parent;
        Layout.fillWidth: true;
		Layout.fillHeight: true;
		
    }
	/*MouseArea {
            anchors.fill: parent;
        }
	GridLayout{
        id: gridLayout;
        anchors.fill: parent;
        anchors.margins: 10;
		
    }*/
}
