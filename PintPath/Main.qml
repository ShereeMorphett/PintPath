import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts 2.15
import QtQuick.Window 2.15
import QtQuick.Controls.Material
import PintPath

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

    function parseVendor(vendor) {
        if (vendor !== null) {
            popup.vendorName = vendor.name || "Unknown Name"
            popup.vendorAddress = (vendor.address1
                                   || "Unknown Address") + ", " + (vendor.city
                                                                   || "Unknown City")

            console.log(popup.vendorPhone)
            popup.vendorPhone = (vendor.phone || "N/A")

            if (vendor.website_url.startsWith("http://")
                    || vendor.website_url.startsWith("https://")) {
                popup.vendorWebsite = vendor.website_url
            } else if (vendor.website_url.startsWith("www.")) {
                popup.vendorWebsite = "https://" + vendor.website_url
            } else {
                popup.vendorWebsite = "N/A"
            }
        } else {
            popup.vendorName = "No vendor found"
            popup.vendorAddress = "N/A"
            popup.vendorWebsite = "N/A"
        }
        console.log("" + +"")
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
                Popup {
                    id: popup
                    width: parent.width - 20
                    height: parent.height / 2
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
                            leftPadding: popup.padding + 3
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
                            onLinkActivated: {
                                Qt.openUrlExternally("tel:" + popup.vendorPhone)
                            }
                        }
                        Text {
                            id: addressText
                            color: "#ffffff"
                            textFormat: Text.RichText
                            text: "<span style='color: #CE93D8;'>Address: </span>"
                                  + popup.vendorAddress
                            leftPadding: popup.padding + 3
                            Layout.preferredWidth: popup.width - 2 * popup.padding
                            wrapMode: Text.Wrap
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }

                Column {
                    id: column
                    width: 200
                    height: 400

                    property string selectedTab: ""

                    Button {
                        id: northern_brew
                        text: qsTr("Northern Most Brewery")
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
                        text: qsTr("Southern Most Brewery")
                        highlighted: column.selectedTab === "southern"
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

            Item {
                id: mapTab
                Text {
                    anchors.centerIn: parent
                    text: qsTr("Map View (To be implemented)")
                    font.pixelSize: 16
                    color: "#ffffff"
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
                    id: refresh_data
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
