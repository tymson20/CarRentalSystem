#include "mainscreen.h"
#include "ui_mainscreen.h"

#include <QSqlQuery>
#include <QSqlError>

#include <QDebug>

MainScreen::MainScreen(unsigned int user_id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainScreen),
    user_id(user_id)
{
    ui->setupUi(this);

    qDebug() << "user_id =" << user_id;

    // Initialize TopBrandsBar
    topBrandsBar = new TopBrandsBar(4, this);
    topBrandsBar->fillBar();
    ui->verticalLayout_MiddleTab->insertWidget(3, topBrandsBar);
    ui->verticalLayout_MiddleTab->setStretch(3, 3);

    // Initialize FavouriteCarTypesBar
    favouriteCarTypesBar = new FavouriteCarTypesBar(3, user_id, this);
    favouriteCarTypesBar->fillBar();
    ui->verticalLayout_LeftTab->insertWidget(3, favouriteCarTypesBar);

    // Initialize LastRentalBar
    lastRentalBar = new LastRentalBar(user_id, this);
    lastRentalBar->fillBar();
    ui->verticalLayout_LeftTab->insertWidget(5, lastRentalBar);

    avatar = QPixmap(":/default_pictures/images/default_avatar.png");
    ui->label_UserPicture->setPixmap(avatar.scaled(150, 150, Qt::KeepAspectRatio));
}

MainScreen::~MainScreen()
{
    if (topBrandsBar) {
        delete topBrandsBar;
        topBrandsBar = nullptr;
    }
    if (favouriteCarTypesBar) {
        delete favouriteCarTypesBar;
        favouriteCarTypesBar = nullptr;
    }
    if (lastRentalBar) {
        delete lastRentalBar;
        lastRentalBar = nullptr;
    }
    delete ui;
}

unsigned int MainScreen::getUser_id() const
{
    return user_id;
}

void MainScreen::updateUserData() {
    QSqlQuery queryGetUserData;
    queryGetUserData.prepare("SELECT email, first_name, last_name, phone_number, covered_distance FROM users "
                             "WHERE user_id = :user_id");
    queryGetUserData.bindValue(":user_id", user_id);
    if (queryGetUserData.exec()) {
        if (queryGetUserData.next()) {
            email = queryGetUserData.value("email").toString();
            first_name = queryGetUserData.value("first_name").toString();
            last_name = queryGetUserData.value("last_name").toString();
            QString phone = queryGetUserData.value("phone_number").toString();
            phone.insert(6, " ");
            phone_number = phone.insert(3, " ");
            covered_distance = queryGetUserData.value("covered_distance").toFloat();
        }
    }
    else {
        QSqlError err = queryGetUserData.lastError();
        qDebug() << "Error in MainScreen.updateUserData():" << err.text();
    }

    ui->label_FirstName->setText(first_name);
    ui->label_LastName->setText(last_name);
    ui->label_Email->setText(email);
    ui->label_PhoneNumber->setText(phone_number);
    ui->label_CoveredDistance->setText(QString::number(covered_distance));

    ui->label_Hello->setText("Hello, " + first_name);
}

void MainScreen::on_pushButton_History_clicked()
{
    emit historyButtonClicked(user_id);
}


void MainScreen::on_pushButton_NewTrip_clicked()
{
    emit newTripButtonClicked(user_id);
}

