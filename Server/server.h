#ifndef SERVER_H
#define SERVER_H

#include "udpserver.h"

#include <QWidget>
#include <QTcpServer>



namespace Ui {
class Server;
}

class Server : public QWidget
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = 0);
    ~Server();

private slots:
    void serverConnected(void);

private:
    Ui::Server *ui;

    QTcpServer *m_tcpServer;
};

#endif // SERVER_H
