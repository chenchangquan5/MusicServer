#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QWidget>
#include <QTcpSocket>

namespace Ui {
class TcpSocket;
}

class TcpSocket : public QWidget
{
    Q_OBJECT

public:
    explicit TcpSocket(QWidget *parent = 0);
    ~TcpSocket();

private:
    Ui::TcpSocket *ui;
};

#endif // TCPSOCKET_H
