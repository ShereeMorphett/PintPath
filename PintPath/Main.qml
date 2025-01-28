import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts 2.15
import QtQuick.Window 2.15
import QtQuick.Controls.Material
import PintPath 1.0

import QtPositioning
import QtLocation

Window {
    id: window
    width: 640
    height: 480
    visible: true
    color: "#111113"
    title: qsTr("PintPath")

    Material.theme: Material.Dark
    Material.accent: Material.Purple

    PintBackend {

        id: backendManager
    }

    function updateVendorModel(vendors) {

        if (!vendors || vendors.length === 0) {
            console.log("No vendors found")
            return
        }

        let minLat = Number.MAX_VALUE
        let maxLat = -Number.MAX_VALUE
        let minLon = Number.MAX_VALUE
        let maxLon = -Number.MAX_VALUE

        for (var i = 0; i < vendors.length; i++) {
            let vendor = vendors[i]
            filteredVendorModel.append({
                                           "latitude": vendor.latitude,
                                           "longitude": vendor.longitude,
                                           "name": vendor.name,
                                           "address": vendor.address,
                                           "phone": vendor.phone,
                                           "websiteUrl": vendor.websiteUrl,
                                           "breweryType": vendor.breweryType
                                       })

            minLat = Math.min(minLat, vendor.latitude)
            maxLat = Math.max(maxLat, vendor.latitude)
            minLon = Math.min(minLon, vendor.longitude)
            maxLon = Math.max(maxLon, vendor.longitude)
        }

        let centerLat = (minLat + maxLat) / 2
        let centerLon = (minLon + maxLon) / 2
        view.map.center = QtPositioning.coordinate(centerLat, centerLon)

        // Calculate zoom level
        let latDiff = maxLat - minLat
        let lonDiff = maxLon - minLon
        let maxDiff = Math.max(latDiff, lonDiff)

        if (maxDiff < 0.01) {
            view.map.zoomLevel = 15
        } else if (maxDiff < 0.1) {
            view.map.zoomLevel = 12
        } else if (maxDiff < 1) {
            view.map.zoomLevel = 10
        } else {
            view.map.zoomLevel = 8
        }
    }

    function parseVendor(vendor) {
        if (vendor !== null) {
            popup.vendorName = vendor.name || "Unknown Name"
            popup.vendorAddress = (vendor.address || "Unknown Address")
            popup.vendorPhone = vendor.phone || "N/A"
            popup.vendorWebsite = vendor.websiteUrl || "N/A"
        } else {
            popup.vendorName = "No vendor found"
            popup.vendorAddress = "N/A"
            popup.vendorWebsite = "N/A"
            popup.vendorPhone = "N/A"
        }
    }

    Popup {
        id: popup
        width: parent.width - 20
        height: parent.height / 1.75
        rightPadding: -0.1
        margins: 4.5
        horizontalPadding: -0.1
        x: 0
        y: parent.height / 2
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
        Material.roundedScale: Material.LargeScale

        property string vendorName: ""
        property string vendorAddress: ""
        property string vendorPhone: ""
        property url vendorWebsite: ""

        enter: Transition {
            NumberAnimation {
                property: "opacity"
                from: 0.0
                to: 1.0
            }
        }

        exit: Transition {
            NumberAnimation {
                property: "opacity"
                from: 1.0
                to: 0.0
            }
        }

        ColumnLayout {
            id: columnLayout
            width: 100
            height: 100
            Text {
                color: "#ffffff"
                leftPadding: popup.padding + 3
                textFormat: Text.RichText
                text: "<span style='color: #CE93D8;'>" + popup.vendorName + " </span>"
                font.pixelSize: 20
            }
            Text {
                id: websiteText
                text: "<span style='color: #CE93D8;'>Website: </span>"
                      + "<a href='" + popup.vendorWebsite
                      + "' style='color: #ffffff;'>" + popup.vendorWebsite + "</a>"
                textFormat: Text.RichText
                wrapMode: Text.Wrap
                leftPadding: popup.padding + 3
                Layout.preferredWidth: popup.width - 2 * popup.padding
                verticalAlignment: Text.AlignVCenter
                visible: popup.vendorWebsite && String(
                             popup.vendorWebsite) !== "N/A"
                onLinkActivated: {
                    Qt.openUrlExternally(popup.vendorWebsite)
                }
            }
            Text {
                id: phoneNumberText
                text: "<span style='color: #CE93D8;'>Phone: </span>"
                      + "<a href='" + popup.vendorPhone
                      + "' style='color: #ffffff;'>" + popup.vendorPhone + "</a>"
                leftPadding: popup.padding + 3
                textFormat: Text.RichText
                visible: popup.vendorPhone !== "N/A"
                onLinkActivated: {
                    Qt.openUrlExternally("tel:" + popup.vendorPhone)
                }
            }
            Text {
                id: addressText
                color: "#ffffff"
                textFormat: Text.RichText
                text: "<span style='color: #CE93D8;'>Address: </span>" + popup.vendorAddress
                leftPadding: popup.padding + 3
                Layout.preferredWidth: popup.width - 2 * popup.padding
                wrapMode: Text.Wrap
                verticalAlignment: Text.AlignVCenter
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        TabBar {
            id: bar
            spacing: 5
            currentIndex: 0
            focusPolicy: Qt.ClickFocus
            enabled: true
            Layout.fillWidth: true

            TabButton {
                text: qsTr("Breweries")
                spacing: 10
                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: true
                onClicked: {
                    bar.currentIndex = 0
                }
            }

            TabButton {
                text: qsTr("Map")
                spacing: 10
                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: true
                onClicked: {
                    bar.currentIndex = 1
                }
            }
            TabButton {
                text: qsTr("Setting")
                spacing: 10
                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: true
                onClicked: {
                    bar.currentIndex = 2
                }
            }
        }

        StackLayout {
            id: stack
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: bar.currentIndex

            Item {
                id: breweriesTab
                Layout.fillWidth: true

                Item {
                    width: parent.width - 5
                    anchors.margins: 10
                    GridLayout {
                        id: column
                        rows: 2
                        columns: 2
                        property string selectedTab: ""
                        anchors.fill: parent
                        columnSpacing: 3

                        Button {
                            id: northern_brew
                            text: qsTr("Northernmost Brewery")
                            highlighted: column.selectedTab === "northern"
                            flat: false
                            onClicked: {
                                column.selectedTab = "northern"
                                let northVendor = backendManager.findNorthern()
                                window.parseVendor(northVendor)
                                popup.open()
                            }
                        }

                        Button {
                            id: southern_brew
                            text: qsTr("Southernmost Brewery")
                            highlighted: column.selectedTab === "southern"
                            // Layout.fillWidth: true
                            flat: false
                            onClicked: {
                                column.selectedTab = "southern"
                                let southVendor = backendManager.findSouthern()
                                window.parseVendor(southVendor)
                                popup.open()
                            }
                        }
                        Button {
                            id: longest_name
                            text: qsTr("Longest Name")
                            highlighted: column.selectedTab === "longest"
                            flat: false
                            onClicked: {
                                column.selectedTab = "longest"
                                let longestVendor = backendManager.findLongestName()
                                window.parseVendor(longestVendor)
                                popup.open()
                            }
                        }
                    }
                }
            }

            Plugin {
                id: myPlugin
                name: "osm"
            }

            Item {
                id: mapTab
                ColumnLayout {
                    anchors.fill: parent
                    spacing: 10

                    ComboBox {
                        visible: true
                        Layout.fillWidth: true
                        model: ["Southernmost Brewery", "Northernmost Brewery", "Longest Name", "Serves Food", "Choose a filter"]
                        currentIndex: 4
                        onActivated: index => {
                                         switch (index) {
                                             case 0:
                                             window.updateVendorModel(
                                                 [backendManager.findSouthern(
                                                      )])
                                             break
                                             case 1:
                                             window.updateVendorModel(
                                                 [backendManager.findNorthern(
                                                      )])
                                             break
                                             case 2:
                                             window.updateVendorModel(
                                                 [backendManager.findLongestName(
                                                      )])
                                             break
                                             case 3:
                                             window.updateVendorModel(
                                                 backendManager.findServesFood(
                                                     ))
                                             default:
                                             break
                                         }
                                     }
                    }

                    ListModel {
                        id: filteredVendorModel
                    }

                    Rectangle {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        MapView {
                            id: view
                            anchors.fill: parent

                            visible: true
                            map.plugin: myPlugin

                            map.zoomLevel: -40

                            MapItemView {
                                model: filteredVendorModel
                                parent: view.map
                                delegate: MapQuickItem {
                                    coordinate: QtPositioning.coordinate(
                                                    model.latitude,
                                                    model.longitude)

                                    anchorPoint.x: rectangle.width * 0.5
                                    anchorPoint.y: rectangle.height

                                    sourceItem: Column {
                                        spacing: 5
                                        Rectangle {
                                            id: rectangle
                                            width: 15
                                            height: 15
                                            radius: 7
                                            color: "red"
                                            border.color: "black"
                                            border.width: 1
                                            MouseArea {
                                                anchors.fill: parent
                                                onClicked: {
                                                    popup.vendorName = model.name
                                                            || "Unknown Name"
                                                    popup.vendorAddress = model.address
                                                            || "Address Not Available"
                                                    popup.vendorPhone = model.phone
                                                            || "N/A"
                                                    popup.vendorWebsite = model.websiteUrl
                                                            || "N/A"
                                                    popup.open()
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            Item {
                id: aboutTab
                Text {
                    text: "Settings"
                    anchors.top: parent.top
                    anchors.topMargin: 0
                    font.pixelSize: 16
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: "#ffffff"
                }

                Button {
                    text: qsTr("Refresh Data")
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    onClicked: {
                        backendManager.sendRequest(
                                    "https://api.openbrewerydb.org/v1/breweries?by_country=ireland&per_page=200")
                    }
                }
            }
        }
    }
}
