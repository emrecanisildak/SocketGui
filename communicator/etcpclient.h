#ifndef ETCPCLIENT_H
#define ETCPCLIENT_H

#include <QObject>
#include <QTimer>
#include <QAbstractSocket>

class QTcpSocket;

class ETCPClient : public QObject
{
    Q_OBJECT
public:
    explicit ETCPClient(QString pHostAdress, quint16 pPort,QObject *parent = nullptr);
    virtual ~ETCPClient();

    void init();
    bool isConnected() const;
    void disconnectFromHost();
    void tryToConnect(QString pHostAdress, quint16 pPort);

signals:
    void dataReceived(const QByteArray& pData);
    void connectionStateChanged(const QString& ip, uint16_t port, QAbstractSocket::SocketState state);

public slots:
    void writeData(const QByteArray& pData);


private:
    QTcpSocket*         mSocket;
    QTimer              mTimer;
    QString             mHostAdress;
    quint16             mPort;
    QAbstractSocket::SocketState mState;
};

#endif // ETCPCLIENT_H
