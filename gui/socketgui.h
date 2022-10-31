#ifndef SOCKETGUI_H
#define SOCKETGUI_H

#include <QWidget>

namespace Ui {
class SocketGui;
}

class SocketGui : public QWidget
{
    Q_OBJECT

public:
    explicit SocketGui(uint32_t mId,QWidget *parent = nullptr);
    ~SocketGui();

    uint32_t uniqueId()const;


signals:
    void closeButtonClicked(uint32_t pUniqueId);

private slots:
    void on_pushButton_4_clicked();

private:
    Ui::SocketGui *ui;
    uint32_t mUniqueId;
};

#endif // SOCKETGUI_H
