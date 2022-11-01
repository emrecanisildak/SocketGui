#include "etcpserver.h"

// Qt libs..
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>

ETCPServer::ETCPServer(QString pHostAdress, quint16 pPort, QObject *parent):
    QObject(parent),
    mHostAdress(pHostAdress),
    mPort(pPort)
{


}

ETCPServer::~ETCPServer()
{
    qDebug()<<"Close TCP Server"<<mHostAdress<<"   "<<mPort;
    mServer->close();
    std::for_each(mSockets.begin(),mSockets.end(),std::default_delete<QTcpSocket>());
}

void ETCPServer::init()
{
    mServer = new QTcpServer(this);
    connect(mServer, &QTcpServer::newConnection,this, &ETCPServer::onNewConnection);

    QHostAddress addr;
    if (addr.setAddress(mHostAdress))
    {
        mServer->listen(addr, mPort);

        qInfo() <<"Tcp Server listen addr:" << mHostAdress
               << "port:" << mPort;
    }
    else
    {
        mServer->listen(QHostAddress::Any, mPort);

        qInfo() <<"Tcp Server listen addr: Ip::Any"
               << "port:" << mPort;
    }
}

bool ETCPServer::hasConnection() const
{
    return mSockets.size();
}

void ETCPServer::writeData(const QByteArray &pArray)
{
    for (QTcpSocket* socket : qAsConst(mSockets)) {
        socket->write(pArray);
    }
}

void ETCPServer::onNewConnection()
{
    while (mServer->hasPendingConnections())
    {
        QTcpSocket *socket = mServer->nextPendingConnection();
        mSockets.push_back(socket);

        // Ready Read
        connect(socket, &QIODevice::readyRead, this, &ETCPServer::onReadyRead);

        // State Changed
        connect(socket, &QTcpSocket::stateChanged, this, [this](QAbstractSocket::SocketState pState)
        {
            QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());

            if(pState == QAbstractSocket::ConnectedState)
            {
                qDebug()<<"Ip: "<<sender->peerAddress()
                       <<"Port: "<<sender->peerPort()
                      <<"connected.";

            }
            else if(pState == QAbstractSocket::ListeningState)
            {
                qDebug()<<"Ip: "<<sender->peerAddress()
                       <<"Port: "<<sender->peerPort()
                      <<"listening.";
            }

            else if (pState == QAbstractSocket::UnconnectedState)
            {
                qDebug()<<"Ip: "<<sender->peerAddress()
                       <<"Port: "<<sender->peerPort()
                      <<"disconnected.";

                mSockets.removeOne(sender);
            }

            connectionStateChanged(mHostAdress,mPort,pState);

        });

        // Error Occured
        connect(socket, &QAbstractSocket::errorOccurred,this, [this](QAbstractSocket::SocketError)
        {
            QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
            qDebug()<<"Ip: "<<sender->peerAddress()<<
                      "Port: "<<sender->peerPort()<<
                      "error occured. Error["<<
                      sender->errorString()<<"]";
        });


    }

}

void ETCPServer::onReadyRead()
{
    QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
    emit dataReceived(sender->readAll());
}
