#include "loginscreen.h"
#include "ui_loginscreen.h"

#include <QSqlQuery>
#include <QSqlError>

#include <QDebug>

LoginScreen::LoginScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginScreen)
{
    ui->setupUi(this);

    const QString style = "QLineEdit {border: 2px solid #21add1;"
                          "           border-radius: 20px;"
                          "           padding-left: 10px;"
                          "           padding-right: 10px;"
                          "           font-size: 13}"
                          "QLineEdit:focus {background-color: #ededed}";
    setStyleSheet(style);

    pixmap = QPixmap(":/background_pictures/images/CRS_main_page2.png");
    ui->label_photo->setPixmap(pixmap.scaled(400, 600, Qt::KeepAspectRatio));

    //DELETE THIS
    ui->lineEdit_Email->setText("test@gmail.com");
    ui->lineEdit_Password->setText("test");

    qDebug() << "Login created";
}

LoginScreen::~LoginScreen()
{
    delete ui;

    qDebug() << "Login deleted";
}

void LoginScreen::on_pushButton_SignUp_clicked()
{
    emit signUpButtonClicked();
}


void LoginScreen::on_pushButton_LogIn_clicked()
{
    QSqlQuery queryLogInUser;
    queryLogInUser.prepare("SELECT user_id FROM users "
                           "WHERE email = :email AND password = :password;");
    queryLogInUser.bindValue(":email", ui->lineEdit_Email->text());
    queryLogInUser.bindValue(":password", ui->lineEdit_Password->text());
    if (queryLogInUser.exec()) {
        if (queryLogInUser.next()) {
            qDebug() << "Login successful, user_id =" << queryLogInUser.value("user_id").toString();
            emit loggedSuccessfully(queryLogInUser.value("user_id").toUInt());
        }
        else {
            qDebug() << "Incorrect email or password!";
        }
    }
    else {
        QSqlError err = queryLogInUser.lastError();
        qDebug() << "Login error :" << err.text() << "\n";
    }
    ui->lineEdit_Email->clear();
    ui->lineEdit_Password->clear();
}

