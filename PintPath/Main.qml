import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts 2.15
import QtQuick.Window 2.15
import PintPath

Window {
    id: window
    width: 640
    height: 480
    visible: true
    color: "#ffffff"
    title: qsTr("PintPath")

    PintBackend {

        id: backendManager
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
            }

            TabButton {
                text: qsTr("Map")
                spacing: 10
                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: true
            }
            TabButton {
                text: qsTr("Setting")
                spacing: 10
                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: true
            }
        }

        StackLayout {
            id: stack
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: bar.currentIndex

            Item {
                id: breweriesTab
                visible: true
                clip: false

                Popup {
                    id: popup
                    width: parent.width
                    height: parent.height / 2
                    x: 0
                    y: parent.height / 2
                    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

                    property string vendorName: ""
                    property string vendorAddress: ""
                    property string vendorPhone: ""

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
                    Rectangle {
                        anchors.fill: parent
                        color: "#ffffff"
                        radius: 16
                        border.color: "#cccccc"
                        border.width: 1

                        ColumnLayout {
                            id: columnLayout
                            width: 100
                            height: 100

                            Text {
                                text: "Name: " + popup.vendorName
                                font.pixelSize: 16
                            }

                            Text {
                                text: "Address: " + popup.vendorAddress
                                font.pixelSize: 16
                            }
                        }
                    }
                }

                Column {
                    id: column
                    width: 200
                    height: 400

                    Button {
                        id: northern_brew
                        text: qsTr("Northern Most Brewery")
                        highlighted: true
                        flat: false
                        onClicked: {
                            let longestVendor = backendManager.findNorthern()
                            if (longestVendor !== null) {
                                popup.vendorName = longestVendor.name
                                popup.vendorAddress = longestVendor.address1
                                        + ", " + longestVendor.city
                            } else {
                                popup.vendorName = "No vendor found"
                                popup.vendorAddress = "N/A"
                            }
                            popup.open()
                        }
                    }

                    Button {
                        id: southern_brew
                        text: qsTr("Southern Most Brewery")
                        highlighted: false
                        flat: false
                        onClicked: {
                            let longestVendor = backendManager.findSouthern()
                            if (longestVendor !== null) {
                                popup.vendorName = longestVendor.name
                                popup.vendorAddress = longestVendor.address1
                                        + ", " + longestVendor.city
                            } else {
                                popup.vendorName = "No vendor found"
                                popup.vendorAddress = "N/A"
                            }
                            popup.open()
                        }
                    }

                    Button {
                        id: longest_name
                        text: qsTr("Longest Name")
                        highlighted: false
                        flat: false
                        onClicked: {
                            let longestVendor = backendManager.findLongestName()
                            if (longestVendor !== null) {
                                popup.vendorName = longestVendor.name
                                popup.vendorAddress = longestVendor.address1
                                        + ", " + longestVendor.city
                            } else {
                                popup.vendorName = "No vendor found"
                                popup.vendorAddress = "N/A"
                            }
                            popup.open()
                        }
                    }
                }
            }

            Item {
                id: mapTab
                visible: false
                Text {
                    visible: true
                    anchors.centerIn: parent
                    text: qsTr("Map View (To be implemented)")
                    anchors.left: parent.horizontalCenter
                    anchors.leftMargin: 0
                    anchors.verticalCenterOffset: 200
                    anchors.horizontalCenterOffset: 300
                }
            }

            Item {
                id: aboutTab
                visible: bar.currentIndex === 2
                Text {
                    anchors.centerIn: parent
                    text: qsTr("Settings")
                    font.pixelSize: 18
                    font.bold: true
                    Button {
                        id: referesh_data
                        text: qsTr("Refresh Data")
                        highlighted: false
                        flat: false
                        onClicked: {
                            backendManager.sendRequest(
                                        "https://api.openbrewerydb.org/v1/breweries?by_country=ireland&per_page=200")
                        }
                    }
                }
            }
        }
    }
}
