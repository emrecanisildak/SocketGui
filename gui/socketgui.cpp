#include "socketgui.h"
#include "ui_socketgui.h"

#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

SocketGui::SocketGui(uint32_t mId, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SocketGui),
    mUniqueId(mId)
{
    ui->setupUi(this);
    initGui();

}

SocketGui::~SocketGui()
{
    qDebug()<<"Socket Gui close"<<mUniqueId;
    delete ui;
}

uint32_t SocketGui::uniqueId() const
{
    return mUniqueId;
}

void SocketGui::initGui()
{
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

}

QString SocketGui::ipPlaceHolderText() const
{
    if(mSocketType == SocketType::SERVER)
        return QString(tr("Host Adress"));
    else
        return QString(tr("Server IP"));
}

