#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog> // 文本对话框
#include <QFileInfo>    // 文件信息
#include <QMessageBox>
#include <clientwidget.h>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->btnSelectFile->setEnabled(false);
    ui->btnSendFile->setEnabled(false);
    tcpServer = new QTcpServer(this);
    tcpServer->listen(QHostAddress::Any, 7777); // ip / port
    connect (tcpServer, &QTcpServer::newConnection, [=](){
        // 取出等待链接的下一个套接字
        tcpSocket = tcpServer->nextPendingConnection();
        QString ip = tcpSocket->peerAddress().toString();
        quint16 port = tcpSocket->peerPort();
        QString tmp = QString("[%1], [%2]以成功链接]").arg(ip).arg(port);
        ui->textEdit->setText(tmp);
        ui->btnSelectFile->setEnabled(true);
    });

    connect(&timer, &QTimer::timeout, [=](){
        timer.stop();
        // 发送文件
        sendDate();
    });
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_btnSelectFile_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open", "../");
    qDebug() << "path" << filePath;
    if (!filePath.isEmpty()){
        QFileInfo info(filePath);
        fileName = info.fileName();
        fileSize = info.size();
        sendSize = 0;
        if (file.isOpen()){
            file.close();
        }
        file.setFileName(filePath);
        // 只读打开文件
        bool isOk = file.open(QIODevice::ReadOnly);

        // open file fault
        if (!isOk){
            qDebug() << "file open fault" << "\n";
        }

        ui->textEdit->append(filePath);
        ui->btnSelectFile->setEnabled(false);
        ui->btnSendFile->setEnabled(true);
    }else{
        qDebug() << "文件路径选择失败";
    }
}

// 头信息 (文件名字，文件大小)
// 文件内容
void Widget::on_btnSendFile_clicked()
{
    // head info
    QString str = QString("%1##%2").arg(fileName).arg(fileSize);
    qDebug() << "文件头: " << str << '\n';
    qint64 len = tcpSocket->write(str.toUtf8().data());
    if (len > 0){
        // success to send head info
        // 发送文件内容
        // 延时发送，解决粘包问题
        timer.start(100); // 20ms trigger time out

    }else{
        qDebug() << "send fault";

        file.close();
        tcpSocket->disconnectFromHost();
        tcpSocket->close();
    }

}

void Widget::sendDate(){
    qint64 len = 0;
    do {
        // read file content
        char buf[1024 * 4] = {0};
        len = file.read(buf, sizeof (buf)); // len :读了多少内容
        // 读多少发多少
        len = tcpSocket->write(buf, len); // 发送多少

        sendSize += len;
    }while (len > 0);

    if (sendSize == fileSize){
        QMessageBox::information(this, "完毕", "send finish");
//        tcpSocket->disconnectFromHost();
//        tcpSocket->close();
        ui->btnSelectFile->setEnabled(true);
        //file.close();
    }
}
