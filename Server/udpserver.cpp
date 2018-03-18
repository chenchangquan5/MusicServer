#include "udpserver.h"
#include "ui_udpserver.h"

#include <QHostAddress>
#include <QDir>
#include <QFileInfoList>
#include <QDebug>

UdpServer::UdpServer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UdpServer)
{
    ui->setupUi(this);

    m_port = 5555;
    m_udpSocket = new QUdpSocket(this);

    m_udpSocket->bind(m_port,QUdpSocket::ReuseAddressHint);

    connect(m_udpSocket,SIGNAL(readyRead()),
            this,SLOT(readyToRead()));

    songNameList();
}

UdpServer::~UdpServer()
{
    delete ui;
}

void UdpServer::readyToRead(void)
{
    songNameList();

    while(m_udpSocket->hasPendingDatagrams())
    {
        QByteArray in;
        in.resize(m_udpSocket->pendingDatagramSize());
        m_udpSocket->readDatagram(in.data(), in.size());

        QString msg = in.data();

        feedBackMessage(in);
        qDebug() << msg;
    }
}

void UdpServer::feedBackMessage(const QByteArray &msg)
{
    if ("Receive songNameList" == msg)
    {
        QString message = "m_songNameList";
        int length = 0;

        if((length = m_udpSocket->writeDatagram(message.toLatin1(),
           message.length(),QHostAddress::Broadcast,m_port)) != message.length())
        {
            return;
        }
    }
}

void UdpServer::songNameList(void)
{
    m_songPath = "E:/lhenv/Feiqiu/UnigressMusic/songs";

    QDir d(m_songPath);
    QFileInfoList list = d.entryInfoList(QStringList() << "*.mp3",
                                         QDir::Files, QDir::Time);
    m_songNameList.clear();

    foreach(const QFileInfo &info, list)
    {
        m_songNameList.append(info.baseName());
    }

    ui->lw_songNameList->clear();
    ui->lw_songNameList->addItems(m_songNameList);
}
