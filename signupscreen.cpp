#include "signupscreen.h"
#include "ui_signupscreen.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include <QDebug>

SignUpScreen::SignUpScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SignUpScreen)
{
    ui->setupUi(this);
}

SignUpScreen::~SignUpScreen()
{
    delete ui;
}

void SignUpScreen::on_pushButton_LogIn_clicked()
{
    emit changeToLoginScreen();
}


void SignUpScreen::on_pushButton_SignUp_clicked()
{
    QSqlDatabase::database().open();
    QSqlQuery querySignUpUser(QSqlDatabase::database());
    querySignUpUser.prepare("INSERT INTO users(email, password, first_name, last_name, phone_number, service_member)"
                            "VALUES(:email, :password, :first_name, :last_name, :phone_number, :service_member)");
    querySignUpUser.bindValue(":email", ui->lineEdit_Email->text().isEmpty() ? NULL : ui->lineEdit_Email->text());
    querySignUpUser.bindValue(":password", ui->lineEdit_Password->text().isEmpty() ? NULL : ui->lineEdit_Password->text());
    querySignUpUser.bindValue(":first_name", ui->lineEdit_FirstName->text().isEmpty() ? NULL : ui->lineEdit_FirstName->text());
    querySignUpUser.bindValue(":last_name", ui->lineEdit_LastName->text().isEmpty() ? NULL : ui->lineEdit_LastName->text());
    querySignUpUser.bindValue(":phone_number", ui->lineEdit_PhoneNumber->text().isEmpty() ? NULL : ui->lineEdit_PhoneNumber->text());
    querySignUpUser.bindValue(":service_member", ui->checkBox_ServiceMember->isChecked() ? "1" : "0");
    if (!querySignUpUser.exec()) {
        QSqlError err = querySignUpUser.lastError();
        qDebug() << err.text();
    }
    QSqlDatabase::database().close();
    ui->lineEdit_FirstName->clear();
    ui->lineEdit_LastName->clear();
    ui->lineEdit_Email->clear();
    ui->lineEdit_PhoneNumber->clear();
    ui->lineEdit_Password->clear();
    ui->lineEdit_ConfPassword->clear();

    emit changeToLoginScreen();
}

