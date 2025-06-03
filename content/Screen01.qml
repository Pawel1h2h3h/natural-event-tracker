/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/

import QtQuick 6.5
import QtQuick.Controls 6.5
import Nasa_ui

Rectangle {
    id: rectangle1
    width: Constants.width
    height: Constants.height
    color: "#bbbcde"


    MyMapItem {
        id: mapItem
        x: 53
        y: 56
        width: 977
        height: 591
        visible: false

    }


    Item {
        id: roundButtonsItem
        x: 1064
        y: 112
        width: 202
        height: 211

        RoundButton {
            id: menuRoundButton
            x: 41
            y: 127
            width: 125
            height: 40
            text: "Menu"
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 44
            checkable: true
            autoExclusive: true
        }

        RoundButton {
            id: mapRoundButton
            x: 41
            width: 125
            height: 40
            text: "Map"
            anchors.top: parent.top
            anchors.topMargin: 41
            autoExclusive: true
            checkable: true
        }
    }

    states: [
        State {
            name: "MapState"
            when: mapRoundButton.checked

            PropertyChanges {
                target: mapItem
                x: 22
                y: 31
                width: 1007
                height: 613
                visible: true
            }
        }
    ]
}
