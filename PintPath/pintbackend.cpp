#include "pintbackend.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QNetworkReply>
#include <QSslSocket>

// https://stackoverflow.com/questions/2677577/how-to-overload-operator-for-qdebug
QDebug &operator<<(QDebug &debug, const vendorData &data)
{
    debug << "id: " << data.id << "\nname: " << data.name << "\nbrewery type: " << data.brewery_type
          << "\naddress 1: " << data.address_1 << "\naddress 2: " << data.address_2
          << "\naddress 3: " << data.address_3 << "\ncity: " << data.city
          << "\nstate/province: " << data.state_province << "\npost code: " << data.post_code
          << "\ncountry: " << data.country << "\nlongitude: " << data.longitude
          << "\nlatitude: " << data.latitude << "\nphone: " << data.phone
          << "\nWebsite URL: " << data.website_url << "\n";

    return debug;
}

// QString id;
// QString name;
// QString brewery_type;
// QString address_1;
// QString address_2;
// QString address_3;
// QString city;
// QString state_province;
// QString post_code;
// QString country;
// double longitude;
// double latitude;
// QString phone;       //TODO could have that open the phone app to call?
// QString website_url; //TODO could have the hyperlink work?

void PintBackend::populateDatabase(const QByteArray &response)
{
    qDebug() << "Populating database";

    QJsonDocument jsonDoc = QJsonDocument::fromJson(response);

    if (!jsonDoc.isArray()) {
        qCritical() << "Invalid JSON format: Expected an array";
        // TODO: Add error handling or pop up an error
        return;
    }

    vendorDatabase.clear();

    QJsonArray jsonArray = jsonDoc.array();
    for (const QJsonValue &value : jsonArray) {
        if (value.isObject()) {
            QJsonObject obj = value.toObject();
            vendorData vendor;
            vendor.id = obj.value("id").toString();
            vendor.name = obj.value("name").toString();
            vendor.brewery_type = obj.value("brewery_type").toString();
            vendor.address_1 = obj.value("address_1").toString(); //TODO how to convert to QString
            vendor.address_2 = obj.value("address_2").toString();
            vendor.address_3 = obj.value("address_3").toString();
            vendor.city = obj.value("city").toString();
            vendor.state_province = obj.value("state_province").toString();
            vendor.post_code = obj.value("post_code").toString();
            vendor.country = obj.value("country").toString();
            vendor.longitude = obj.value("longitude")
                                   .toDouble(); //TODO:: this should be like STOD or something
            vendor.latitude = obj.value("latitude")
                                  .toDouble(); //TODO:: this should be like STOD or something
            vendor.phone = obj.value("phone").toString(); // keep as string to allow for +, - etc.
            vendor.website_url = obj.value("website_url").toString();

            // vendor.address = address;

            vendorDatabase.push_back(vendor);

            qDebug() << "Parsed vendor:" << vendor.name;
        }
    }

    qDebug() << "Database populated with" << vendorDatabase.size() << "entries.";
}

PintBackend::PintBackend()
    : QObject()
{
    //setting up ssl support
    qDebug() << "Device supports OpenSSL: " << QSslSocket::supportsSsl();
    currentCriteria = "test";
    connect(this, &PintBackend::apiResponseReceived, this, &PintBackend::handleApiResponse);
    connect(this, &PintBackend::apiErrorOccurred, this, &PintBackend::handleApiError);

    connect(&networkManager, &QNetworkAccessManager::finished, this, [this](QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            qDebug() << "API Response Received:" << QString(responseData);
            emit apiResponseReceived(currentCriteria, responseData);
        } else {
            qCritical() << "API Error Occurred:" << reply->errorString();
            emit apiErrorOccurred("refreshDatabase", QString(reply->errorString()));
        }
        reply->deleteLater();
    });

    sendRequest(
        "https://api.openbrewerydb.org/v1/breweries?by_country=ireland&per_page=200",
        "refreshDatabase"); //TODO: manage the limitation, what if there are more that 200 breweries in ireland?
}

vendorData PintBackend::findingNorthern()
{
    qDebug() << "Parsing response for Northern Most Brewery:";

    return vendorDatabase[0];
}

vendorData PintBackend::findingSouthern()
{
    qDebug() << "Parsing response for Southern Most Brewery:";
    return vendorDatabase[0];
}

vendorData PintBackend::findingLongestName()
{
    qDebug() << "Parsing response for Longest Name:";
    return vendorDatabase[0];
}

void PintBackend::handleApiResponse(const QString &criteria, const QByteArray &response)
{
    //TODO: the criteria may end up being obselte?
    qDebug() << "Handling response for criteria:" << criteria;

    if (criteria == "refreshDatabase") {
        currentCriteria = "refreshDatabase";
        qDebug() << "Parsing response for refreshDatabase:" << response;
        populateDatabase(response);
    } else {
        qWarning() << "Unknown criteria:" << criteria;
    }
}

void PintBackend::handleApiError(const QString &criteria, const QString &error)
{
    qCritical() << "Error occurred for criteria:" << criteria;
    qCritical() << "Error message:" << error;
}

void PintBackend::sendRequest(const QString &endpoint, const QString &criteria)
{

    QUrl apiUrl(endpoint);
    currentCriteria = criteria;
    QNetworkRequest request(apiUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    networkManager.get(request);
}

bool PintBackend::isWorking() const
{
    qDebug() << "Is working:  " << m_isWorking;
    return m_isWorking;
}

void PintBackend::setIsWorking(const bool &newIsWorking)
{
    if (m_isWorking == newIsWorking)
        return;
    m_isWorking = newIsWorking;
    emit isWorkingChanged();
}
