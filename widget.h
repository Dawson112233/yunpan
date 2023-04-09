#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_btnSelectFile_clicked();

    void on_btnSendFile_clicked();

private:
    Ui::Widget *ui;
    QTcpServer *tcpServer; // listen socket
    QTcpSocket *tcpSocket; // 通信socket

    QString fileName; // 文件名称
    qint64 fileSize;  // 文件大小
    qint64 sendSize;  // 已发送大小

    QFile file;       // 文件
    QTimer timer;     // 计时器，解决粘包
private:
    void sendDate();
};
#endif // WIDGET_H
