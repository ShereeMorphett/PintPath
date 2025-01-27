#include "vendormodel.h"
#include "pintbackend.h"

VendorModel::VendorModel(QObject *parent)
    : QAbstractItemModel(parent)
{}

QModelIndex VendorModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() || row < 0 || column < 0 || row >= vendorDataList.size() || column >= 1) {
        return QModelIndex();
    }
    return createIndex(row, column);
}

QModelIndex VendorModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);
    return QModelIndex(); // Flat model, no parent-child relationships
}

int VendorModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1; // Single column for a flat list
}

void VendorModel::addVendor(const vendorData &vendor)
{
    beginInsertRows(QModelIndex(), vendorDataList.size(), vendorDataList.size());
    vendorDataList.append(vendor);
    endInsertRows();
}

void VendorModel::addVendors(const QList<vendorData> vendors)
{
    qDebug() << "In addVendors: ", vendors.size();
    if (vendors.isEmpty()) {
        qWarning() << "Attempted to add an empty vendors list!";
        return;
    }
    beginInsertRows(QModelIndex(),
                    vendorDataList.size(),
                    vendorDataList.size() + vendors.size() - 1);
    vendorDataList.append(vendors);
    endInsertRows();
    qDebug() << "out addVendors";
}

// Required overrides
QVariant VendorModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= vendorDataList.size())
        return QVariant();

    const vendorData &vendor = vendorDataList[index.row()];

    switch (role) {
    case IdRole:
        return vendor.id;
    case NameRole:
        return vendor.name;
    case BreweryTypeRole:
        return vendor.brewery_type;
    case AddressRole:
        return vendor.address; /* + " " + vendor.address_2 + " " + vendor.address_3;*/
    case CityRole:
        return vendor.city;
    case StateProvinceRole:
        return vendor.state_province;
    case PostcodeRole:
        return vendor.post_code;
    case CountryRole:
        return vendor.country;
    case LongitudeRole:
        return vendor.longitude;
    case LatitudeRole:
        return vendor.latitude;
    case PhoneRole:
        return vendor.phone;
    case WebsiteUrlRole:
        return vendor.website_url;
    default:
        return QVariant();
    }
}

int VendorModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return vendorDataList.size();
}

QHash<int, QByteArray> VendorModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[NameRole] = "name";
    roles[BreweryTypeRole] = "breweryType";
    roles[AddressRole] = "address";
    roles[CityRole] = "city";
    roles[StateProvinceRole] = "stateProvince";
    roles[PostcodeRole] = "postCode";
    roles[CountryRole] = "country";
    roles[LongitudeRole] = "longitude";
    roles[LatitudeRole] = "latitude";
    roles[PhoneRole] = "phone";
    roles[WebsiteUrlRole] = "websiteUrl";
    return roles;
}
