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
    color: "#cbe2d8"

    ListModel {
    id: eventModel
}


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
        x: 1058
        y: 56
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
    // Debug recentEventsReady signal for malformed event data
    Connections {
        target: mapController
        function onRecentEventsReady(eventList) {
            eventModel.clear()
            for (let i = 0; i < eventList.length; i++) {
                const event = eventList[i];
                if (event && event.title !== undefined && event.latitude !== undefined && event.longitude !== undefined) {
                    eventModel.append({
                        title: event.title,
                        lat: event.latitude,
                        lon: event.longitude
                    });
                }
            }
        }
    }

    ListView {
        id: listView
        x: 1121
        y: 385
        width: 110
        height: 160
        visible: false
        model: eventModel

        delegate: Item {
            width: listView.width
            height: column.implicitHeight

            Column {
                id: column
                spacing: 4

                Text {
                    text: title
                    font.bold: true
                    wrapMode: Text.WrapAnywhere
                    width: listView.width
                    elide: Text.ElideRight
                }
                Text {
                    text: "Lat: " + lat + ", Lon: " + lon
                    font.pointSize: 10
                    color: "#555"
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    mapController.centerMapOn(title)
                    mapItem.centerMapOn(lat, lon, title)
                }
            }
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

            PropertyChanges {
                target: listView
                x: 968
                y: 380
                width: 286
                height: 229
                visible: true
            }

            PropertyChanges {
                target: daysSlider
                y: 306
                anchors.bottomMargin: 366
                anchors.horizontalCenterOffset: 471
                visible: true
            }

            PropertyChanges {
                target: text1
                y: 273
                anchors.bottomMargin: 4
                anchors.horizontalCenterOffset: 486
                visible: true
            }
        }
    ]
    Slider {
        id: daysSlider
        y: 536
        from: 1
        to: 30
        stepSize: 1
        value: 7
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 136
        anchors.horizontalCenterOffset: 409
        width: parent.width * 0.2
        visible: false



        onValueChanged: {
            mapController.generateRecentEvents(Math.round(value))
        }
    }

    Text {
        id: text1
        y: 506
        visible: false
        text: "Days back: " + Math.round(daysSlider.value)
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: daysSlider.top
        anchors.bottomMargin: 6
        anchors.horizontalCenterOffset: 440
        font.bold: true
    }
}
