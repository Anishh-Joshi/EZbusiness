#ifndef EZBUSINESS_H
#define EZBUSINESS_H

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


QT_BEGIN_NAMESPACE
namespace Ui { class EzBusiness; }
QT_END_NAMESPACE

class EzBusiness : public QWidget
{
    Q_OBJECT

public:
    EzBusiness(QWidget *parent = nullptr);
    ~EzBusiness();

private slots:
    void on_sign_clicked();

    void on_Sales_clicked();

    void on_Purchase_clicked();

    void on_Stocks_clicked();

    void on_comboBox_currentIndexChanged();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_9_clicked();

    void on_comboBox_4_currentIndexChanged();

    void on_pushButton_10_clicked();

    void on_comboBox_2_currentIndexChanged();

    void on_LogOut_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_7_clicked();

    void on_ChangePassword_clicked();

    void on_pushButton_8_clicked();

    void on_Adduser_clicked();

    void on_pushButton_24_clicked();

    void on_CompanyName_clicked();

    void on_pushButton_28_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_11_clicked();

    void on_table_sales_activated(const QModelIndex &index);

    void on_table_purchase_activated(const QModelIndex &index);

    void on_pushButton_12_clicked();


    void on_pushButton_4_clicked();

    void on_pushButton_13_clicked();

    void on_forgot_clicked();

    void on_pushButton_14_clicked();

    void on_AboutUsBut_clicked();

    void on_pushButton_15_clicked();

    void on_AboutUs_clicked();

    void on_Home_clicked();

private:
    Ui::EzBusiness *ui;
    QSqlDatabase mydb;
    QSqlQueryModel *model;
    QSqlQueryModel *modelSearch;
};
class billing{
private:

    QString SalesBill,Seller,PanSales,VatAmount,FinalPrice;
    QDate date;

public:
    void billsetter(QString one,QString two,QString three, QDate four){
        SalesBill=one;
        Seller=two;
        PanSales=three;
        date=four;
    }

    QString billNo_Giver(){
        return SalesBill;
    }

    QString seller_Giver(){
        return Seller;
    }
    QString pan_Giver(){
        return PanSales;
    }
    QString date_Giver(){
        QString conversion;
        conversion=date.toString();
        return conversion;

    }

};

class Sales_Billing:public billing{

};
class Purchase_Billing:public billing{

};
class productsCommon{
public:
    QString PRODUCT[10],QUANTITY[10],PRICE[10], CODE;
    int index;
    QString vatHolder="Yes";
    QString Holder;
    int itemTotal[10];
    int FinalPrice=0;

public:
    void product_getter(QString one,QString two,QString three){

            PRODUCT[index] = one;
            QUANTITY[index]=two;
            PRICE[index]=three;
    }

    QString Pgiver(){
        return PRODUCT[index];

    }
    QString Qgiver(){
        return QUANTITY[index];

    }

    QString Prgiver(){
        return PRICE[index];

    }

    int index_indicator(){
        return index;
    }




    int ItemTotalCalculator(){

            qDebug()<<index;
            bool ok;
            itemTotal[index]=QUANTITY[index].toInt(&ok)*PRICE[index].toInt(&ok);
            return itemTotal[index];

    }




    void index_updater(int linker){
        index=linker;
    }

//    QString UpdateQuantity(){
//        for (int l = 0; l < 10; ++l) {
//            if (QUANTITY[l]>0){
//                return QUANTITY[l];
//            }
//        }
//    }

//    QString productUpdate(){
//        for (int l = 0; l < 10; ++l) {
//            if(PRODUCT[l] != ""){
//                return PRODUCT[l];

//        }
//        }
//    }


};

class salesProducts:public productsCommon{


};
class purchaseProducts:public productsCommon{

};
class stocks:public productsCommon{


public:
    void stocks_setter(QString one ,QString two, QString three, QString four){
        PRODUCT[1]=one;
        QUANTITY[1]=two;
        PRICE[1]=three;
        CODE=four;
    }
    void vat_setter(QString vatTaker){

        if(vatTaker=="No"){
            vatHolder = vatTaker;

        }
    }

    QString pGiver(){
        return PRODUCT[1];
    }
    QString qGiver(){
        return QUANTITY[1];
    }
    QString price_Giver(){
        return PRICE[1];
    }
    QString cGiver(){
        return CODE;
    }
    QString vGiver(){

        return vatHolder;
    }
};

class Admin{
public:
    QString CurrentPassword,NewPassword,ConfirmedPassword;
    QString EZBusinessKey="KUPROJECT";
    QString FUsername;
    QString Newusername;
    QString Phone,Address,Name,CPAN;
public:
    void PasswordGetter(QString CurrentPassword,QString NewPassword,QString ConfirmedPassword,QString key){
        this->CurrentPassword=CurrentPassword;
        this->NewPassword=NewPassword;
        this->ConfirmedPassword=ConfirmedPassword;
        this->EZBusinessKey=key;

    }

    void Forgot(QString FUsername,QString NewPassword,QString ConfirmedPassword){
        this->FUsername=FUsername;
        this->NewPassword=NewPassword;
        this->ConfirmedPassword=ConfirmedPassword;

    }



    QString FPWGiver(){
        return FUsername;
    }



    QString CurrentPasswordG(){
        return CurrentPassword;
    }
    QString NewPasswordG(){
        return NewPassword;
    }

    QString ConfirmedPasswordG(){
        return ConfirmedPassword;
    }
    QString KeyG(){
        return EZBusinessKey;
    }
    QString AddressG(){
        return Address;
    }
    QString NameG(){
        return Name;
    }
    QString PhoneG(){
        return Phone;
    }
    QString Newname(){
        return Newusername;
    }
    QString Newpass(){
        return ConfirmedPassword;
    }

    QString CPANG(){
        return CPAN;
    }
};
class newUser: public Admin{

public:
    void getNewUser(QString Newusername,QString CurrentPassword,QString NewPassword,QString ConfirmedPassword){
        this->Newusername=Newusername;
        this->CurrentPassword=CurrentPassword;
        this->NewPassword=NewPassword;
        this->ConfirmedPassword=ConfirmedPassword;

    }



};
class CompanyInfo:public Admin{

public:
    void infoGetter(QString Name,QString CurrentPassword,QString Address,QString Phone, QString CPAN){
        this->CurrentPassword=CurrentPassword;
        this->Address=Address;
        this->Name=Name;
        this->Phone=Phone;
        this->CPAN=CPAN;

    }



};

#endif // EZBUSINESS_H
