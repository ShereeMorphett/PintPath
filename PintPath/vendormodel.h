#ifndef VENDORMODEL_H
#define VENDORMODEL_H

#include <QAbstractItemModel>
#include <QList>
#include <QtCore/QObject>

struct vendorData; //may not be needed

class VendorModel : public QAbstractItemModel
{
    Q_OBJECT

    enum VendorRole {
        IdRole = Qt::UserRole + 1,
        NameRole,
        BreweryTypeRole,
        AddressRole,
        CityRole,
        StateProvinceRole,
        PostcodeRole,
        CountryRole,
        LongitudeRole,
        LatitudeRole,
        PhoneRole,
        WebsiteUrlRole
    };

public:
    VendorModel(QObject *parent);

    // Required implementations
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QHash<int, QByteArray> roleNames() const override;

    void addVendor(const vendorData &vendor);
    void addVendors(const QList<vendorData> vendors);

private:
    QList<vendorData> vendorDataList;

    friend class PintBackend; //TODO:: MAY NOT NEED THIS
};

#endif // VENDORMODEL_H
