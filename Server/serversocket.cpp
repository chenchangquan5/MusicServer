#include "serversocket.h"
#include "ui_serversocket.h"

#include <QMessageBox>
#include <QByteArray>
#include <QDir>
#include <QFileInfoList>

ServerSocket::ServerSocket(QTcpSocket *socket, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerSocket)
{
    ui->setupUi(this);

    m_tcpBlockSize = 0;
    m_tcpSocket = socket;

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()),
            this, SLOT(slotTimeOut()));

    connect(m_tcpSocket, SIGNAL(readyRead()),
            this, SLOT(readyToRead()));
}

ServerSocket::~ServerSocket()
{
    delete ui;
}

void ServerSocket::readyToRead(void)
{
    QDataStream in(m_tcpSocket);
    in.setVersion(QDataStream::Qt_5_8);

    if(m_tcpBlockSize == 0)
    {
        if(m_tcpSocket->bytesAvailable()<sizeof(quint16))
        {
            return;
        }

        in >> m_tcpBlockSize;
    }

    if(m_tcpSocket->bytesAvailable() < m_tcpBlockSize)
    {
        return;
    }

    QByteArray message;
    in >> message;

//    qDebug() << QString(message);

    getSongNameList();

    //接收到信息，将歌词列表名发送到客户端
    QString msg = "@$@$@" + m_songNameList.join(",");
    if("receive songs list" == QString(message))
    {
        sendMsgToClient(msg.toUtf8());
    }

    //匹配含有"%^%*%"的字符串信息
    QString position = message;
    if(position.contains("%^%*%", Qt::CaseSensitive))
    {
        bool ok;

        position.remove("%^%*%");
        int num = position.toInt(&ok, 10);
        m_songName = m_songNameList.at(num);

        QString filePath = m_songPath + "/" + m_songName + ".mp3";
        if(false == filePath.isEmpty()) //路径有效
        {
            m_fileName.clear();
            m_fileSize = 0;
            //获取文件信息：名字、大小
            QFileInfo info(filePath);
            m_fileName = info.fileName();
            m_fileSize = info.size();
            m_sendSize = 0;   //已经发送文件大小

            //以只读方式打开文件
            m_file.setFileName(filePath);
            if(false == m_file.open(QIODevice::ReadOnly)){
                 qDebug() << "只读方式打开文件失败";
            }
            //提示已经打开的文件路径
            qDebug() << filePath;

        }else
        {
            qDebug() << "选择文件路径无效：SERVER80";
        }

        QByteArray buffer;
        QDataStream out(&buffer, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_8);

        //先发送文件头信息:文件名##大小
        //构造头部信息
        QString head = QString("%1##%2").arg(m_fileName).arg(m_fileSize);
        //发送头部信息
        quint64 len = sendMsgToClient(head.toUtf8());
        if(len < 0)
        {
            qDebug() << "文件头部信息发送失败！";
            //关闭文件
            m_file.close();
        }
    }

    //采用回射信息进行粘包处理
    if("FileHead recv" == QString(message))
    {
        qDebug() << "文件头部接收成功，开始发送文件...";
        m_timer->start(5);
    }else if("file write done" == QString(message))
    {
        //服务器发送的快，而客户端接收的慢，所以要等客户端接收完毕后才能断开连接，以免丢包
        QMessageBox::information(this,"完成","对端接收完成");
        qDebug() << "文件发送且接收完成";
        m_file.close();
        m_tcpSocket->disconnectFromHost();
        m_tcpSocket->close();
    }

    m_tcpBlockSize = 0;
}

void ServerSocket::slotTimeOut(void)
{
    //一次发送的大小
    QByteArray array = m_file.read(BUF_SIZE);   //len为读取的字节数
    if(array.isEmpty())
    {
        m_timer->stop();
    }else
    {
        sendMsgToClient(array);

//        //已发数据累加
//        m_sendSize += len;
    }
//    qDebug() << "m_sendSize: " << m_sendSize;
}

void ServerSocket::sendData(void)
{
    qint64 len = 0;
    do{

    }while(len > 0);
}

//void ServerSocket::on_pb_send_clicked()
//{
//    //发送按钮已经点击，发送过程中不能再点击，恢复按钮初始化
//    ui->pb_choose->setEnabled(false);
//    ui->pb_send->setEnabled(false);

//    QByteArray buffer;
//    QDataStream out(&buffer, QIODevice::WriteOnly);
//    out.setVersion(QDataStream::Qt_4_6);

//    //先发送文件头信息:文件名##大小
//    //构造头部信息
//    QString head = QString("%1##%2").arg(m_fileName).arg(m_fileSize);
//    //发送头部信息
//    quint64 len = sendMsgToClient(head.toUtf8());
//    if(len < 0)
//    {
//        ui->textEdit->append("文件头部信息发送失败！");
//        //关闭文件
//        m_file.close();
//    }
//}

//void ServerSocket::on_pb_choose_clicked()
//{
//    QString filePath = QFileDialog::getOpenFileName(this,"open","../");
//    if(false == filePath.isEmpty()) //路径有效
//    {
//        m_fileName.clear();
//        m_fileSize = 0;
//        //获取文件信息：名字、大小
//        QFileInfo info(filePath);
//        m_fileName = info.fileName();
//        m_fileSize = info.size();
//        m_sendSize = 0;   //已经发送文件大小

//        //以只读方式打开文件
//        m_file.setFileName(filePath);
//        if(false == m_file.open(QIODevice::ReadOnly)){
//            ui->textEdit->append("只读方式打开文件失败");
//        }
//        //提示已经打开的文件路径
//        ui->textEdit->append(filePath);

//        //可以发送
//        ui->pb_choose->setEnabled(false);  //只能选择一次
//        ui->pb_send->setEnabled(true);
//    }else
//    {
//        ui->textEdit->append("选择文件路径无效：SERVER80");
//    }
//}

void ServerSocket::getSongNameList(void)
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
}

quint64 ServerSocket::sendMsgToClient(const QByteArray message)
{
    QByteArray buffer;
    QDataStream out(&buffer, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_8);

    out << (quint16)0;
    out << message;
    out.device()->seek(0);
    out << (quint16)(buffer.size() - sizeof(quint16));

    return m_tcpSocket->write(buffer);
}
