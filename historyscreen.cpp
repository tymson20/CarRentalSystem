#include "historyscreen.h"
#include "ui_historyscreen.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

HistoryScreen::HistoryScreen(unsigned int initialUserId, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HistoryScreen),
    userId(initialUserId)
{
    ui->setupUi(this);

    queryModel = new QSqlQueryModel;
    QSqlQuery queryGetHistoryData;
    queryGetHistoryData.prepare("SELECT concat(name, ' ', model) AS car_name, start_datetime, end_datetime, price, distance, registration_number FROM "
                                "   (SELECT vehicle_id, start_datetime, end_datetime, price, distance FROM rentals "
                                "   WHERE driver_id = :userId) AS rentals "
                                "LEFT JOIN vehicles ON vehicles.vehicle_id = rentals.vehicle_id "
                                "LEFT JOIN car_types ON car_types.car_type_id = vehicles.car_type_id "
                                "LEFT JOIN brands ON brands.brand_id = car_types.brand_id "
                                "ORDER BY start_datetime DESC;");
    queryGetHistoryData.bindValue(":userId", userId);

    if (!queryGetHistoryData.exec()) {
        QSqlError err = queryGetHistoryData.lastError();
        qDebug() << "Error in HistoryScreen constructor:" << err.text();
        return;
    }
    queryModel->setQuery(std::move(queryGetHistoryData));
    queryModel->setHeaderData(0, Qt::Horizontal, "Car model");
    queryModel->setHeaderData(1, Qt::Horizontal, "From");
    queryModel->setHeaderData(2, Qt::Horizontal, "To");
    queryModel->setHeaderData(3, Qt::Horizontal, "Price");
    queryModel->setHeaderData(4, Qt::Horizontal, "Distance");
    queryModel->setHeaderData(5, Qt::Horizontal, "Registration number");
    ui->tableView->setModel(queryModel);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setColumnWidth(0, 150);
    ui->tableView->setColumnWidth(5, 130);
}

HistoryScreen::~HistoryScreen()
{
    delete ui;
    if (queryModel) {
        delete queryModel;
        queryModel = nullptr;
    }
}

void HistoryScreen::on_pushButton_Back_clicked()
{
    emit backButtoncliked(userId);
}

