import QtQuick 6.5
import QtQuick.Controls 6.5
import QtLocation 6.5
import QtPositioning 6.5



Item {
    id: mapItem
    width: 1060
    height: 640
    visible: true

    function centerMapOn(lat, lon, title) {
        map.center = QtPositioning.coordinate(lat, lon)
    }


    Rectangle {
        id: rectangle
        x: 39
        y: 51
        width: 885
        height: 521
        color: "#ffffff"
        rotation: 0

        Map {
            id: map
            anchors.fill: parent
            plugin: Plugin { name: "osm" }
            center: QtPositioning.coordinate(52.2297, 21.0122)
            zoomLevel: 10

            WheelHandler {
                id: wheelZoomHandler
                acceptedDevices: PointerDevice.Mouse
                property real zoomSpeed: 0.2

                onWheel: (event) => {
                             if (event.angleDelta.y > 0 && map.zoomLevel < 21) {
                                 map.zoomLevel += zoomSpeed
                             } else if (event.angleDelta.y < 0 && map.zoomLevel > 1) {
                                 map.zoomLevel -= zoomSpeed
                             }
                             event.accepted = true
                         }
            }

            DragHandler {
                id: dragHandler
                target: null  // pozwala przeciągać mapę, a nie komponent
                onActiveChanged: {
                    if (active) {
                        map.startCentroid = map.toCoordinate(dragHandler.centroid.pressPosition)
                    } else {
                        const releaseCoord = map.toCoordinate(dragHandler.centroid.position)
                        const deltaLat = map.startCentroid.latitude - releaseCoord.latitude
                        const deltaLon = map.startCentroid.longitude - releaseCoord.longitude
                        map.center = QtPositioning.coordinate(map.center.latitude + deltaLat,
                                                              map.center.longitude + deltaLon)
                    }
                }
            }

            PinchHandler {
                id: pinch
                target: null
                onActiveChanged: if (active) {
                                     map.startCentroid = map.toCoordinate(pinch.centroid.position, false)
                                 }
                onScaleChanged: (delta) => {
                                    map.zoomLevel += Math.log2(delta)
                                    map.alignCoordinateToPoint(map.startCentroid, pinch.centroid.position)
                                }
                onRotationChanged: (delta) => {
                                       map.bearing -= delta
                                       map.alignCoordinateToPoint(map.startCentroid, pinch.centroid.position)
                                   }
                grabPermissions: PointerHandler.TakeOverForbidden
            }

            property var startCentroid

            Component {
                id: markerComponent

                MapQuickItem {
                    anchorPoint.x: 8
                    anchorPoint.y: 8
                    coordinate: QtPositioning.coordinate(0, 0)
                    sourceItem: Rectangle {
                        id: markerRect
                        property var eventData
                        property real baseSize: 16
                        width: baseSize * ((typeof map.zoomLevel === "number" ? map.zoomLevel : 10) / 10)
                        height: baseSize * ((typeof map.zoomLevel === "number" ? map.zoomLevel : 10) / 10)
                        radius: width / 2
                        color: "#ff0000"
                        border.color: "#000000"
                        border.width: 1

                        MouseArea {
                            anchors.fill: parent
                            acceptedButtons: Qt.RightButton
                            onClicked: (mouse) => {
                                           if (mouse.button === Qt.RightButton) {
                                               eventPopup.eventData = parent.eventData;
                                               eventPopup.x = (typeof mouse.screenX === "number" ? mouse.screenX : map.width / 2);
                                               eventPopup.y = (typeof mouse.screenY === "number" ? mouse.screenY : map.height / 2);
                                               eventPopup.open();
                                           }
                                       }
                        }
                    }
                }
            }

            Connections {
                target: mapController

                function onAddMarker(name, coordinate, type, color, date, description, link, magnitude, sources, categories, coordinates) {
                    const pin = markerComponent.createObject(map, {
                                                                 coordinate: coordinate,
                                                                 "sourceItem.color": color,
                                                                 "sourceItem.eventData": {
                                                                     title: name,
                                                                     category: type,
                                                                     coordinate: coordinate,
                                                                     date: date,
                                                                     description: description,
                                                                     link: link,
                                                                     magnitude: magnitude,
                                                                     sources: sources,
                                                                     categories: categories,
                                                                     coordinatesInfo: coordinates
                                                                 }
                                                             });

                    map.addMapItem(pin);
                }

                function onClearMarkers() {
                    for (let i = map.mapItems.length - 1; i >= 0; i--) {
                        const item = map.mapItems[i];
                        if (item.sourceItem && item.sourceItem instanceof Rectangle) {
                            map.removeMapItem(item);
                            item.destroy();
                        }
                    }
                }
            }

        }

        RoundButton { id: firesRoundButton
            x: 691
            y: -30
            width: 84
            height: 15
            text: "wildfires"
            autoExclusive: true
            checkable: true
            onClicked: {
                mapController.selectEvents(text)
            }
        }
        RoundButton { id: volcanoRoundButton
            x: 781
            y: -30
            width: 84
            height: 15
            text: "volcanoes"
            autoExclusive: true
            checkable: true
            onClicked: {
                mapController.selectEvents(text)
            }
        }

        RoundButton {
            id: seaLakeIceRoundButton
            x: 555
            y: -30
            width: 102
            height: 15
            text: "seaLakeIce"
            autoExclusive: true
            checkable: true
            onClicked: {
                mapController.selectEvents(text)
            }
        }
        RoundButton {
            id: dustHazeRoundButton
            x: 310
            y: -30
            width: 102
            height: 15
            text: "dustHaze"
            checkable: true
            autoExclusive: true
            onClicked: {
                mapController.selectEvents(text)
            }
        }

        RoundButton {
            id: earthquakesRoundButton
            x: 202
            y: -30
            width: 102
            height: 15
            text: "earthquakes"
            checkable: true
            autoExclusive: true
            onClicked: {
                mapController.selectEvents(text)
            }
        }

        RoundButton {
            id: floodsRoundButton
            x: 12
            y: -30
            width: 82
            height: 15
            text: "floods"
            checkable: true
            autoExclusive: true
            onClicked: {
                mapController.selectEvents(text)
            }
        }

        RoundButton {
            id: landSlidesRoundButton
            x: 105
            y: -30
            width: 91
            height: 15
            text: "landslides"
            checkable: true
            autoExclusive: true
            onClicked: {
                mapController.selectEvents(text)
            }
        }

        RoundButton {
            id: manMadeRoundButton
            x: 14
            y: 533
            width: 79
            height: 15
            text: "manmade"
            checkable: true
            autoExclusive: true
            onClicked: {
                mapController.selectEvents(text)
            }
        }

        RoundButton {
            id: snowRoundButton
            x: 99
            y: 533
            width: 59
            height: 15
            text: "snow"
            checkable: true
            autoExclusive: true
            onClicked: {
                mapController.selectEvents(text)
            }
        }

        RoundButton {
            id: tempExtremesRoundButton
            x: 164
            y: 533
            width: 108
            height: 15
            text: "tempExtremes"
            checkable: true
            autoExclusive: true
            onClicked: {
                mapController.selectEvents(text)
            }
        }

        RoundButton {
            id: waterColorRoundButton
            x: 278
            y: 533
            width: 85
            height: 15
            text: "waterColor"
            checkable: true
            autoExclusive: true
            onClicked: {
                mapController.selectEvents(text)
            }
        }

        RoundButton {
            id: clearRoundButton
            x: 903
            y: 25
            width: 93
            height: 15
            text: "clear"
            checkable: true
            autoExclusive: true
            onClicked: {
                mapController.clearEvents()
            }
        }

        RoundButton {
            id: openEventsRoundButton
            objectName: "openEventsRoundButton"
            x: 903
            y: 55
            width: 93
            height: 15
            text: "show open"
            checkable: true
            autoExclusive: true
            onClicked: {
                mapController.generateEvents(objectName)
            }
        }
        RoundButton {
            id: allEventsRoundButton
            objectName: "allEventsRoundButton"
            x: 903
            y: 84
            width: 93
            height: 15
            text: "show all"
            checkable: true
            autoExclusive: true
            onClicked: {
                mapController.generateEvents(objectName)
            }
        }

        RoundButton {
            id: closedEventsRoundButton
            objectName: "closedEventsRoundButton"
            x: 903
            y: 117
            width: 93
            height: 15
            text: "show closed"
            checkable: true
            autoExclusive: true
            onClicked: {
                mapController.generateEvents(objectName)
            }
        }

        RoundButton {
            id: stormRoundButton
            x: 422
            y: -30
            width: 110
            height: 15
            text: "severeStorms"
            checkable: true
            autoExclusive: true
            onClicked: {
                mapController.selectEvents(text)
            }
        }
    }


    Popup {
        id: eventPopup
        width: 250
        height: Math.min(contentColumn.implicitHeight + 20, 400)
        modal: true
        focus: true
        closePolicy: Popup.CloseOnPressOutside

        property var eventData

        Rectangle {
            anchors.fill: parent
            color: "white"
            border.color: "black"
            border.width: 1

            Flickable {
                anchors.fill: parent
                interactive: true
                contentHeight: contentColumn.height
                contentWidth: contentColumn.width
                clip: true
                maximumFlickVelocity: 5000

                Column {
                    id: contentColumn
                    width: parent.width
                    spacing: 5
                    padding: 10

                    Text { text: "Event title: " + (eventPopup.eventData?.title || "N/A") }
                    Text { text: "Category: " + (eventPopup.eventData?.category || "N/A") }
                    Text { text: "Coordinates: " + (eventPopup.eventData?.coordinate?.latitude || "?") + ", " + (eventPopup.eventData?.coordinate?.longitude || "?") }
                    Text { text: "Date: " + (eventPopup.eventData?.date || "N/A") }
                    // Additional info fields
                    Text { text: "Magnitude: " + (eventPopup.eventData?.magnitude || "N/A") }
                    Text { text: "Sources: " + (eventPopup.eventData?.sources || "N/A") }
                    Text { text: "Categories: " + (eventPopup.eventData?.categories || "N/A") }
                    Text { text: "Location: " + (eventPopup.eventData?.coordinatesInfo || "N/A") }
                    Text { text: "Description: " + (eventPopup.eventData?.description || "N/A") }
                    Text {
                        text: "Link"
                        color: "blue"
                        font.underline: true
                        MouseArea {
                            anchors.fill: parent
                            onClicked: Qt.openUrlExternally(eventPopup.eventData?.link || "")
                        }
                    }
                }
                ScrollBar.vertical: ScrollBar {
                    policy: ScrollBar.AlwaysOn
                }
                ScrollBar.horizontal: ScrollBar {
                    policy: ScrollBar.AsNeeded
                }
            }
        }
    }
}
