import QtQuick 6.5
import QtQuick.Controls 6.5
import QtLocation 6.5
import QtPositioning 6.5



Item {
    id: mapItem
    width: 200
    height: 200
    visible: true


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
                function onAddMarker(name, coordinate, type, color, date, description, link) {
                    const pin = markerComponent.createObject(map, {
                        coordinate: coordinate,
                        "sourceItem.color": color,
                        "sourceItem.eventData": {
                            title: name,
                            category: type,
                            coordinate: coordinate,
                            date: date,
                            description: description,
                            link: link
                        }
                    });

                    map.addMapItem(pin);
                }
            }

            Connections {
                target: mapController

                function onClearMarkers() {
                    for (let i = map.mapItems.length - 1; i >= 0; i--) {
                        const item = map.mapItems[i];
                        // Jeśli to instancja utworzona przez markerComponent – usuń
                        if (item.sourceItem && item.sourceItem instanceof Rectangle) {
                            map.removeMapItem(item);
                            item.destroy();  // usuwa obiekt z pamięci
                        }
                    }
                }
            }
        }

    }

    RoundButton { id: firesRoundButton
                  x: 728
                  y: 19
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
        x: 840
        y: 19
        width: 84
        height: 15
        text: "volcanoes"
        autoExclusive: true
        checkable: true
        onClicked: {
            mapController.selectEvents(text)
        }
    }

                  TextEdit {
                      id: textEdit
                      x: 74
                      y: 23
                      width: 112
                      height: 19
                      text: qsTr("Country name...")
                      font.pixelSize: 12
                  }

                  RoundButton {
                      id: seaLakeIceRoundButton
                      x: 608
                      y: 19
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
                      id: clearRoundButton
                      x: 930
                      y: 51
                      width: 69
                      height: 15
                      text: "clear"
                      checkable: true
                      autoExclusive: true
                      onClicked: {
                          mapController.clearEvents()
                      }
                  }

                  RoundButton {
                      id: renderRoundButton
                      x: 930
                      y: 81
                      width: 69
                      height: 15
                      text: "render"
                      checkable: true
                      autoExclusive: true
                      onClicked: {
                          mapController.generateAllEvents()
                      }
                  }

                  RoundButton {
                      id: stormRoundButton
                      x: 482
                      y: 19
                      width: 110
                      height: 15
                      text: "severeStorms"
                      checkable: true
                      autoExclusive: true
                      onClicked: {
                          mapController.selectEvents(text)
                      }
                  }

    Popup {
        id: eventPopup
        width: 250
        height: 120
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

                Column {
                    id: contentColumn
                    width: Math.max(parent.width, 500)
                    spacing: 5
                    padding: 10

                    Text { text: "Event title: " + (eventPopup.eventData?.title || "N/A") }
                    Text { text: "Category: " + (eventPopup.eventData?.category || "N/A") }
                    Text { text: "Coordinates: " + (eventPopup.eventData?.coordinate?.latitude || "?") + ", " + (eventPopup.eventData?.coordinate?.longitude || "?") }
                    Text { text: "Date: " + (eventPopup.eventData?.date || "N/A") }
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
