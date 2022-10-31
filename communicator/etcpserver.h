#ifndef ETCPSERVER_H
#define ETCPSERVER_H

#include <QObject>
#include <QHash>

class QTcpServer;
class QTcpSocket;

class ETCPServer : public QObject
{
public:
    explicit ETCPServer(QString pHostAdress, quint16 pPort, QObject* parent = nullptr);
    void init();
    bool hasConnection() const;

signals:
    void dataReceived(const QByteArray& pData);
    void connectionStateChanged(const QString& ip, uint16_t port, int state);

public slots:
    void writeData(const QByteArray& pData);

private slots:
    void onNewConnection();
    void onReadyRead();


private:
    QString             mHostAdress;
    quint16             mPort;
    QTcpServer*         mServer;
    QList<QTcpSocket*>  mSockets;

};

#endif // ETCPSERVER_H
