#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <QWidget>
#include <QStringList>
#include <QUdpSocket>

namespace Ui {
class UdpServer;
}

class UdpServer : public QWidget
{
    Q_OBJECT

public slots:
    void readyToRead(void);

public:
    explicit UdpServer(QWidget *parent = 0);
    ~UdpServer();

    void songNameList(void);
    void feedBackMessage(const QByteArray &msg);

private:
    Ui::UdpServer *ui;

    QString m_songPath;
    QStringList m_songNameList;

    int m_port;
    QUdpSocket *m_udpSocket;
};

#endif // UDPSERVER_H
