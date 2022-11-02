#include "socketgui.h"
#include "ui_socketgui.h"

#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

#include "communicator/etcpclient.h"
#include "communicator/etcpserver.h"

SocketGui::SocketGui(uint32_t mId, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SocketGui),
    mUniqueId(mId)
{
    ui->setupUi(this);

    initGui();

    setConnection(false);

}

SocketGui::~SocketGui()
{
    qDebug()<<"Socket Gui close"<<mUniqueId;

    if(mClient)
        delete mClient;

    if(mServer)
        delete mServer;

    delete ui;
}

uint32_t SocketGui::uniqueId() const
{
    return mUniqueId;
}

bool SocketGui::connectionButtonState() const
{
    return mConnectionButtonState;
}

void SocketGui::toggleConnectionButtonState()
{
    mConnectionButtonState = !mConnectionButtonState;

    if(mConnectionButtonState)
    {
        // Connect State ------------------------
        ui->radioButtonClient->setDisabled(true);
        ui->radioButtonServer->setDisabled(true);
        ui->buttonConnect->setText(tr("Close"));

    }
    else
    {
        ui->buttonConnect->setText(tr("Connect"));
        ui->radioButtonClient->setDisabled(false);
        ui->radioButtonServer->setDisabled(false);
    }
}

void SocketGui::initGui()
{
    // Clsoe Button
    ui->buttonClose->setFixedSize(50,50);
    ui->buttonClose->setIcon(QIcon(":/icons/CLOSE.png"));
    ui->buttonClose->setIconSize(QSize(50,50));
    ui->buttonClose->setToolTip(tr("Close"));
    ui->buttonClose->setStyleSheet("QPushButton{background-color:rgba(0,0,0,0); } QPushButton:hover{border:1px solid #ff0000; border-radius:5px;}");


    // Signal Slots..
    connect(ui->buttonClose, &QPushButton::clicked, this, [this](bool checked){
        emit closeButtonClicked(mUniqueId);
    });

    connect(ui->radioButtonServer, &QRadioButton::clicked,[this](bool checked){
        mSocketType =  SocketType::SERVER;
        ui->lineEdit_ip->setPlaceholderText(ipPlaceHolderText());
    });

    connect(ui->radioButtonClient, &QRadioButton::clicked,[this](bool checked){
        mSocketType =  SocketType::CLIENT;
        ui->lineEdit_ip->setPlaceholderText(ipPlaceHolderText());
    });

    connect(ui->buttonConnect, &QPushButton::clicked, [this](bool checked)
    {
        toggleConnectionButtonState();

        if(connectionButtonState()){
            if(mSocketType ==  SocketType::CLIENT)
            {
                auto ip = ui->lineEdit_ip->text();
                auto port = ui->lineEdit_port->text();

                // Client
                if(mClient == nullptr){

                    mClient = new ETCPClient(ip,port.toInt());
                    mClient->init();
                    connect(mClient, &ETCPClient::connectionStateChanged,this,[this](const QString& ip, uint16_t port, QAbstractSocket::SocketState state)
                    {
                        if(state == QAbstractSocket::SocketState::ConnectedState)
                            setConnection(true);
                        else
                            setConnection(false);

                    });
                }
                // Client ikinci ilklenmesi
                else{
                    mClient->tryToConnect(ip,port.toInt());
                }



            }
            else
            {
                auto ip = ui->lineEdit_ip->text();
                auto port = ui->lineEdit_port->text();

                // Server ilk defa ilkleniyorsa
                if(mServer == nullptr){
                    mServer = new ETCPServer(ip,port.toInt());
                    mServer->init();


                    connect(mServer, &ETCPServer::connectionStateChanged,this,[this](const QString& ip, uint16_t port, QAbstractSocket::SocketState state)
                    {
                        if(state == QAbstractSocket::SocketState::ConnectedState)
                            setConnection(true);
                        else
                            setConnection(false);
                    });
                }
                // Server 2. defa ilkleniyorsa
                else
                {
                    mServer->listenClient(ip,port.toInt());
                }
            }
        }

        // NOT CONNECTION STATE -------------------
        else{

            if(mSocketType ==  SocketType::CLIENT)
            {
                //
                mClient->disconnectFromHost();
            }
            else{
                mServer->closeHostService();
            }


        }

    });



    QString IpRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegularExpression IpRegex ("^" + IpRange
                                + "(\\." + IpRange + ")"
                                + "(\\." + IpRange + ")"
                                + "(\\." + IpRange + ")$");
    QRegularExpressionValidator *ipValidator = new QRegularExpressionValidator(IpRegex, this);

    QRegularExpression portRegex("^()([1-9]|[1-5]?[0-9]{2,4}|6[1-4][0-9]{3}|65[1-4][0-9]{2}|655[1-2][0-9]|6553[1-5])$");
    QRegularExpressionValidator* portValidator = new QRegularExpressionValidator(portRegex,this);


    // IP Line edit...
    ui->lineEdit_ip->setValidator(ipValidator);
    ui->lineEdit_ip->setFixedWidth(100);
    ui->lineEdit_ip->setPlaceholderText(ipPlaceHolderText());


    // Port Line edit
    ui->lineEdit_port->setValidator(portValidator);
    ui->lineEdit_port->setFixedWidth(80);
    ui->lineEdit_port->setPlaceholderText(tr("Port"));

    // Radio Button
    ui->radioButtonServer->setChecked(mSocketType == SocketType::SERVER);
    ui->radioButtonClient->setChecked(mSocketType == SocketType::CLIENT);




    // Size
    ui->buttonConnect->setFixedWidth(50);
    ui->labelConnection->setFixedSize(50,50);


}

void SocketGui::setConnection(bool isConnected)
{
    if(isConnected)
        ui->labelConnection->setPixmap(QPixmap(":/icons/STATUS_OK.png").scaled(30,30,Qt::KeepAspectRatio));
    else
        ui->labelConnection->setPixmap(QPixmap(":/icons/STATUS_NOK.png").scaled(30,30,Qt::KeepAspectRatio));
}

void SocketGui::setSocketType(SocketType pSocketType)
{
    if(mSocketType == pSocketType)
        return;

    mSocketType = pSocketType;
}

QString SocketGui::ipPlaceHolderText() const
{
    if(mSocketType == SocketType::SERVER)
        return QString(tr("Host Adress"));
    else
        return QString(tr("Server IP"));
}

