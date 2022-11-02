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

ETCPClient::~ETCPClient()
{
    qDebug()<<"[ETCPClient] --  Client say to goodBye.."<<mHostAdress<<"  "<<mPort;
    mTimer.stop();
    mSocket->close();

}

void ETCPClient::init()
{
    mSocket = new QTcpSocket(this);

    connect(mSocket, &QTcpSocket::readyRead,
            this,   [this](){ emit dataReceived(mSocket->readAll());});


    // Try to connect
    connect(&mTimer,&QTimer::timeout,this, [this](){
        if(mState != QAbstractSocket::SocketState::ConnectedState
                && mState != QAbstractSocket::SocketState::HostLookupState
                && mState != QAbstractSocket::SocketState::ConnectingState)
        {
            qDebug()<<"[ETCPClient] --  Try to connect"<<mHostAdress<<mPort;
            mSocket->connectToHost(mHostAdress, mPort);
        }
    });

    mTimer.start(500);


    connect(mSocket, &QTcpSocket::stateChanged,
            this,    [this](QAbstractSocket::SocketState pState)
    {
        qDebug()<<"[ETCPClient] --  State Changed!"<<pState<<"  "<<mHostAdress<<mPort;
        mState = pState;
        emit connectionStateChanged(mHostAdress,mPort,mState);
    });

}

bool ETCPClient::isConnected() const
{
    return mState == QAbstractSocket::SocketState::ConnectedState;
}

void ETCPClient::disconnectFromHost()
{
    mTimer.stop();
    mSocket->disconnectFromHost();
}

void ETCPClient::tryToConnect(QString pHostAdress, quint16 pPort)
{
    mHostAdress = pHostAdress;
    mPort       = pPort;

    mTimer.start(500);
}

void ETCPClient::writeData(const QByteArray &pData)
{
    mSocket->write(pData);
}



