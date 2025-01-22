#ifndef PINTBACKEND_H
#define PINTBACKEND_H

#include <QNetworkAccessManager>
#include <QObject>
#include <qqml.h>

class PintBackend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isWorking READ isWorking WRITE setIsWorking NOTIFY isWorkingChanged FINAL)
    QML_ELEMENT

public:
    PintBackend();
    bool isWorking() const;
    void setIsWorking(const bool &newIsWorking);

    void sendRequest(); // Method to send a GET request

signals:
    void isWorkingChanged();

    void apiResponseReceived(const QString &response);
    void apiErrorOccurred(const QString &error);

private:
    bool m_isWorking;
    QNetworkAccessManager networkManager;
};

#endif // PINTBACKEND_H
