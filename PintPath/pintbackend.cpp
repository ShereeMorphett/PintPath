#include "pintbackend.h"
#include <QDebug>
#include <QDesktopServices>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QNetworkReply>
#include <QSslSocket>

PintBackend::PintBackend()
    : QObject()
    , m_vendorModel(new VendorModel(this))
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

void PintBackend::populateDatabase(const QByteArray &response)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(response);

    if (!jsonDoc.isArray()) {
        qCritical() << "Invalid JSON format: Expected an array";
        return;
    }

    QJsonArray jsonArray = jsonDoc.array();
    QList<vendorData> vendors;

    for (const QJsonValue &value : jsonArray) {
        if (value.isObject()) {
            QJsonObject obj = value.toObject();
            vendorData vendor;
            vendor.id = obj.value("id").toString();
            vendor.name = obj.value("name").toString();
            vendor.breweryType = obj.value("brewery_type").toString();
            vendor.address = obj.value("address_1").toString() + " "
                             + obj.value("address_2").toString() + " "
                             + obj.value("address_3").toString();
            vendor.city = obj.value("city").toString();
            vendor.stateProvince = obj.value("state_province").toString();
            vendor.postcode = obj.value("post_code").toString();
            vendor.country = obj.value("country").toString();
            vendor.longitude = obj.value("longitude").toString().toDouble();
            vendor.latitude = obj.value("latitude").toString().toDouble();
            vendor.phone = obj.value("phone").toString();
            vendor.websiteUrl = obj.value("website_url").toString();

            vendors.append(vendor);
        }
    }
    m_vendorModel->addVendors(vendors);
}

QVariantMap PintBackend::createVendorMap(QModelIndex modelIndex)
{
    QVariantMap vendor;
    vendor["id"] = m_vendorModel->data(modelIndex, VendorModel::IdRole);
    vendor["name"] = m_vendorModel->data(modelIndex, VendorModel::NameRole);
    vendor["breweryType"] = m_vendorModel->data(modelIndex, VendorModel::BreweryTypeRole);
    vendor["address"] = m_vendorModel->data(modelIndex, VendorModel::AddressRole);
    vendor["city"] = m_vendorModel->data(modelIndex, VendorModel::CityRole);
    vendor["stateProvince"] = m_vendorModel->data(modelIndex, VendorModel::StateProvinceRole);
    vendor["postCode"] = m_vendorModel->data(modelIndex, VendorModel::PostcodeRole);
    vendor["country"] = m_vendorModel->data(modelIndex, VendorModel::CountryRole);
    vendor["longitude"] = m_vendorModel->data(modelIndex, VendorModel::LongitudeRole);
    vendor["latitude"] = m_vendorModel->data(modelIndex, VendorModel::LatitudeRole);
    vendor["phone"] = m_vendorModel->data(modelIndex, VendorModel::PhoneRole);
    vendor["websiteUrl"] = m_vendorModel->data(modelIndex, VendorModel::WebsiteUrlRole);
    return vendor;
}

QVariant PintBackend::findServesFood()
{
    if (m_vendorModel->rowCount() == 0) {
        qWarning() << "VendorModel is empty.";
        return QVariant();
    }

    QList<QVariantMap> vendors;

    for (int i = 0; i < m_vendorModel->rowCount(); ++i) {
        QModelIndex modelIndex = m_vendorModel->index(i, 0);
        QString breweryType = m_vendorModel->data(modelIndex, VendorModel::BreweryTypeRole)
                                  .toString();
        if (breweryType == "brewpub") {
            QVariantMap vendor = createVendorMap(modelIndex);
            vendors.append(vendor);
        }
    }
    qDebug() << vendors.size();
    return QVariant::fromValue(vendors);
}

QVariant PintBackend::findNorthern()
{
    if (m_vendorModel->rowCount() == 0) {
        qWarning() << "VendorModel is empty.";
        return QVariant();
    }

    double maxLatitude = -90.0;
    int index = -1;
    for (int i = 0; i < m_vendorModel->rowCount(); ++i) {
        QModelIndex modelIndex = m_vendorModel->index(i, 0);
        double latitude = m_vendorModel->data(modelIndex, VendorModel::LatitudeRole).toDouble();
        if (latitude > maxLatitude) {
            maxLatitude = latitude;
            index = i;
        }
    }

    if (index != -1) {
        QModelIndex modelIndex = m_vendorModel->index(index, 0);
        QVariantMap vendor = createVendorMap(modelIndex);
        return vendor;
    }

    return QVariant();
}

QVariant PintBackend::findSouthern()
{
    if (m_vendorModel->rowCount() == 0) {
        qWarning() << "VendorModel is empty.";
        return QVariant();
    }

    double minLatitude = 91.0;
    int index = -1;
    for (int i = 0; i < m_vendorModel->rowCount(); ++i) {
        QModelIndex modelIndex = m_vendorModel->index(i, 0);
        double latitude = m_vendorModel->data(modelIndex, VendorModel::LatitudeRole).toDouble();
        if (latitude < minLatitude) {
            minLatitude = latitude;
            index = i;
        }
    }

    if (index != -1) {
        QModelIndex modelIndex = m_vendorModel->index(index, 0);
        QVariantMap vendor = createVendorMap(modelIndex);
        return vendor;
    }

    return QVariant();
}

QVariant PintBackend::findLongestName()
{
    if (m_vendorModel->rowCount() == 0) {
        qWarning() << "VendorModel is empty.";
        return QVariant();
    }

    int longestNameLength = -1;
    int index = -1;

    for (int i = 0; i < m_vendorModel->rowCount(); ++i) {
        QModelIndex modelIndex = m_vendorModel->index(i, 0);
        QString name = m_vendorModel->data(modelIndex, VendorModel::NameRole).toString();
        if (name.length() > longestNameLength) {
            longestNameLength = name.length();
            index = i;
        }
    }

    if (index != -1) {
        QModelIndex modelIndex = m_vendorModel->index(index, 0);
        QVariantMap vendor = createVendorMap(modelIndex);
        return vendor;
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
