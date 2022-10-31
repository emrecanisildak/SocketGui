#include "etcpclient.h"

// Qt Libs..
#include <QTcpSocket>

ETCPClient::ETCPClient(QString pHostAdress, quint16 pPort, QObject *parent)
    : QObject{parent},
      mHostAdress{pHostAdress},
      mPort{pPort}
{
      mState = QAbstractSocket::SocketState::UnconnectedState;
}

void ETCPClient::init()
{
    mSocket = new QTcpSocket(this);

    connect(mSocket, &QTcpSocket::readyRead,
            this,   [this](){ emit dataReceived(mSocket->readAll());});


    // Try to connect
    connect(&mTimer,&QTimer::timeout,this, [this](){
        if(mState != QAbstractSocket::SocketState::ConnectedState)
            mSocket->connectToHost(mHostAdress, mPort);
    });

    mTimer.start(500);




    connect(mSocket, &QTcpSocket::stateChanged,
            this,    [this](QAbstractSocket::SocketState pState)
    {
        mState = pState;
    });

}

bool ETCPClient::isConnected() const
{
    return mState == QAbstractSocket::SocketState::ConnectedState;
}

void ETCPClient::writeData(const QByteArray &pData)
{
    mSocket->write(pData);
}



