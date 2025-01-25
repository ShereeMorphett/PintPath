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
    Q_PROPERTY(bool isWorking READ isWorking WRITE setIsWorking NOTIFY isWorkingChanged FINAL)
    QML_ELEMENT

public:
    PintBackend();
    bool isWorking() const;
    void setIsWorking(const bool &newIsWorking);

    Q_INVOKABLE void sendRequest(const QString &endpoint, const QString &criteria);

    vendorData findingNorthern();
    vendorData findingSouthern();
    vendorData findingLongestName();

signals:
    void isWorkingChanged();
    void apiResponseReceived(const QString &criteria, const QByteArray &response);
    void apiErrorOccurred(const QString &criteria, const QString &error);

    //TODO: Gain a better understanding of slots and signals
private slots:
    void handleApiResponse(const QString &criteria, const QByteArray &response);
    // void handleApiResponse(const QString &criteria, const QString &response);
    void handleApiError(const QString &criteria, const QString &error);

private:
    std::vector<vendorData> vendorDatabase;
    bool m_isWorking;
    QNetworkAccessManager networkManager;
    QString currentCriteria;

    void populateDatabase(const QByteArray &response);
};

#endif // PINTBACKEND_H
