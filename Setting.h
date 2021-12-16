#ifndef SETTING_H
#define SETTING_H

#include <QThread>
#include <QVector>
#include "mainwindow.h"
#include "dialog.h"
#include "ui_dialog.h"

struct Datas;
struct Time;
class MainWindow;
class Setting : public QThread
{
    Q_OBJECT

public:
    Setting(QWidget *parent, QVector<Datas>* &d);
    ~Setting();
    QVector<Datas>*& ds;

public slots:
    void rec_users(QString users);
    void rec_locations(QString locations);
    void rec_latitude(QString latitude);
    void rec_longitude(QString longitude);
    void rec_days(QDate daystart, QDate dayend);

protected:
    void run();

private:
    bool u_f,lct_f,t_f,lo_f,la_f;

    QVector<int>* Users;
    QVector<int>* Locations;
    QVector<float>* Latitude;
    QVector<float>* Longitude;
    QVector<QDate>* Times1;
    QVector<Datas>* AfterSelect;

signals:
    void SendDataset(Datas da);

};
#endif // SETTING_H