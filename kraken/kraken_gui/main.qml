import QtQuick 2.0

Rectangle {
    id: simplebutton
    color: "grey"
    width: 150; height: 75

    Text{
        id: buttonLabel
        anchors.centerIn: parent
        text: "button label"
    }
}
