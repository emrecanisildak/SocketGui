#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QRandomGenerator>
#include <algorithm>
#include "gui/socketgui.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->clear();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    auto  uId = uniqueIdGenerate();
    SocketGui* sGui = new SocketGui(uId);


    ui->tabWidget->addTab(sGui,QString::number(uId));
    connect(sGui, &SocketGui::closeButtonClicked, this,&MainWindow::on_closeButtonClicked);

    mSocketGui.push_back(sGui);
}

void MainWindow::on_closeButtonClicked(quint32 id)
{
    auto itr = std::find_if(mSocketGui.begin(),mSocketGui.end(),[ id](SocketGui* gui)
    {
        return gui->uniqueId() == id;
    });


    if(itr != mSocketGui.end())
    {
        ui->tabWidget->removeTab(std::distance(mSocketGui.begin(),itr));
        delete *itr;
        mSocketGui.erase(itr);
    }
}

quint32 MainWindow::uniqueIdGenerate()
{
   static bool isSearchContinue = true;
   isSearchContinue = true;

   static  quint32 tId = 0;
   tId = 0;

    while(isSearchContinue)
    {
        // Generate Random Id
        tId = QRandomGenerator::global()->generate();

        auto itr = std::find_if(mSocketGui.begin(),mSocketGui.end(),[](SocketGui* gui)
        {
            return gui->uniqueId() == tId;
        });


        if( itr == mSocketGui.end())
            isSearchContinue = false;
    }

    return tId;

}

