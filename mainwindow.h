#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class SocketGui;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_closeButtonClicked(quint32 id);

private:
    Ui::MainWindow *ui;
    std::vector<SocketGui*> mSocketGui;

    quint32 uniqueIdGenerate();

};
#endif // MAINWINDOW_H
