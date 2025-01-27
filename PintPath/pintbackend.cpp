#include "pintbackend.h"
#include <QDebug>
#include <QDesktopServices>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QNetworkReply>
#include <QSslSocket>

QDebug operator<<(QDebug debug, const vendorData &data)
{
    debug.nospace() << "id: " << data.id << "\nname: " << data.name
                    << "\nbrewery type: " << data.brewery_type << "\naddress 1: " << data.address_1
                    << "\naddress 2: " << data.address_2 << "\naddress 3: " << data.address_3
                    << "\ncity: " << data.city << "\nstate/province: " << data.state_province
                    << "\npost code: " << data.post_code << "\ncountry: " << data.country
                    << "\nlongitude: " << data.longitude << "\nlatitude: " << data.latitude
                    << "\nphone: " << data.phone << "\nWebsite URL: " << data.website_url;
    return debug;
}

void PintBackend::populateDatabase(const QByteArray &response)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(response);

    if (!jsonDoc.isArray()) {
        qCritical() << "Invalid JSON format: Expected an array";
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
            vendor.address_1 = obj.value("address_1").toString();
            vendor.address_2 = obj.value("address_2").toString();
            vendor.address_3 = obj.value("address_3").toString();
            vendor.city = obj.value("city").toString();
            vendor.state_province = obj.value("state_province").toString();
            vendor.post_code = obj.value("post_code").toString();
            vendor.country = obj.value("country").toString();

            QString longitude = obj.value("longitude").toString();
            vendor.longitude = longitude.toDouble();

            QString latitude = obj.value("latitude").toString();
            vendor.latitude = latitude.toDouble();

            vendor.phone = obj.value("phone").toString(); // keep as string to allow for +, - etc.
            vendor.website_url = obj.value("website_url")
                                     .toString(); //TODO:: you can make this a URL resource?
            vendorDatabase.push_back(vendor);
        }
    }
    qDebug() << "Database populated with" << vendorDatabase.size() << "entries.";
}

PintBackend::PintBackend()
    : QObject()
{
    qDebug() << "Device supports OpenSSL: " << QSslSocket::supportsSsl();
    connect(this, &PintBackend::apiResponseReceived, this, &PintBackend::handleApiResponse);
    connect(this, &PintBackend::apiErrorOccurred, this, &PintBackend::handleApiError);

    connect(&networkManager, &QNetworkAccessManager::finished, this, [this](QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            emit apiResponseReceived(responseData);
        } else {
            qCritical() << "API Error Occurred:" << reply->errorString();
            emit apiErrorOccurred(QString(reply->errorString()));
        }
        reply->deleteLater();
    });

    sendRequest("https://api.openbrewerydb.org/v1/breweries?by_country=ireland&per_page=200");
}

bool compareByNameLength(const vendorData &a, const vendorData &b)
{
    return a.name.length() < b.name.length();
}

//TODO:: probably a silly way to do this, what is a better way?, or suck it up and learn to use LAMBDAS
bool compareByHighestLatitude(const vendorData &a, const vendorData &b)
{
    return a.latitude
           < b.latitude; // Latitude (north or south) always precedes longitude (east or west)
}

bool compareByLowestlatitude(const vendorData &a, const vendorData &b)
{
    return a.latitude < b.latitude;
}

QVariantMap createVendorMap(const vendorData &data)
{
    QVariantMap vendorMap;
    vendorMap["id"] = data.id;
    vendorMap["name"] = data.name;
    vendorMap["phone"] = data.phone;
    vendorMap["address1"] = data.address_1 + " " + data.address_2 + " " + data.address_3;
    vendorMap["city"] = data.city;
    vendorMap["country"] = data.country;
    vendorMap["website_url"] = data.website_url;
    return vendorMap;
}

QVariant PintBackend::findNorthern()
{
    if (vendorDatabase.empty()) {
        return QVariant();
    }

    auto northern = std::max_element(vendorDatabase.begin(),
                                     vendorDatabase.end(),
                                     compareByHighestLatitude);

    if (northern != vendorDatabase.end()) {
        QVariantMap vendorMap;
        vendorMap = createVendorMap(*northern);
        return vendorMap;
    }
    return QVariant();
}

QVariant PintBackend::findSouthern()
{
    if (vendorDatabase.empty()) {
        return QVariant();
    }

    auto southern = std::min_element(vendorDatabase.begin(),
                                     vendorDatabase.end(),
                                     compareByLowestlatitude);

    if (southern != vendorDatabase.end()) {
        QVariantMap vendorMap;
        vendorMap = createVendorMap(*southern);
        return vendorMap;
    }
    return QVariant();
}

QVariant PintBackend::findLongestName()
{
    if (vendorDatabase.empty()) {
        return QVariant();
    }

    auto longest = std::max_element(vendorDatabase.begin(),
                                    vendorDatabase.end(),
                                    compareByNameLength);

    if (longest != vendorDatabase.end()) {
        QVariantMap vendorMap;
        vendorMap = createVendorMap(*longest);
        return vendorMap;
    }
    return QVariant();
}

void PintBackend::handleApiResponse(const QByteArray &response)
{
    populateDatabase(response);
}

void PintBackend::handleApiError(const QString &error)
{
    qCritical() << "Error message:" << error;
}

void PintBackend::sendRequest(const QString &endpoint)
{

    QUrl apiUrl(endpoint);
    QNetworkRequest request(apiUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    networkManager.get(request);
}

