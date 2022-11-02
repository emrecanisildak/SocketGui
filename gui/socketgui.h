#ifndef SOCKETGUI_H
#define SOCKETGUI_H

#include <QWidget>

enum class SocketType{
    SERVER = 0,
    CLIENT = 1
};

enum class Errors{
    EMPTY_IP,
    EMPTY_PORT,
    EMPTY_IP_PORT,
    INVALID_IP,
    SOCKET_ERRORS
};

namespace Ui {
class SocketGui;
}

class ETCPClient;
class ETCPServer;

class SocketGui : public QWidget
{
    Q_OBJECT

public:
    explicit SocketGui(uint32_t mId,QWidget *parent = nullptr);
    ~SocketGui();

    uint32_t uniqueId()const;


signals:
    void closeButtonClicked(uint32_t pUniqueId);



private:
    Ui::SocketGui *ui;
    uint32_t mUniqueId;

    ETCPClient* mClient = nullptr;
    ETCPServer* mServer = nullptr;

    SocketType mSocketType = SocketType::SERVER;

    bool mConnectionButtonState = false;

    bool connectionButtonState()const;
    void toggleConnectionButtonState();



    void initGui();
    void setConnection(bool isConnected);
    void setSocketType(SocketType pSocketType);

    QString ipPlaceHolderText()const;
};

#endif // SOCKETGUI_H
