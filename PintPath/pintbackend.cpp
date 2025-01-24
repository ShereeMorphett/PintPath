#include "pintbackend.h"
#include <QDebug>
#include <QNetworkReply>
#include <QSslSocket>

//TODO: ANDROID INTERNET PERMISSION
PintBackend::PintBackend()
    : QObject()
{
    //setting up ssl support
    qDebug() << "Device supports OpenSSL: " << QSslSocket::supportsSsl();

    connect(this, &PintBackend::apiResponseReceived, this, &PintBackend::handleApiResponse);
    connect(this, &PintBackend::apiErrorOccurred, this, &PintBackend::handleApiError);

    connect(&networkManager, &QNetworkAccessManager::finished, this, [this](QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            qDebug() << "API Response Received:" << QString(responseData);
            emit apiResponseReceived("test", QString(responseData));
        } else {
            qCritical() << "API Error Occurred:" << reply->errorString();
            emit apiErrorOccurred("test", QString(reply->errorString()));
        }
        reply->deleteLater();
    });

    sendRequest("https://api.openbrewerydb.org/v1/breweries/random", "Random Brewery");
}

// TODO: switch case
void PintBackend::handleApiResponse(const QString &criteria, const QString &response)
{
    qDebug() << "Handling response for criteria:" << criteria;

    if (criteria == "northern") {
        // Logic for "Northern Most Brewery"
        qDebug() << "Parsing response for Northern Most Brewery:" << response;
    } else if (criteria == "southern") {
        // Logic for "Southern Most Brewery"
        qDebug() << "Parsing response for Southern Most Brewery:" << response;
    } else if (criteria == "longestName") {
        // Logic for "Longest Name"
        qDebug() << "Parsing response for Longest Name:" << response;
    } else if (criteria == "test") {
        // Logic for "Longest Name"
        qDebug() << "Parsing response for test:" << response;
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
    // Store the criteria for this request
    currentCriteria = criteria;

    QUrl apiUrl(endpoint);

    QNetworkRequest request(apiUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    networkManager.get(request);

    qDebug() << "Request sent to" << apiUrl.toString() << "with criteria" << criteria;
}

void PintBackend::sendGetNameRequest()
{
    qDebug() << "Sending Name Request";
    QUrl apiUrl("https://api.openbrewerydb.org/v1/breweries?by_country=ireland");

    QNetworkRequest request(apiUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    networkManager.get(request);

    qDebug() << "Request sent to" << apiUrl.toString();
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
