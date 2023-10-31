#include "signupscreen.h"
#include "ui_signupscreen.h"

SignUpScreen::SignUpScreen(QSqlDatabase* db_conn, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SignUpScreen),
    db(db_conn)
{
    ui->setupUi(this);
}

SignUpScreen::~SignUpScreen()
{
    delete ui;
}
