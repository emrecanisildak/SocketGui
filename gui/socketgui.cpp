#include "socketgui.h"
#include "ui_socketgui.h"

#include <QDebug>

SocketGui::SocketGui(uint32_t mId, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SocketGui),
    mUniqueId(mId)
{
    ui->setupUi(this);
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

void SocketGui::on_pushButton_4_clicked()
{
   emit closeButtonClicked(mUniqueId);
}

