import QtQuick
import QtQuick.Window

Window {
    width: 400
    height: 300
    visible: true
    title: "Qt 6 App with qt_add_qml_module"
    Text {
        anchors.centerIn: parent
        text: "Hello, World!"
    }
}