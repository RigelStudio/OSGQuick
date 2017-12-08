import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1

Window {
    visible: true;
    width: 640;
    height: 480;
    title: qsTr("Hello World");
	GridLayout{
        id: gridLayout;
        anchors.fill: parent;
        anchors.margins: 10;
        Rectangle{
            id:rect;
            color: "red";
            Layout.fillWidth: true;
            Layout.fillHeight: true;
        }
    }
}
