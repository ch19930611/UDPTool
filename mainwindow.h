#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QtNetwork/QUdpSocket>
#include <QHostAddress>
#include <QMessageBox>
#include <iostream>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_1_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void recieveDatagram();

    void repeatSendDatagram();

    void on_repeatCheckBox_clicked();

private:
    Ui::MainWindow *ui;
    QHostAddress * addr;
    QUdpSocket * sender,
               * recver;
    QTimer * timer;

    int localPort, destPort, mSec;
};

#endif MAINWINDOW_H
