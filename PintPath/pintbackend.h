#ifndef PINTBACKEND_H
#define PINTBACKEND_H

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

signals:
    void isWorkingChanged();

private:
    bool m_isWorking;
};

#endif // PINTBACKEND_H
