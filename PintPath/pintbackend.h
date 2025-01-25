#ifndef PINTBACKEND_H
#define PINTBACKEND_H

#include <QNetworkAccessManager>
#include <QObject>
#include <qqml.h>

struct vendorData
{
    QString id;
    QString name;
    QString brewery_type;
    QString address_1;
    QString address_2;
    QString address_3;
    QString city;
    QString state_province;
    QString post_code;
    QString country;
    // QString longitude;
    // QString latitude;
    double longitude;
    double latitude;
    QString phone;       //TODO could have that open the phone app to call?
    QString website_url; //TODO could have the hyperlink work?
};

// https://stackoverflow.com/questions/2677577/how-to-overload-operator-for-qdebug
QDebug &operator<<(QDebug &debug, const vendorData &data);

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

signals:
    void isWorkingChanged();
    void apiResponseReceived(const QByteArray &response);
    void apiErrorOccurred(const QString &error);

    //TODO: Gain a better understanding of slots and signals
private slots:
    void handleApiResponse(const QByteArray &response);
    void handleApiError(const QString &error);

private:
    std::vector<vendorData> vendorDatabase;
    bool m_isWorking;
    QNetworkAccessManager networkManager;

    void populateDatabase(const QByteArray &response);
};

#endif // PINTBACKEND_H
