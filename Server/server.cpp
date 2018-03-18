#include "server.h"
#include "ui_server.h"

#include "serversocket.h"

#include <QTcpSocket>
#include <QDebug>

Server::Server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Server)
{
    ui->setupUi(this);

    m_tcpServer = new QTcpServer(this);

    connect(m_tcpServer,SIGNAL(newConnection()),
            this,SLOT(serverConnected()));

    m_tcpServer->listen(QHostAddress::Any,5555);
}

Server::~Server()
{
    delete ui;
}

void Server::serverConnected()
{
    ServerSocket *socket = new ServerSocket(
                m_tcpServer->nextPendingConnection());
}



