#ifndef EZBUSINESS_USER_H
#define EZBUSINESS_USER_H

#include <QWidget>
#include<QtSql>
#include<QDebug>
#include<QFileInfo>
#include<QtCore>
#include<QMessageBox>
#include<ui_ezbusiness.h>
#include<QDate>
#include <QDateTimeEdit>
#include<QShortcut>

namespace Ui {
class EZBUSINESS_USER;
}

class EZBUSINESS_USER : public QDialog
{
    Q_OBJECT

public:
    explicit EZBUSINESS_USER(QWidget *parent = nullptr);
    ~EZBUSINESS_USER();

private slots:
    void on_pushButton_clicked();

private:
    Ui::EZBUSINESS_USER *ui;
    QSqlDatabase mydb;
    QSqlQueryModel *model;
    QSqlQueryModel *modelSearch;
};
//class AdminOutside{
//private:
//    QString Username,UPassword,KEY, OldPassword,NewPassword,ConfirmNewPassword;


//public:
//    void Creategetter(QString Username,QString UPassword,QString KEY, QString OldPassword,QString NewPassword,QString ConfirmNewPassword;)
//};

#endif // EZBUSINESS_USER_H
