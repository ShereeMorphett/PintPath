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
    currentCriteria = "test";
    connect(this, &PintBackend::apiResponseReceived, this, &PintBackend::handleApiResponse);
    connect(this, &PintBackend::apiErrorOccurred, this, &PintBackend::handleApiError);

    connect(&networkManager, &QNetworkAccessManager::finished, this, [this](QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            qDebug() << "API Response Received:" << QString(responseData);
            emit apiResponseReceived(currentCriteria, QString(responseData));
        } else {
            qCritical() << "API Error Occurred:" << reply->errorString();
            emit apiErrorOccurred("test", QString(reply->errorString()));
        }
        reply->deleteLater();
    });

    // sendRequest("https://api.openbrewerydb.org/v1/breweries/random", "test");
}

void findingNorthern(const QString &response)
{
    qDebug() << "Parsing response for Northern Most Brewery:" << response;
}

void findingSouthern(const QString &response)
{
    qDebug() << "Parsing response for Southern Most Brewery:" << response;
}

void findingLongestName(const QString &response)
{
    qDebug() << "Parsing response for Longest Name:" << response;
}

void PintBackend::handleApiResponse(const QString &criteria, const QString &response)
{
    qDebug() << "Handling response for criteria:" << criteria;
    if (criteria == "northern") {
        currentCriteria = "northern";
        // Logic for "Northern Most Brewery"
        findingNorthern(response);
    } else if (criteria == "southern") {
        currentCriteria = "southern";
        // Logic for "Southern Most Brewery"
        findingSouthern(response);
    } else if (criteria == "longestName") {
        currentCriteria = "longestName";
        findingLongestName(response);

    } else if (criteria == "test") {
        currentCriteria = "test";
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
