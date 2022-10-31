#ifndef SOCKETGUI_H
#define SOCKETGUI_H

#include <QWidget>

enum class SocketType{
    SERVER = 0,
    CLIENT = 1
};

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



private:
    Ui::SocketGui *ui;
    uint32_t mUniqueId;

    SocketType mSocketType = SocketType::SERVER;



    void initGui();

    QString ipPlaceHolderText()const;
};

#endif // SOCKETGUI_H
