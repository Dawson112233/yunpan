#include "clientwidget.h"
#include "ui_clientwidget.h"
#include <QMessageBox>


ClientWidget::ClientWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientWidget)
{
    ui->setupUi(this);
    tcpSocket = new QTcpSocket(this);
    isHead = true;
    ui->progressBar->setValue(0);
    connect(tcpSocket, &QTcpSocket::readyRead, [=](){
       QByteArray array = tcpSocket->readAll();
        if (true == isHead){
            if (file.isOpen()){
                file.close();
            }
            // read head   name##len##content
            // 拆包
            isHead = false;
            fileName = QString(array).section("##", 0, 0);
            fileSize = QString(array).section("##", 1, 1).toInt();
            recvSize = 0;
            qDebug() << "client file name: " << fileName;
            // 创建文件，设置只写
            QString filePath = "D:\\MyCloudPan\\";
            file.setFileName(filePath + fileName);
            bool isOk = file.open(QIODevice::WriteOnly);
            if (false == isOk){
                qDebug() << "file open fault" << "\n";
                tcpSocket->disconnectFromHost();
                tcpSocket->close();
            }
            ui->progressBar->setMinimum(0);
            ui->progressBar->setMaximum(fileSize / 1024); // int
            ui->progressBar->setValue(0);
        }else{ // 得到文件内容
            qint64 len = file.write(array);
            recvSize += len;
            ui->progressBar->setValue(recvSize / 1024);
            if (recvSize == fileSize){
                isHead = true;
                QMessageBox::information(this, "info", "接收成功");
                file.close();
//                tcpSocket->disconnectFromHost();
//                tcpSocket->close();
            }
        }
    });
}

ClientWidget::~ClientWidget()
{
    delete ui;
}

void ClientWidget::on_pushButton_clicked()
{
    QString ip = ui->lineEdit->text();
    quint16 port = ui->lineEdit_2->text().toUInt();
    tcpSocket->connectToHost(ip, port);
}
