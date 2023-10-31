#include "loginscreen.h"
#include "ui_loginscreen.h"

LoginScreen::LoginScreen(QSqlDatabase* db_conn, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginScreen),
    db(db_conn)
{
    ui->setupUi(this);
}

LoginScreen::~LoginScreen()
{
    delete ui;
}
