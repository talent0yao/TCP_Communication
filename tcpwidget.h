#ifndef TCPWIDGET_H
#define TCPWIDGET_H

#include <QWidget>
#include <QTcpServer>//监听套接字
#include <QTcpSocket>//通信套接字

namespace Ui {
class TCPWidget;
}

class TCPWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TCPWidget(QWidget *parent = 0);
    ~TCPWidget();

    bool eventFilter(QObject *watched, QEvent *event);
private slots:
    void on_sendBtn_clicked();

    void on_disconnectBtn_clicked();

private:
    Ui::TCPWidget *ui;
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;

};

#endif // TCPWIDGET_H
