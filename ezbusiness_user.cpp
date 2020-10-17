#include "ezbusiness_user.h"
#include "ui_ezbusiness_user.h"
#include "ezbusiness.h"
#include<QSql>
#include<QDebug>
#include<QMessageBox>


EZBUSINESS_USER::EZBUSINESS_USER(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EZBUSINESS_USER)
{
    ui->setupUi(this);


}

EZBUSINESS_USER::~EZBUSINESS_USER()
{
    delete ui;
}

void EZBUSINESS_USER::on_pushButton_clicked()
{
    mydb=QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("../EZBUSINESS/Database/Login");
    if(!mydb.open())
    {
        qDebug()<<"Problem opss";

    }
    Admin forgot;

    forgot.Forgot(ui->username->text(),ui->password->text(),ui->confirm->text());
    qDebug()<<forgot.FPWGiver();

    if(ui->key->text()=="KUPROJECT"){
         QSqlQuery query2;
         query2.prepare("update Information set Username='"+forgot.FPWGiver()+"',Password='"+forgot.NewPasswordG()+"' where Username='"+forgot.FPWGiver()+"'");
         if(!query2.exec()){
             QMessageBox::warning(this,tr("Admin Panel"),tr("Can't Change your Password"));
         }
         else{
             QMessageBox::information(this,tr("Admin Panel"),tr("Password Sucessfully changed"));

         }
    }
    else{
    QMessageBox::warning(this,tr("Admin"),tr("Wrong Key"));
    }


}
