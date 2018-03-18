#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include <QWidget>
#include <QTcpSocket>
#include <QFile>
#include <QTimer>
#include <QStringList>

#define BUF_SIZE 1024*4

namespace Ui {
class ServerSocket;
}

class ServerSocket : public QWidget
{
    Q_OBJECT

public:
    explicit ServerSocket(QTcpSocket *socket, QWidget *parent = 0);
    ~ServerSocket();

    void getSongNameList(void);
    void sendData(void);
    quint64 sendMsgToClient(const QByteArray message);

private slots:
    void readyToRead(void);
    void slotTimeOut(void);

private:
    Ui::ServerSocket *ui;

    QFile m_file;             //文件对象
    QString m_fileName;       //文件名字
    qint64 m_fileSize;        //文件大小
    qint64 m_sendSize;        //已经发送大小
    QTimer *m_timer;           //定时

    QString m_songPath;
    QString m_songName;
    QStringList m_songNameList;

    quint16 m_tcpBlockSize;
    QTcpSocket *m_tcpSocket;
};

#endif // SERVERSOCKET_H
