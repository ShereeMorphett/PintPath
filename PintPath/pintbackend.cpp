#include "pintbackend.h"
#include <QDebug>

PintBackend::PintBackend() {}

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
