#include "ezbusiness.h"
#include "ui_ezbusiness.h"
#include"ezbusiness_user.h"



/*---------------------------------------------------------Global Declarations----------------------------------*/

QString usernameGlobal,SGlobalBill,GlobalBill;

void connectionOpen();      //Database initializer for purchase and stocks.
void connectionClose();     //Database Closer for purchase and stocks.
void salesOpen();
void salesClose();
void loginOpen();
void loginClose();
QString db_BillLoader(QString Db);
int QuanitityChecker(QString pass);
int linkerSales=0,linkerPurchase=0;
void valueUpdater(QString choice,QString Target,QString quantity);


/*---------------------------------------------------------Global Declarations Ends-----------------------------*/

EzBusiness::EzBusiness(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EzBusiness)
{
    ui->setupUi(this);

}

EzBusiness::~EzBusiness()
{
    delete ui;
}

/*---------------------------------------------------------LOGIN Portion -----------------------------*/

void EzBusiness::on_sign_clicked()
{


    //Initialing/declaring Database of name Login
    mydb=QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("../EZBUSINESS/Database/Login");
    QString username,password;
    username=ui->USERNAME->text();
    usernameGlobal=username;
    password=ui->PASSWORD->text();

    //To check if database is functional
    if(!mydb.open()){
        qDebug()<<"Failed to open the database sorry";
        return;
    }

    QSqlQuery qry; //Query

    //Selection from database using select and comparing user given username password with database.
    if(qry.exec("select * from Information where Username='"+username+"' and password='"+password +"'")){
        int count=0;
        while(qry.next()){  //looping through next data in database
            count++;
        }

        if(count==1){
            ui->stackedWidget->setCurrentIndex(1);
            //Closing Login database.
            ui->NameUserAd->setText(usernameGlobal);
            mydb.close();
            mydb.removeDatabase(QSqlDatabase::defaultConnection);
            ui->PASSWORD->setText("");
            QString path=":images/"+usernameGlobal+".png";
            QString photo=path;

            if(username=="Kalyan"||username=="Anish"||username=="Paribesh"){

            QPixmap px(photo);
            ui->PhotoIcon->setPixmap(px);
            }
            else{
                QPixmap px(":images/person-icon-blue-18.png");
                ui->PhotoIcon->setPixmap(px);

            }

        }

        if(count<1){
           QMessageBox::warning(this,tr("Login Unsucessful"),tr("Wait a minute Who are you??"));
        }
    }


}
/*----------------------------------------------------------- Login Ends Here ----------------------------------------*/

/*----------------------------------------------------------- Sales Protion Starts Here ----------------------------------------*/

void EzBusiness::on_Sales_clicked()
{

    ui->stackedWidget_2->setCurrentIndex(1);
    QDate wer = wer.currentDate();
    ui->dateTimeEdit->setDate(wer);
    connectionOpen();
    this->model=new QSqlQueryModel();

    model->setQuery("SELECT [Product] FROM StocksTable"); // Loading Data held in Product key into the combo box

    ui->comboBox->setModel(model); //Ui displaying the loaded data.
    connectionClose(); //Stocks databsae ends here

}
void EzBusiness::on_comboBox_currentIndexChanged()
{
    QString name,quan;
    name=ui->comboBox->currentText(); //Dopdown menu selected item gets stored in name String.
    connectionOpen();
    QSqlQuery qry;
    qry.prepare("select * from StocksTable where Product='"+name+"'");  //Database searchh for that name and its attributes for Qlabel.

        if(qry.exec()){
            while(qry.next())
            {
                ui->lineEdit_4->setText(qry.value(0).toString()); //stocks index 0 is Product name
                ui->lineEdit_5->setText(qry.value(2).toString()); //stocks index 3 is Product rate
                ui->lineEdit_8->setText(qry.value(4).toString()); //stocks index 3 is Product rate

            }
                connectionClose();
        }
        else{
            QMessageBox::information(this,tr("error"),tr("Not Loaded"));
        }

}
void EzBusiness::on_pushButton_clicked()
{
    salesOpen();

    ui->lineEdit->setText(db_BillLoader("SalesbillPrimary"));

    //Variable data fetch from Qlabel
    Sales_Billing deploy;

    deploy.billsetter(ui->lineEdit->text(),ui->lineEdit_2->text(),ui->lineEdit_3->text(),ui->dateTimeEdit->date());

    SGlobalBill=deploy.billNo_Giver();//Passing bill number to SGlobal to make it a global value.

    QString query = "CREATE TABLE SalesbillPrimary ("
                    "Name TEXT,"
                    "BillNo INTEGER,"
                    "Date TEXT,"
                    "Pan INTEGER);";

    QSqlQuery qry;
    if(!qry.exec(query))
    {
        qDebug()<<"Error in creating a Table";
    }

         qry.prepare("insert into SalesbillPrimary (Name,BillNo,Pan,Date) values ('"+deploy.seller_Giver()+"','"+deploy.billNo_Giver()+"','"+deploy.pan_Giver()+"','" +deploy.date_Giver()+"')");
         if(!qry.exec()){
             qDebug()<<"Opss error in adding values";
         }
         else{
             QMessageBox::information(this,tr("Database"),tr("Bill Initialized"));
         }


         salesClose();

}
void EzBusiness::on_pushButton_3_clicked()
{
    salesOpen();


    QString SBillPath;//Billpath to target primary Bill.
    SBillPath = SGlobalBill; //Global BillNumber.
    int akira;
    QString query = "CREATE TABLE '"+SBillPath+"' ("
                    "BillPurchase,"
                    "Product TEXT,"
                    "Quantity INTEGER,"
                    "Total TEXT,"
                    "Price INTEGER);";

    QSqlQuery qry;
    if(!qry.exec(query))
    {
        qDebug()<<"Error in creating a Functional Table";
    }

    salesProducts deploy;
    deploy.index_updater(linkerSales);
    deploy.product_getter(ui->lineEdit_4->text(),ui->lineEdit_6->text(),ui->lineEdit_8->text());
    QString here = QString::number(deploy.ItemTotalCalculator());



        akira=QuanitityChecker(deploy.Qgiver());

        if(akira!=1){
            qry.prepare("insert into'"+SBillPath+"' (BillPurchase,Product,Quantity,Price,Total) values ('"+SGlobalBill+"','"+deploy.Pgiver()+"','"+deploy.Qgiver()+"','"+deploy.Prgiver()+"','"+here+"')");

                    if(!qry.exec()){
                qDebug()<<"Opss error in adding Functional values";

                    }

            ui->lineEdit_4->setText("");
            ui->lineEdit_6->setText("");
            ui->lineEdit_8->setText("");
            linkerSales++;
            salesClose();

            salesOpen();

            this->model=new QSqlQueryModel();
            model->setQuery("SELECT [Product],[Quantity],[Price],[Total] FROM '"+SBillPath+"'");
            ui->table_sales->setModel(model);
            salesClose();

        }
        valueUpdater("Sales",deploy.Pgiver(),deploy.Qgiver());

}
/*----------------------------------------------------------- Sales Ends Here -----------------------------------------*/

/*----------------------------------------------------------- Purchase Starts Here -----------------------------------------*/
void EzBusiness::on_Purchase_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(2);
    QDate wer = wer.currentDate();
    ui->dateTimeEdit_4->setDate(wer);
    connectionOpen();

    this->model=new QSqlQueryModel();

    model->setQuery("SELECT [Product] FROM StocksTable");// Loading Data held in Product key into the combo box.

    ui->comboBox_4->setModel(model);//Ui displaying the loaded data.
    on_pushButton_7_clicked();

    connectionClose();
}

void EzBusiness::on_pushButton_9_clicked()
{
    connectionOpen();
    ui->lineEdit_25->setText(db_BillLoader("billPrimary"));
    Purchase_Billing deploy;
    deploy.billsetter(ui->lineEdit_25->text(),ui->lineEdit_26->text(),ui->lineEdit_27->text(),ui->dateTimeEdit_4->date());
    GlobalBill=deploy.billNo_Giver();
    connectionOpen();


    QString query = "CREATE TABLE billPrimary ("
                    "Name TEXT,"
                    "BillNo INTEGER,"
                    "Date TEXT,"
                    "Pan INTEGER);";

    QSqlQuery qry;
    if(!qry.exec(query))
    {
        qDebug()<<"Error in creating a Table";
    }


         qry.prepare("insert into billPrimary (Name,BillNo,Pan,Date) values ('"+deploy.seller_Giver()+"','"+deploy.billNo_Giver()+"','"+deploy.pan_Giver()+"','" +deploy.date_Giver()+"')");
         if(!qry.exec()){
             qDebug()<<"Opss error in adding values";
         }
         else{
             QMessageBox::information(this,tr("Database"),tr("Bill Initialized"));
         }
         connectionClose();


}

void EzBusiness::on_comboBox_4_currentIndexChanged()
{
    QString name;

    name=ui->comboBox_4->currentText();
    connectionOpen();
    QSqlQuery qry;
    qry.prepare("select * from StocksTable where Product='"+name+"'");//Database seacrch for that name and its attributes for Qlabel.

        if(qry.exec()){
            while(qry.next())
            {
                ui->lineEdit_28->setText(qry.value(0).toString());//stocks index 0 is Product name
                ui->lineEdit_32->setText(qry.value(4).toString()); //stocks index 3 is Product rate

            }
                connectionClose();
        }
        else{
            QMessageBox::information(this,tr("error"),tr("Not Loaded"));
        }
        connectionClose();
}

void EzBusiness::on_pushButton_10_clicked()
{
    connectionOpen();


    QString BillPath;
    BillPath = GlobalBill;

    int akira;

    QString query = "CREATE TABLE '"+BillPath+"' ("
                    "BillPurchase,"
                    "Product TEXT,"
                    "Quantity INTEGER,"
                    "Total INTEGER,"
                    "Price INTEGER);";

    QSqlQuery qry;
    if(!qry.exec(query))
    {
        qDebug()<<"Error in creating a Functional Table";
    }

        purchaseProducts Deploy;
        Deploy.index_updater(linkerPurchase);
        Deploy.product_getter(ui->lineEdit_28->text(),ui->lineEdit_30->text(),ui->lineEdit_32->text());
        akira=QuanitityChecker(Deploy.Qgiver());
        QString here = QString::number(Deploy.ItemTotalCalculator());


    if(akira!=1){
        qry.prepare("insert into'"+BillPath+"' (BillPurchase,Product,Quantity,Price,Total) values ('"+GlobalBill+"','"+Deploy.Pgiver()+"','"+Deploy.Qgiver()+"','"+Deploy.Prgiver()+"','"+here+"')");
        if(!qry.exec()){
    qDebug()<<"Opss error in adding Functional values";
        }

    linkerPurchase++;
    connectionOpen();
    this->model=new QSqlQueryModel();
    model->setQuery("SELECT [Product],[Quantity],[Price],[Total] FROM '"+BillPath+"'");
    ui->table_purchase->setModel(model);
    connectionClose();
    }
    valueUpdater("Purchase",Deploy.Pgiver(),Deploy.Qgiver());
}
/*----------------------------------------------------------- Purchase Ends Here -----------------------------------------*/


/*----------------------------------------------------------- Stocks Starts Here -----------------------------------------*/

void EzBusiness::on_Stocks_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(3);
    connectionOpen();
    this->model=new QSqlQueryModel();
    model->setQuery("SELECT [Product] FROM StocksTable");
    ui->comboBox_2->setModel(model);
    connectionClose();


}

void EzBusiness::on_comboBox_2_currentIndexChanged()
{
        QString name;
        name=ui->comboBox_2->currentText();
        connectionOpen();
        QSqlQuery qry;
        qry.prepare("select * from StocksTable where Product='"+name+"'");

            if(qry.exec()){
                while(qry.next())
                {
                    ui->lineEdit_14->setText(qry.value(0).toString());
                }
                    connectionClose();
            }
            else{
                QMessageBox::information(this,tr("error"),tr("Not Loaded"));
            }
}

void EzBusiness::on_pushButton_6_clicked()
{
    QString Ndel;
    Ndel=ui->lineEdit_14->text();
    connectionOpen();
    QSqlQuery qry;
    qry.prepare("DELETE from StocksTable where Product='"+Ndel+"'");

        if(!qry.exec()){

                QMessageBox::information(this,tr("Delete"),tr("Unable"));
        }
        else{
            QMessageBox::information(this,tr("Delete"),tr("Deleted"));

        }
     connectionClose();
}

void EzBusiness::on_pushButton_5_clicked()
{
    connectionOpen();

    QString query = "CREATE TABLE StocksTable ("
                    "Product TEXT,"
                    "ProductCode TEXT,"
                    "Quantity INTEGER,"
                    "Vat TEXT,"
                    "Price INTEGER);";

    QSqlQuery qry;

    if(!qry.exec(query))
    {
        qDebug()<<"Error in creating a Stocks Table";
    }

    stocks deploy;
    deploy.stocks_setter(ui->lineEdit_10->text(),ui->lineEdit_11->text(),ui->lineEdit_12->text(),ui->lineEdit_9->text());
    deploy.vat_setter(ui->comboBox_2->currentText());
    qry.prepare("insert into StocksTable (Product,ProductCode,Quantity,Price,Vat) values ('"+deploy.pGiver()+"','"+deploy.cGiver()+"','"+deploy.qGiver()+"','"+deploy.price_Giver()+"','"+deploy.vGiver()+"')");

        if(!qry.exec()){
                QMessageBox::information(this,tr("Stocks"),tr("Item Couldn't be Saved"));
        }
        else{
            QMessageBox::information(this,tr("Stocks"),tr("Item Saved"));
        }

        ui->lineEdit_9->setText("");
        ui->lineEdit_10->setText("");
        ui->lineEdit_11->setText("");
        ui->lineEdit_12->setText("");
    connectionClose();
}

void EzBusiness::on_pushButton_7_clicked()
{
    connectionOpen();
    this->model=new QSqlQueryModel();
    model->setQuery("SELECT [ProductCode],[Product],[Quantity] FROM StocksTable");
    ui->tableView->setModel(model);
    connectionClose();

}


/*----------------------------------------------------------- Stocks Ends Here -----------------------------------------*/

/*----------------------------------------------------------- Admin Starts Here -----------------------------------------*/
void EzBusiness::on_pushButton_8_clicked()
{
    Admin deploy;
    mydb=QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("../EZBUSINESS/Database/Login");
    if(!mydb.open()){
        qDebug()<<"Failed to open the database sorry";
        return;
    }

    deploy.PasswordGetter(ui->Current->text(),ui->New->text(),ui->Confirm->text(),ui->CompKey->text());

    QSqlQuery qry;

    if(deploy.NewPasswordG()==deploy.ConfirmedPasswordG()){

    //Selection from database using select and comparing user given username password with database.
    if(qry.exec("select * from Information where Username='"+usernameGlobal+"' and Password='"+deploy.CurrentPasswordG() +"'")){
        int count=0;
        while(qry.next()){  //looping through next data in database
            count++;
        }

        if(count==1){

            if(deploy.KeyG()=="KUPROJECT"){

                QSqlQuery query2;
                query2.prepare("update Information set Username='"+usernameGlobal+"',Password='"+deploy.NewPasswordG()+"' where Username='"+usernameGlobal+"'");
                QMessageBox::information(this,tr("ADMIN PANEL"),tr("Password has been susessfully changed!"));
                if(!query2.exec()){
                    QMessageBox::warning(this,tr("ADMIN PANEL"),tr("Unable to update Password"));
                    mydb.close();
                    mydb.removeDatabase(QSqlDatabase::defaultConnection);
                }
            }
            else{
                QMessageBox::warning(this,tr("ADMIN PANEL"),tr("Given Company key is Incorrect"));
            }

        }

        if(count<1){
           QMessageBox::warning(this,tr("ADMIN PANEL"),tr("Unable to verify if it's you"));
        }
    }
    }
    else{
        QMessageBox::warning(this,tr("ADMIN PANEL"),tr("Unable to confirm new password"));

    }
}

void EzBusiness::on_Adduser_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(7);
    ui->label_58->setText(usernameGlobal+",");

}

void EzBusiness::on_pushButton_24_clicked()
{
    newUser deploy;
    mydb=QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("../EZBUSINESS/Database/Login");
    if(!mydb.open()){
        qDebug()<<"Failed to open the database sorry";
        return;
    }

    deploy.getNewUser(ui->username->text(),ui->pw->text(),ui->npw->text(),ui->cpw->text());
    QSqlQuery qry;
    if(deploy.NewPasswordG()==deploy.ConfirmedPasswordG()){

    //Selection from database using select and comparing user given username password with database.
    if(qry.exec("select * from Information where Username='"+usernameGlobal+"' and Password='"+deploy.CurrentPasswordG() +"'")){
        int count=0;
        while(qry.next()){  //looping through next data in database
            count++;
        }

        if(count==1){

            QSqlQuery qry;
                 qry.prepare("insert into Information (USERNAME,PASSWORD) values ('"+deploy.Newname()+"','"+deploy.Newpass()+"')");
                 if(!qry.exec()){
                     qDebug()<<"Opss error in adding values";
                 }
                 else{
                     QMessageBox::information(this,tr("ADMIN PANEL"),tr("Account Created"));
                 }
        }

        if(count<1){
           QMessageBox::warning(this,tr("ADMIN PANEL"),tr("Unable to verify if it's you"));
        }
    }
    }
    else{
        QMessageBox::warning(this,tr("ADMIN PANEL"),tr("Unable to confirm new password"));

    }
}


void EzBusiness::on_pushButton_28_clicked()
{
    CompanyInfo deploy;
    mydb=QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("../EZBUSINESS/Database/Login");
    if(!mydb.open()){
        qDebug()<<"Failed to open the database sorry";
        return;
    }

    deploy.infoGetter(ui->CNAme->text(),ui->pw_5->text(),ui->Address->text(),ui->Phone->text(), ui->COPAN->text());
    QSqlQuery qry;

    //Selection from database using select and comparing user given username password with database.
    if(qry.exec("select * from Information where Username='"+usernameGlobal+"' and Password='"+deploy.CurrentPasswordG() +"'")){
        int count=0;
        while(qry.next()){  //looping through next data in database
            count++;
        }
        QString id="1";

        if(count==1){
            QString query = "CREATE TABLE CompanyINFO ("
                            "ID INTEGER,"
                            "Name TEXT,"
                            "Address TEXT,"
                            "Pan INTEGER,"
                            "Phone INTEGER);";

            QSqlQuery qry;
            if(!qry.exec(query))
            {
                qDebug()<<"Error in creating a Table";
            }


                qry.prepare("Update CompanyINFO set Name ='"+deploy.NameG()+"', Address='"+deploy.AddressG()+"', Phone='"+deploy.PhoneG()+"', Pan='"+deploy.CPANG()+"' where ID='"+id+"'");
                 if(!qry.exec()){
                     qDebug()<<"Opss error in adding values";
                 }
                 else{
                     QMessageBox::information(this,tr("ADMIN PANEL"),tr("Comapany Updated"));
                 }
        }

        if(count<1){
           QMessageBox::warning(this,tr("ADMIN PANEL"),tr("Unable to verify if it's you"));
        }
    }

    else{
        QMessageBox::warning(this,tr("ADMIN PANEL"),tr("Unable to confirm new password"));

    }
}

void EzBusiness::on_ChangePassword_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(6);
    ui->label_36->setText(usernameGlobal +",");
}

/*----------------------------------------------------------- Admin Ends Here -----------------------------------------*/

void connectionOpen(){
    QSqlDatabase purchase;
    //SQlite Query for storing Billnumber, Name and Pan Number
    qDebug()<<"Start";
    purchase=QSqlDatabase::addDatabase("QSQLITE");
    purchase.setDatabaseName("../EZBUSINESS/Database/Purchase");
    if(!purchase.open())
    {
        qDebug()<<"Problem opss";

    }

}
void EzBusiness::on_forgot_clicked()
{
    EZBUSINESS_USER call;
    call.setModal(true);
    call.exec();

}

void connectionClose(){
    QSqlDatabase purchase;
    purchase.close();
    purchase.removeDatabase(QSqlDatabase::defaultConnection);

}

void salesOpen(){
    QSqlDatabase Sales;
    qDebug()<<"Start";
    Sales=QSqlDatabase::addDatabase("QSQLITE");
    Sales.setDatabaseName("../EZBUSINESS/Database/Sales");
    if(!Sales.open())
    {
        qDebug()<<"Problem opss";

    }

}
void salesClose(){
    QSqlDatabase Sales;
    Sales.close();
    Sales.removeDatabase(QSqlDatabase::defaultConnection);
}

int QuanitityChecker(QString pass){
    int Converter;
    int init=0;

    bool ok;
    Converter = pass.toInt(&ok);
    QMessageBox uy;
    uy.setText(pass);
    if(Converter<=0){
        QMessageBox here;
        here.setText("Quantity Error Refill The first cart");
        here.exec();
        init=1;
    }
    return init;
}


void EzBusiness::on_LogOut_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}



void EzBusiness::on_CompanyName_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(8);
    ui->label_52->setText(usernameGlobal+",");


}

/*-------------------------------------------------------- VALUE UPDATER ------------------------------------------*/

void valueUpdater(QString choice,QString Target,QString quantity){
    connectionOpen();

    QSqlQuery qry;
    if(choice=="Sales"){
        qry.prepare("update StocksTable set Quantity=(Quantity-'"+quantity+"') where Product='"+Target+"'" );

            if(!qry.exec()){
                qDebug()<<"Failed to open the database sorry";
    }
    }
    else{
        qry.prepare("update StocksTable set Quantity=(Quantity+'"+quantity+"') where Product='"+Target+"'" );

            if(!qry.exec()){
                qDebug()<<"Failed to open the database sorry";
    }
    }
    connectionClose();
}



void EzBusiness::on_table_sales_activated(const QModelIndex &index)
{
    QString name;
    salesOpen();
    name=ui->table_sales->model()->data(index).toString();
    QSqlQuery qry;
    qry.prepare("select * from'"+SGlobalBill+"' where Product='"+name+"' or Quantity='"+name+"' or Total='"+name+"' or Price='"+name+"' or BillPurchase='"+name+"'");//Database seacrch for that name and its attributes for Qlabel.

        if(qry.exec()){
            while(qry.next())
            {

                QString p,q;
                ui->deleter->setText(qry.value(1).toString());
                p=(qry.value(1).toString());
                q=(qry.value(2).toString());
                salesClose();
                connectionOpen();
                QSqlQuery qry;

                 qry.prepare("update StocksTable set Quantity=(Quantity+'"+q+"') where Product='"+p+"'" );

                 if(!qry.exec()){
                     qDebug()<<"Some Problem Occured";
                 }
                 connectionClose();

            }

        }
        else{
            QMessageBox::information(this,tr("Error"),tr("Not Loaded"));
        }
        connectionClose();

}

void EzBusiness::on_pushButton_11_clicked()
{
    QString deleteName=ui->deleter->text();
    salesOpen();
    QSqlQuery qry;
    qDebug()<<SGlobalBill;
    qry.prepare("Delete from'"+SGlobalBill+"' where Product='"+deleteName+"'");
    if(qry.exec()){

            QMessageBox::information(this,tr("Item Information"),tr("Item removed from Cart"));
            this->model=new QSqlQueryModel();
            model->setQuery("SELECT [Product],[Quantity],[Price],[Total] FROM '"+SGlobalBill+"'");
            ui->table_sales->setModel(model);
            salesClose();

    }
    else{
        QMessageBox::information(this,tr("Error"),tr("Couldn't Remove the selected item, something went wrong"));
    }

    salesClose();

}

/*-------------------------------------------------------- VALUE UPDATER Sales Ends Here ------------------------------------------*/


/*---------------------------------------------------------- Sales Invoice ---------------------------------------------------*/
void EzBusiness::on_pushButton_2_clicked()
{
    mydb=QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("../EZBUSINESS/Database/Login");
    if(!mydb.open()){
        qDebug()<<"Failed to open the database sorry";
        return;
    }

    QString COMPANYINV,PANDISP;
     QSqlQuery queryC("SELECT [Name],[Pan] FROM CompanyINFO");
     while (queryC.next()) {
         COMPANYINV.append( queryC.value(0).toString() + " ");
         PANDISP.append( queryC.value(1).toString() + " ");

     }
     ui->label_11->setText(COMPANYINV);
     ui->PanINV->setText(PANDISP);


    ui->stackedWidget_2->setCurrentIndex(9);
    salesOpen();
    this->model=new QSqlQueryModel();
    model->setQuery("SELECT [Product],[Quantity],[Price],[Total] FROM '"+SGlobalBill+"'");
    ui->tableView_2->setModel(model);

    bool ok;
    QSqlQuery query;
       QString queryString = "SELECT Total FROM '"+SGlobalBill+"'";

       query.exec(queryString);

        int calculator;
        int Grand_Total=0;
       while (query.next()) {


           QSqlRecord record = query.record();
           calculator= record.value(0).toInt(&ok);
           Grand_Total=Grand_Total+calculator;
           qDebug()<<calculator;


       }

        ui->BIllINV->setText(ui->lineEdit->text());
        ui->NameINV->setText(ui->lineEdit_2->text());
        ui->PanSINV->setText(ui->lineEdit_3->text());
        ui->DateINV->setText(ui->dateTimeEdit->text());
        ui->FINALTOTAL->setNum(Grand_Total);
        ui->lineEdit->setText("");
        ui->lineEdit_2->setText("");
        ui->lineEdit_3->setText("");

       salesClose();

}
/*---------------------------------------------------------- Sales Invoice Ends Here ---------------------------------------------------*/




void EzBusiness::on_table_purchase_activated(const QModelIndex &index)
{
    QString name;

    name=ui->table_purchase->model()->data(index).toString();
    connectionOpen();
    QSqlQuery qry;

    qry.prepare("select * from'"+GlobalBill+"' where Product='"+name+"' or Quantity='"+name+"' or Total='"+name+"' or Price='"+name+"' or BillPurchase='"+name+"'");//Database seacrch for that name and its attributes for Qlabel.

        if(qry.exec()){
            while(qry.next())
            {
                QString p,q;
                ui->deleter1->setText(qry.value(1).toString());
                p=(qry.value(1).toString());
                q=(qry.value(2).toString());
                connectionClose();

                connectionOpen();
                QSqlQuery qry;

                 qry.prepare("update StocksTable set Quantity=(Quantity-'"+q+"') where Product='"+p+"'" );

                 if(!qry.exec()){
                     qDebug()<<"Some Problem Occured";
                 }
                 connectionClose();

            }
                connectionClose();
        }
        else{
            QMessageBox::information(this,tr("Error"),tr("Not Loaded"));
        }
        connectionClose();
}



void EzBusiness::on_pushButton_12_clicked()
{
    QString deleteName=ui->deleter1->text();
    connectionOpen();
    QSqlQuery qry;
    qDebug()<<GlobalBill;
    qry.prepare("Delete from'"+GlobalBill+"' where Product='"+deleteName+"'");
    if(qry.exec()){

            QMessageBox::information(this,tr("Cart"),tr("Item removed From Cart"));
            this->model=new QSqlQueryModel();
            model->setQuery("SELECT [Product],[Quantity],[Price],[Total] FROM '"+GlobalBill+"'");
            ui->table_purchase->setModel(model);
            connectionClose();

    }
    else{
        QMessageBox::information(this,tr("Error"),tr("Not Loaded"));
    }
}



/*------------------------------------------------------ Invoice Purchase -----------------------------------*/

void EzBusiness::on_pushButton_4_clicked()
{
    mydb=QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("../EZBUSINESS/Database/Login");
    if(!mydb.open()){
        qDebug()<<"Failed to open the database sorry";
        return;
    }

    QString COMPANYINV,PANDISP;
     QSqlQuery queryC("SELECT [Name],[Pan] FROM CompanyINFO");
     while (queryC.next()) {
         COMPANYINV.append( queryC.value(0).toString() + " ");
         PANDISP.append( queryC.value(1).toString() + " ");

     }
     ui->label_11->setText(COMPANYINV);
     ui->PanINV->setText(PANDISP);


    ui->stackedWidget_2->setCurrentIndex(9);
    connectionOpen();
    this->model=new QSqlQueryModel();
    model->setQuery("SELECT [Product],[Quantity],[Price],[Total] FROM '"+GlobalBill+"'");
    ui->tableView_2->setModel(model);

    bool ok;
    QSqlQuery query;
       QString queryString = "SELECT Total FROM '"+GlobalBill+"'";

       query.exec(queryString);

        int calculator;
        int Grand_Total=0;
       while (query.next()) {


           QSqlRecord record = query.record();
           calculator= record.value(0).toInt(&ok);
           Grand_Total=Grand_Total+calculator;
           qDebug()<<calculator;


       }
       ui->BIllINV->setText(ui->lineEdit_25->text());
       ui->NameINV->setText(ui->lineEdit_26->text());
       ui->PanSINV->setText(ui->lineEdit_27->text());
       ui->DateINV->setText(ui->dateTimeEdit_4->text());

        ui->FINALTOTAL->setNum(Grand_Total);
        ui->lineEdit_26->setText("");
        ui->lineEdit_27->setText("");
        ui->lineEdit_25->setText("");
       connectionClose();
}

/*------------------------------------------------------ Invoice Purchase Ends Here -----------------------------------*/

/*------------------------------------------------------ Ledger Here -----------------------------------*/
void EzBusiness::on_pushButton_14_clicked()
{
    salesOpen();
    this->model=new QSqlQueryModel();
    model->setQuery("SELECT [BillNo],[Name],[Date],[Pan] FROM SalesbillPrimary ");
    ui->tableView_3->setModel(model);
    salesClose();
}



void EzBusiness::on_pushButton_15_clicked()
{
    connectionOpen();
    this->model=new QSqlQueryModel();
    model->setQuery("SELECT [BillNo],[Name],[Date],[Pan] FROM billPrimary ");
    ui->tableView_3->setModel(model);
    connectionClose();
}
/*------------------------------------------------------ Ledger Ends Here -----------------------------------*/
void EzBusiness::on_AboutUsBut_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(4);
}


void EzBusiness::on_AboutUs_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(5);
}

void EzBusiness::on_Home_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(0);
}


QString db_BillLoader(QString Db) {
      bool ok;
    int Bill=0;

    QSqlQuery query;
       QString queryString = "SELECT BillNo FROM '"+Db+"'";
       query.exec(queryString);

        QString fetcher;
       while (query.next()) {


           QSqlRecord record = query.record();
           fetcher= record.value(0).toString();
           int number=fetcher.toInt(&ok);


           if(number>=Bill){
               Bill=number;
           }

           else if(fetcher==""){
               Bill=0;
           }
           else{
               QMessageBox here;
               here.setText("Fatal Error");
               here.exec();
           }

}


       Bill=Bill+1;

       QString BillString = QString::number(Bill);
       qDebug()<<BillString;
       return BillString;

}


