#include <QDebug>
#include <QKeyEvent>
#include <QDateTime>
#include "clientwidget.h"
#include "ui_clientwidget.h"
#include <QHostAddress>
#include "E:/QtWorkSpace/ERP_release_v3/ERP/util.h"
#pragma execution_character_set("utf-8")
ClientWidget::ClientWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientWidget)
{
    ui->setupUi(this);
    setWindowTitle("Client");
    ui->readTextEdit->setReadOnly(true);
    ui->sendBtn->setShortcut(QKeySequence::InsertParagraphSeparator);//将回车键与登录按钮绑定在一起
    //ui->login_LoginPushButton->setShortcut(Qt::Key_Enter);
    ui->sendBtn->setShortcut(Qt::Key_Return);

    ui->IPLineEdit->setText("127.0.0.1");
    ui->portLineEdit->setText("8888");
    ui->writeTextEdit->installEventFilter(this);
    tcpSocket = new QTcpSocket(this);

    //建立连接后 显示提示信息
    connect(tcpSocket,&QTcpSocket::connected,[=]()
    {
        ui->readTextEdit->setText("成功建立连接,可以开始聊天了！");

    });
    //接收到服务端信息后进行显示
    connect(tcpSocket,&QTcpSocket::readyRead,[=]()
    {

        QDateTime systime;
        QString timestamp = systime.currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        ui->readTextEdit->append("["+timestamp+"]"+tcpSocket->readAll());
    });

    connect(tcpSocket,&QTcpSocket::disconnected,[=]()
    {
        ui->readTextEdit->append("<br><h6 align='center'><font color = gray>----连接已断开----</font></h6>");
    });
}

ClientWidget::~ClientWidget()
{
    delete ui;
}

bool ClientWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->writeTextEdit)
    {
        if(event->type()==QEvent::KeyPress)
        {
            QKeyEvent *k = static_cast<QKeyEvent *>(event);
            if(k->key() == Qt::Key_Return)
            {
                ui->sendBtn->setFocus();
                ui->sendBtn->click();
                return true;
            }
        }
    }
    return QWidget::eventFilter(watched,event);
}

void ClientWidget::on_connectBtn_clicked()
{
    QString ip = ui->IPLineEdit->text();
    quint16 port = ui->portLineEdit->text().toUInt();
    tcpSocket->connectToHost(QHostAddress(ip),port);

}

void ClientWidget::on_sendBtn_clicked()
{
    qDebug()<<"isWritable:"<<tcpSocket->isWritable();
    if(!tcpSocket->isWritable())
    {
        showMessageBox(warningBox,"不能发送数据！请检查连接是否建立");
        return;
    }
    if(ui->writeTextEdit->toPlainText().isEmpty())
    {
        showMessageBox(warningBox,"请输入内容！");
        return;
    }

    QString msg_to_send = ui->writeTextEdit->toPlainText();
    tcpSocket->write(msg_to_send.toUtf8().data());

    QDateTime systime;
    QString timestamp = systime.currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString str = QString("<br><h6 align='right'>[%1]"+msg_to_send+"</h6>").arg(timestamp);
    ui->readTextEdit->append(str);
    ui->writeTextEdit->clear();

}

void ClientWidget::on_disconnectBtn_clicked()
{

    tcpSocket->disconnectFromHost();
    tcpSocket->close();
}




