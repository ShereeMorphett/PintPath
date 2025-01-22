#include "pintbackend.h"
#include <QDebug>
#include <QNetworkReply>

//TODO: ANDROID INTERNET PERMISSION
PintBackend::PintBackend()
    : QObject()
{
    // Connect to the manager's finished signal to process responses
    connect(&networkManager, &QNetworkAccessManager::finished, this, [this](QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            qDebug() << "API Response Received:" << QString(responseData);
            emit apiResponseReceived(QString(responseData));
        } else {
            qCritical() << "API Error Occurred:" << reply->errorString();
            emit apiErrorOccurred(reply->errorString());
        }
        reply->deleteLater();
    });

    sendRequest();
}

void PintBackend::sendRequest()
{
    QUrl apiUrl("https://api.openbrewerydb.org/v1/breweries/random");

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
