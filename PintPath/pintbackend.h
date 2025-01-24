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

    Q_INVOKABLE void sendRequest(const QString &endpoint, const QString &criteria);

signals:
    void isWorkingChanged();
    void apiResponseReceived(const QString &criteria, const QString &response);
    void apiErrorOccurred(const QString &criteria, const QString &error);

    //TODO: Gain a better understanding of slots and signals
private slots:
    void handleApiResponse(const QString &criteria, const QString &response);
    void handleApiError(const QString &criteria, const QString &error);

private:
    bool m_isWorking;
    QNetworkAccessManager networkManager;
    QString currentCriteria;
};

#endif // PINTBACKEND_H
