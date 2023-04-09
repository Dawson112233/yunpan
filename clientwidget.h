#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QFile>
namespace Ui {
class ClientWidget;
}

class ClientWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ClientWidget(QWidget *parent = nullptr);
    ~ClientWidget();

private slots:
    void on_pushButton_clicked();

private:
    Ui::ClientWidget *ui;
    QTcpSocket *tcpSocket;
    bool isHead; // whether head info

    QString fileName; // 文件名称
    qint64 fileSize;  // 文件大小
    qint64 recvSize;  // 已接收大小

    QFile file;       // 文件
};

#endif // CLIENTWIDGET_H
