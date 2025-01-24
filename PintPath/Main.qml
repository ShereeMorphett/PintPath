import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15
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
        anchors.leftMargin: 0
        anchors.rightMargin: 0
        anchors.topMargin: 0
        anchors.bottomMargin: 0

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
                Layout.alignment: Qt.AlignHCenter // Center within the layout
                Layout.fillWidth: true // Evenly distribute across the width
            }

            TabButton {
                text: qsTr("Map")
                spacing: 10
                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: true
            }
            TabButton {
                text: qsTr("About")
                spacing: 10
                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: true
            }

            onCurrentIndexChanged: {
                console.log("Current tab changed to:", currentIndex)
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
                            backendManager.isWorking
                        }
                    }

                    Button {
                        id: southern_brew
                        text: qsTr("Southern Most Brewery")
                        highlighted: false
                        flat: false
                    }

                    Button {
                        id: longest_name
                        text: qsTr("Longest Name")
                        highlighted: false
                        flat: false
                        onClicked: {
                            backendManager.sendRequest(
                                        "https://api.openbrewerydb.org/v1/breweries?by_country=ireland",
                                        "Longest Name")
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
                }
            }

            Item {
                id: aboutTab
                visible: bar.currentIndex === 2
                Text {
                    anchors.centerIn: parent
                    text: qsTr("About PintPath")
                    font.pixelSize: 18
                    font.bold: true
                }
            }
        }
    }
}
