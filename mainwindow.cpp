#include "mainwindow.h"
#include "OpenFile.h"
#include "dialog.h"
#include "Setting.h"
#include "function.h"
#include "ui_mainwindow.h"
#include "ui_dialog.h"
#include "ui_function.h"

#include "QFileDialog"
#include "QMessageBox"
#include "QGridLayout"
#include "QPushButton"
#include "QTextStream"
#include "QDebug"
#include "iostream"
#include <QMetaType>
#include <QEventLoop>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    qRegisterMetaType<Datas>("Datas");
    Dataset = new QVector<Datas>;

    ui->setupUi(this);
    oui = new Dialog;
    oui->setWindowFlags(Qt::WindowStaysOnTopHint);
    oui->show();

    //read in
    th1 = new Openf(this,Dataset);
    oui->ui->lineEdit->setPlaceholderText(" There are currently no files listed, please click \"browsing\".");
    connect(oui->ui->Open,SIGNAL(clicked()),this,SLOT(FileOpen()));
    connect(oui->ui->begin,SIGNAL(clicked()),this,SLOT(begin()));
    connect(this,&MainWindow::SendFile,th1,&Openf::rec,Qt::AutoConnection);
    connect(th1,&Openf::SendToMain,this,&MainWindow::RecFromOp);

    //set range
    userid_flag = false;
    locationid_flag = false;
    time_flag = false;
    lo_flag = false;
    la_flag = false;

    ui->userid_edit->setPlaceholderText("eg: 1;3;4");
    ui->locationid_edit->setPlaceholderText("eg: 1;3;4");
    ui->lo_edit->setPlaceholderText("eg: 30.24-31.35");
    ui->la_edit->setPlaceholderText("eg: 30.24-31.35");
    ui->fctBox->addItem("Top10 POIs");
    ui->fctBox->addItem("number of checking-ins");
    ui->fctBox->addItem("dynamic POIs(change by time)");

    connect(ui->User_ID,SIGNAL(clicked()),this,SLOT(SlcUserid()));
    connect(ui->Location_ID,SIGNAL(clicked()),this,SLOT(SlcLocationid()));
    connect(ui->Time,SIGNAL(clicked()),this,SLOT(SlcTime()));
    connect(ui->Longitude,SIGNAL(clicked()),this,SLOT(SlcLo()));
    connect(ui->Latitude,SIGNAL(clicked()),this,SLOT(SlcLa()));

    Set_th2 = new Setting(this,Dataset);
    connect(ui->next,SIGNAL(clicked()),this,SLOT(setRange()));
    connect(this,&MainWindow::SendUsers,Set_th2,&Setting::rec_users);
    connect(this,&MainWindow::SendLocations,Set_th2,&Setting::rec_locations);
    connect(this,&MainWindow::SendLatitude,Set_th2,&Setting::rec_latitude);
    connect(this,&MainWindow::SendLongitude,Set_th2,&Setting::rec_longitude);
    connect(this,&MainWindow::SendDays,Set_th2,&Setting::rec_days);

    //function
    fui = new Function(this);
    connect(Set_th2,&Setting::SendDataset,fui,&Function::rec_dataset);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::begin(){
    qDebug()<<"Main ID:"<<QThread::currentThreadId()<<endl;
    oui->on_toolButton_clicked();
    this->show();
}

void MainWindow::RecFromOp(int i){
    oui->ui->Openprogress->setValue(i);
    if (i == 1502535){
        oui->ui->Loading->setText("Upload Successful!");
    }
}

void MainWindow::FileOpen(){
    oui->ui->Openprogress->setRange(0,1502535);
    QString fileName = QFileDialog::getOpenFileName(
                this,
                tr("open a file."),
                "D:/",
                tr("excel(*.csv);;All files(*.*)")
        );

        if (fileName.isEmpty()) {
            QMessageBox::warning(this, "Warning!", "Failed to open the video!");
        }
        else {
            th1->start();
            emit SendFile(fileName);
            oui->ui->Loading->setText("Loading...");
        }
        oui->ui->lineEdit->setText(fileName);
}

void MainWindow::SlcUserid(){
    if (ui->User_ID->isChecked()){
        userid_flag = true;
    }
    else {
        userid_flag = false;
    }
}

void MainWindow::SlcLocationid(){
    if (ui->Location_ID->isChecked()){
        locationid_flag = true;
    }
    else {
        locationid_flag = false;
    }
}

void MainWindow::SlcTime(){
    if (ui->Time->isChecked()){
        time_flag = true;
    }
    else {
        time_flag = false;
    }
}

void MainWindow::SlcLo(){
    if (ui->Longitude->isChecked()){
        lo_flag = true;
    }
    else {
        lo_flag = false;
    }
}

void MainWindow::SlcLa(){
    if (ui->Latitude->isChecked()){
        la_flag = true;
    }
    else {
        la_flag = false;
    }
}

void MainWindow::setRange(){
    QStringList list;
    list.clear();

    if (userid_flag){
        QString tmp_uid = ui->userid_edit->text();
        emit SendUsers(tmp_uid);
    }

    if (locationid_flag){
        QString tmp_lctid = ui->locationid_edit->text();
        emit SendLocations(tmp_lctid);
    }

    if (time_flag){
        QDate tmp_t1 = ui->dateEdit->date();
        QDate tmp_t2 = ui->dateEdit_2->date();
        emit SendDays(tmp_t1, tmp_t2);
    }

    if (la_flag){
        QString tmp_la = ui->la_edit->text();
        emit SendLatitude(tmp_la);
    }

    if (lo_flag){
        QString tmp_lo = ui->lo_edit->text();
        emit SendLongitude(tmp_lo);
    }

    Set_th2->start();
    this->hide();
    startfunction();
}

void MainWindow::startfunction(){
    fui->show();
}














