#include <QKeyEvent>
#include <QDateTime>
#include "tcpwidget.h"
#include "ui_tcpwidget.h"
#include "E:/QtWorkSpace/ERP_release_v3/ERP/util.h"
#pragma execution_character_set("utf-8")
TCPWidget::TCPWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TCPWidget)
{
    ui->setupUi(this);
    setWindowTitle("Server");
    ui->readTextEdit->setReadOnly(true);

    ui->writeTextEdit->installEventFilter(this);
    tcpSocket = NULL;
    //建立监听套接字；来监听客户端是否发送消息；
    tcpServer = new QTcpServer(this);

    //开始监听
    tcpServer->listen(QHostAddress::Any,8088);

    //当检测到客户端发送的连接请求时,监听套接字会触发newConnect信号，所以链接他的槽 信号与槽
    connect(tcpServer,&QTcpServer::newConnection,[=]()
            //客户端发起连接请求时的槽函数，连接成功时显示客户端的信息
    {
        tcpSocket = tcpServer->nextPendingConnection();
        QString ip = tcpSocket->peerAddress().toString();//获取对方（peer，也就是客户端）的IP地址
        quint16 port = tcpSocket->peerPort();//获取对方的端口号
        QString connectSuccess = QString("[%1:%2]连接成功！").arg(ip).arg(port);
        ui->readTextEdit->setText(connectSuccess);

        //显示客户端发送过来的信息
        connect(tcpSocket,&QTcpSocket::readyRead,[=]()
        {
            QByteArray client_msg = tcpSocket->readAll();
            QDateTime systime;
            QString timestamp = systime.currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            ui->readTextEdit->append("["+timestamp+"]"+client_msg);

        });
        connect(tcpSocket,&QTcpSocket::disconnected,[=]()
        {
            ui->readTextEdit->append("<br><h6 align='center'><font color = gray>----连接已断开----</font></h6>");
        });

    }
    );




}

TCPWidget::~TCPWidget()
{
    delete ui;
}

bool TCPWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->writeTextEdit)
    {
        if(event->type() == QEvent::KeyPress)
        {
            QKeyEvent *k = static_cast<QKeyEvent *>(event);
            if(k->key()==Qt::Key_Return)
            {
                ui->sendBtn->click();
                return true;
            }
        }
    }
    return QWidget::eventFilter(watched,event);
}
//发送消息
void TCPWidget::on_sendBtn_clicked()
{
    if(tcpSocket == NULL)
    {
        showMessageBox(warningBox,"连接都没有建立，你给谁发呢？");
        return;
    }
    QString message_to_send = ui->writeTextEdit->toPlainText();
    //QString转char*
    tcpSocket->write(message_to_send.toUtf8().data());
    QDateTime systime;
    QString timestamp = systime.currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString str = QString("<br><h6 align='right'>[%1]"+message_to_send+"</h6>").arg(timestamp);
    ui->readTextEdit->append(str);
    ui->writeTextEdit->clear();
}
//断开连接
void TCPWidget::on_disconnectBtn_clicked()
{

    if(tcpSocket == NULL)
    {
        showMessageBox(warningBox,"连接还未建立，无法断开");
        return;
    }
    tcpSocket->disconnectFromHost();
    tcpSocket = NULL;
}
