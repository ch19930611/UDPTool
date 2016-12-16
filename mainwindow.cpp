#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    sender = new QUdpSocket(this);
    recver = new QUdpSocket(this);
    addr = new QHostAddress(QHostAddress::AnyIPv4);
    timer = new QTimer(this);

    localPort = 0;
    destPort = 0;

    ui->setupUi(this);
    ui->pushButton_2->setDisabled(true);
    ui->addrEdit_1->setText("0");
    ui->addrEdit_1->setValidator(new QIntValidator(0, 255, this));
    ui->addrEdit_2->setText("0");
    ui->addrEdit_2->setValidator(new QIntValidator(0, 255, this));
    ui->addrEdit_3->setText("0");
    ui->addrEdit_3->setValidator(new QIntValidator(0, 255, this));
    ui->addrEdit_4->setText("0");
    ui->addrEdit_4->setValidator(new QIntValidator(0, 255, this));
    ui->portEdit_1->setText("0");
    ui->portEdit_1->setValidator(new QIntValidator(0, 65535, this));
    ui->portEdit_2->setText("0");
    ui->portEdit_2->setValidator(new QIntValidator(0, 65535, this));
    ui->repeatEdit->setText("0");
    ui->repeatEdit->setValidator(new QIntValidator(0, 65535, this));

    connect(recver, SIGNAL(readyRead()),this, SLOT(recieveDatagram()));
    connect(timer, SIGNAL(timeout()), this, SLOT(repeatSendDatagram()));
}

MainWindow::~MainWindow()
{
    timer->stop();
    sender->close();
    recver->close();
    delete timer;
    delete sender;
    delete recver;
    delete ui;
}

void MainWindow::on_pushButton_1_clicked()
{
    localPort = ui->portEdit_1->text().toInt();
    if(localPort < 1000)
    {
        QMessageBox::information(this,nullptr, "Port is not correct!");
        return;
    }

    if(!recver->bind(localPort))
    {
        QMessageBox::information(this,nullptr, "Port is binded!");
        return;
    }
    ui->pushButton_1->setDisabled(true);
    ui->pushButton_2->setDisabled(false);
}

void MainWindow::on_pushButton_2_clicked()
{
    recver->close();
    ui->pushButton_2->setDisabled(true);
    ui->pushButton_1->setDisabled(false);
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->textBrowser->clear();
}

void MainWindow::on_pushButton_4_clicked()
{
    if(!ui->repeatCheckBox->isChecked())
    {
        QString addrStr = ui->addrEdit_1->text()
               + "." + ui->addrEdit_2->text()
               + "." + ui->addrEdit_3->text()
               + "." + ui->addrEdit_4->text();
        addr->setAddress(addrStr);
        destPort = ui->portEdit_2->text().toInt();

        QString qStr = ui->textEdit->toPlainText();
        QByteArray datagram = qStr.toLatin1();
        sender->writeDatagram(datagram.data(), datagram.size(),*addr, destPort);
    }
    else
    {
        mSec = ui->repeatEdit->text().toInt();
        if(mSec > 0)
        {
            ui->repeatEdit->setDisabled(true);
            ui->pushButton_4->setDisabled(true);
            timer->start(mSec);
        }
        else
        {
            ui->repeatCheckBox->setChecked(false);
        }
    }
}

void MainWindow::on_repeatCheckBox_clicked()
{
    if(!ui->repeatCheckBox->isChecked())
    {
        timer->stop();
        ui->repeatEdit->setDisabled(false);
        ui->pushButton_4->setDisabled(false);
    }
}

void MainWindow::recieveDatagram()
{
    while(recver->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(recver->pendingDatagramSize());
        recver->readDatagram(datagram.data(), datagram.size());
        if(!ui->hexCheckBox->isChecked())
        {
            QString qStr = datagram;
            ui->textBrowser->append(qStr);
        }
        else
        {
            QString qStr;
            for(int i = 0; i < datagram.size(); i++)
            {
                char aznable = datagram[i];
                if(aznable < 16)
                    qStr += "0" + QString::number((unsigned)aznable,16).toUpper() + " ";
                else
                    qStr += QString::number((unsigned)aznable,16).toUpper() + " ";
            }
            ui->textBrowser->append(qStr);
        }
    }
}

void MainWindow::repeatSendDatagram()
{
    QString addrStr = ui->addrEdit_1->text()
           + "." + ui->addrEdit_2->text()
           + "." + ui->addrEdit_3->text()
           + "." + ui->addrEdit_4->text();
    addr->setAddress(addrStr);
    destPort = ui->portEdit_2->text().toInt();

    QString qStr = ui->textEdit->toPlainText();
    QByteArray datagram = qStr.toLatin1();
    sender->writeDatagram(datagram.data(), datagram.size(),*addr, destPort);
}
