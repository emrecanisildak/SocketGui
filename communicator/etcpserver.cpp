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
    qDebug()<<"[ETCPServer] --  Close TCP Server"<<mHostAdress<<"   "<<mPort;
    mServer->close();
    std::for_each(mSockets.begin(),mSockets.end(),std::default_delete<QTcpSocket>());
}

void ETCPServer::init()
{
    mServer = new QTcpServer(this);
    connect(mServer, &QTcpServer::newConnection ,this,&ETCPServer::onNewConnection);

    QHostAddress addr;
    if (addr.setAddress(mHostAdress))
    {
        mServer->listen(addr, mPort);

        qInfo() <<"[ETCPServer] --   Listening..." << mHostAdress
               << " " << mPort;
    }
    else
    {
        mServer->listen(QHostAddress::Any, mPort);

        qInfo() <<"[ETCPServer] --  Listening Any IP..."
               << "  " << mPort<<"(Not valid IP Adress may be setted invalid ip!!)";
    }
}

bool ETCPServer::hasConnection() const
{
    return mSockets.size();
}

void ETCPServer::closeHostService()
{
    mServer->close();
}

void ETCPServer::listenClient(QString pHostAdress, quint16 pPort)
{
    mHostAdress = pHostAdress;
    mPort = pPort;

    QHostAddress addr;
    if (addr.setAddress(mHostAdress))
    {
        mServer->listen(addr, mPort);

        qInfo() <<"[ETCPServer] --   Listening..." << mHostAdress
               << " " << mPort;
    }
    else
    {
        mServer->listen(QHostAddress::Any, mPort);

        qInfo() <<"[ETCPServer] --  Listening Any IP..."
               << "  " << mPort<<"(Not valid IP Adress may be setted invalid ip!!)";
    }
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

            qDebug()<<"[ETCPServer] -- state changed "<<pState
                   <<sender->peerAddress()<<" "<<
                     sender->peerPort();

            if (pState == QAbstractSocket::UnconnectedState)
                mSockets.removeOne(sender);

            emit connectionStateChanged(mHostAdress,mPort,pState);

        });

        // Error Occured
        connect(socket, &QAbstractSocket::errorOccurred,this, [this](QAbstractSocket::SocketError)
        {
            QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
            qDebug()<<"[ETCPServer] -- Error occured "<<sender->peerAddress()<<
                      " "<<sender->peerPort()<<
                      sender->errorString();
        });
    }

}

void ETCPServer::onReadyRead()
{
    QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
    emit dataReceived(sender->readAll());
}
