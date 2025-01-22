#include "pintbackend.h"
#include <QDebug>

PintBackend::PintBackend()
    : QObject()
{
    // Define a basic route
    server.route("/", []() { return "Hello, world!"; });

    // Create a TCP server
    auto tcpserver = new QTcpServer(this);

    if (!tcpserver->listen(QHostAddress::Any, 8080)) {
        qCritical() << "Failed to start the TCP server!";
        delete tcpserver;
        return;
    }

    // Bind QTcpServer to QHttpServer
    if (!server.bind(tcpserver)) {
        qCritical() << "Failed to bind the HTTP server!";
        delete tcpserver;
        return;
    }

    qDebug() << "HTTP server is listening on port" << tcpserver->serverPort();
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
