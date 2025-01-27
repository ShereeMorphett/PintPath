#ifndef PINTBACKEND_H
#define PINTBACKEND_H

#include <QNetworkAccessManager>
#include <QObject>
#include "vendormodel.h"
#include <qqml.h>

struct vendorData
{
    QString id;
    QString name;
    QString brewery_type;
    QString address; //CONCATINATE ON PARSING
    QString address_1;
    QString address_2;
    QString address_3;
    QString city;
    QString state_province;
    QString post_code;
    QString country;
    double longitude;
    double latitude;
    QString phone;
    QString website_url;
};

QDebug operator<<(QDebug debug, const vendorData &data);

class PintBackend : public QObject
{
    Q_OBJECT
    QML_ELEMENT

public:
    PintBackend();

    Q_INVOKABLE void sendRequest(const QString &endpoint);

    Q_INVOKABLE QVariant findNorthern();
    Q_INVOKABLE QVariant findSouthern();
    Q_INVOKABLE QVariant findLongestName();
    Q_INVOKABLE QVariant findServesFood();
    Q_INVOKABLE VendorModel *getVendorModel() const { return m_vendorModel; }

signals:
    void apiResponseReceived(const QByteArray &response);
    void apiErrorOccurred(const QString &error);

    //TODO: Gain a better understanding of slots and signals
private slots:
    void handleApiResponse(const QByteArray &response);
    void handleApiError(const QString &error);

private:
    VendorModel *m_vendorModel;

    QNetworkAccessManager networkManager;

    void populateDatabase(const QByteArray &response);
    QVariantMap createVendorMap(QModelIndex modelIndex);
};

#endif // PINTBACKEND_H
