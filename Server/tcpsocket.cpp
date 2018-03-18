#include "tcpsocket.h"
#include "ui_tcpsocket.h"

TcpSocket::TcpSocket(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TcpSocket)
{
    ui->setupUi(this);
}

TcpSocket::~TcpSocket()
{
    delete ui;
}
