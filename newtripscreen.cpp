#include "newtripscreen.h"
#include "ui_newtripscreen.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

NewTripScreen::NewTripScreen(unsigned int initialUserId, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewTripScreen),
    userId(initialUserId)
{
    ui->setupUi(this);

    const QString style = "QLineEdit {border: 2px solid #21add1;"
                          "           border-radius: 13px;"
                          "           padding-left: 10px;"
                          "           padding-right: 10px;"
                          "           font-size: 9}"
                          "QLineEdit:focus {background-color: #ededed}";
    setStyleSheet(style);

    queryModel = new QSqlQueryModel();
    ui->tableView->setModel(queryModel);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->verticalHeader()->setVisible(false);
    selectedCarId = UINT32_MAX;

    QObject::connect(ui->tableView->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &NewTripScreen::displayCarParameters);

    QSqlQuery queryCheckCurrentRental;
    queryCheckCurrentRental.prepare("SELECT current_rental_id FROM users "
                                    "WHERE user_id = :user_id;");
    queryCheckCurrentRental.bindValue(":user_id", userId);
    if (queryCheckCurrentRental.exec()) {
        queryCheckCurrentRental.next();
        if (queryCheckCurrentRental.value(0).isNull()) {
            openNewTripPage();
        }
        else {
            currentRentalId = queryCheckCurrentRental.value(0).toUInt();
            openEndTripPage();
        }
    }
    else {
        QSqlError err = queryCheckCurrentRental.lastError();
        qDebug() << "Error in NewTripScreen constructor:" << err.text();
    }
}

NewTripScreen::~NewTripScreen()
{
    delete ui;
    if (queryModel) {
        delete queryModel;
        queryModel = nullptr;
    }
    qDebug() << "Trip screen deleted";
}

void NewTripScreen::on_pushButton_Back_clicked()
{
    if (ui->stackedWidget->currentIndex() != 0) ui->stackedWidget->setCurrentIndex(0);
    emit backButtoncliked(userId);
}


void NewTripScreen::on_pushButton_EndTrip_clicked()
{
    unsigned int vehicleId;
    float mileage, distance;
    int pricePerHour, durationHour, durationMinutes;
    QSqlQuery queryGetVehicleData;
    queryGetVehicleData.prepare("SELECT vehicles.vehicle_id, mileage, price_per_hour, timediff(NOW(), start_datetime) AS duration FROM rentals "
                                "LEFT JOIN vehicles ON vehicles.vehicle_id = rentals.vehicle_id "
                                "WHERE rental_id = :rental_id;");
    queryGetVehicleData.bindValue(":rental_id", currentRentalId);
    if (!queryGetVehicleData.exec()) {
        QSqlError err = queryGetVehicleData.lastError();
        qDebug() << "Error in NewTripScreen.on_pushButton_EndTrip_clicked:" << err.text();
        return;
    }
    queryGetVehicleData.next();
    vehicleId = queryGetVehicleData.value(0).toUInt();
    mileage = queryGetVehicleData.value(1).toFloat();
    distance = ui->doubleSpinBox_Mileage->value() - mileage;
    if (distance < 0.0f) {
        qDebug() << "Mileage incorrect!";
        return;
    }

    pricePerHour = queryGetVehicleData.value(2).toInt();
    QStringList duration = queryGetVehicleData.value(3).toString().split(':');
    durationHour = duration[0].toInt();
    durationMinutes = duration[1].toInt();
    QSqlQuery queryUpdateVehicle;
    queryUpdateVehicle.prepare("UPDATE vehicles "
                               "SET mileage = :mileage, "
                               "    available = 1 "
                               "WHERE vehicle_id = :vehicle_id");
    queryUpdateVehicle.bindValue(":mileage", ui->doubleSpinBox_Mileage->value());
    queryUpdateVehicle.bindValue(":vehicle_id", vehicleId);
    if (!queryUpdateVehicle.exec()) {
        QSqlError err = queryUpdateVehicle.lastError();
        qDebug() << "Error in NewTripScreen.on_pushButton_EndTrip_clicked:" << err.text();
        return;
    }

    QSqlQuery queryUpdateRental;
    queryUpdateRental.prepare("UPDATE rentals "
                              "SET end_datetime = NOW(), "
                              "    price = :price, "
                              "    distance = :distance "
                              "WHERE rental_id = :rental_id;");
    float price = (durationHour + durationMinutes/60.0f)*pricePerHour;
    qDebug() << durationHour << durationMinutes << price;
    queryUpdateRental.bindValue(":price", price);
    queryUpdateRental.bindValue(":distance", distance);
    queryUpdateRental.bindValue(":rental_id", currentRentalId);
    if (!queryUpdateRental.exec()) {
        QSqlError err = queryUpdateRental.lastError();
        qDebug() << "Error in NewTripScreen.on_pushButton_EndTrip_clicked:" << err.text();
        return;
    }

    QSqlQuery queryUpdateUser;
    queryUpdateUser.prepare("UPDATE users "
                            "SET current_rental_id = null, "
                            "    covered_distance = covered_distance + :distance, "
                            "    spent_money = spent_money + :price "
                            "WHERE user_id = :user_id;");
    queryUpdateUser.bindValue(":distance", distance);
    queryUpdateUser.bindValue(":price", price);
    queryUpdateUser.bindValue(":user_id", userId);
    if (!queryUpdateUser.exec()) {
        QSqlError err = queryUpdateUser.lastError();
        qDebug() << "Error in NewTripScreen.on_pushButton_EndTrip_clicked:" << err.text();
        return;
    }

    openNewTripPage();
}

void NewTripScreen::on_pushButton_Search_clicked()
{
    if (ui->lineEdit->text().isEmpty()) {
        loadAllCars();
    }
    else {
        loadCarsByPattern(ui->lineEdit->text());
    }

    if (ui->stackedWidget->currentIndex() != 0) ui->stackedWidget->setCurrentIndex(0);
}

void NewTripScreen::on_pushButton_Rent_clicked()
{
    QSqlQuery queryStartNewRentalRentals;
    queryStartNewRentalRentals.prepare("INSERT INTO rentals(driver_id, vehicle_id) VALUES( "
                                       ":user_id, :vehicle_id);");
    queryStartNewRentalRentals.bindValue(":user_id", userId);
    queryStartNewRentalRentals.bindValue(":vehicle_id", selectedCarId);
    if(!queryStartNewRentalRentals.exec()) {
        QSqlError err = queryStartNewRentalRentals.lastError();
        qDebug() << "Error in NewTripScreen.on_pushButton_Rent_clicked():" << err.text();
        return;
    }
    QSqlQuery queryStartNewRentalUsers;
    queryStartNewRentalUsers.prepare("UPDATE users "
                                     "SET current_rental_id = LAST_INSERT_ID() "
                                     "WHERE user_id = :user_id;");
    queryStartNewRentalUsers.bindValue(":user_id", userId);
    if(!queryStartNewRentalUsers.exec()) {
        QSqlError err = queryStartNewRentalUsers.lastError();
        qDebug() << "Error in NewTripScreen.on_pushButton_Rent_clicked():" << err.text();
        return;
    }
    QSqlQuery queryStartNewRentalVehicles;
    queryStartNewRentalVehicles.prepare("UPDATE vehicles "
                                        "SET available = 0 "
                                        "WHERE vehicle_id = :vehicle_id;");
    queryStartNewRentalVehicles.bindValue(":vehicle_id", selectedCarId);
    if(!queryStartNewRentalVehicles.exec()) {
        QSqlError err = queryStartNewRentalVehicles.lastError();
        qDebug() << "Error in NewTripScreen.on_pushButton_Rent_clicked():" << err.text();
        return;
    }

    qDebug() << "New rental created!";
    openEndTripPage();
}

void NewTripScreen::displayCarParameters(const QModelIndex& current, const QModelIndex& previous) {
    qDebug() << "Index changed :)";
    selectedCarId = ui->tableView->model()->index(current.row(), 0).data().toUInt();
    qDebug() << selectedCarId;

    QSqlQuery queryGetCarData;
    queryGetCarData.prepare("SELECT registration_number, mileage, price_per_hour, concat(brands.name, ' ', car_types.model) AS car_name, "
                            "body_type, number_of_seats, horsepower, gearbox, description, image FROM "
                            "    (SELECT vehicle_id, registration_number, mileage, car_type_id, price_per_hour FROM vehicles "
                            "	WHERE vehicle_id = :vehicle_id) AS vehicles "
                            "LEFT JOIN car_types ON car_types.car_type_id = vehicles.car_type_id "
                            "LEFT JOIN brands ON brands.brand_id = car_types.brand_id;");
    queryGetCarData.bindValue(":vehicle_id", selectedCarId);

    if(!queryGetCarData.exec()) {
        QSqlError err = queryGetCarData.lastError();
        qDebug() << "Error in NewTripScreen.displayCarParameters():" << err.text();
        return;
    }
    queryGetCarData.next();
    ui->label_RegistrationNumber->setText(queryGetCarData.value(0).toString());
    ui->label_Mileage->setText(queryGetCarData.value(1).toString() + " km");
    ui->label_PricePerHour->setText(queryGetCarData.value(2).toString() + " zł/h");
    ui->label_CarName->setText(queryGetCarData.value(3).toString());
    ui->label_BodyType->setText(queryGetCarData.value(4).toString());
    ui->label_NumberOfSeats->setText(queryGetCarData.value(5).toString());
    ui->label_Horsepower->setText(queryGetCarData.value(6).toString() + "HP");
    ui->label_Gearbox->setText(queryGetCarData.value(7).toString());
    ui->label_Description->setText(queryGetCarData.value(8).toString());
    QPixmap image;
    if (queryGetCarData.value(9).isNull())
        image.load(":/default_pictures/images/types_of_cars/default_car_image_400x228px.jpg");
    else
        image.loadFromData(queryGetCarData.value(9).toByteArray());
    ui->label_CarImage->setPixmap(image.scaled(400, 300, Qt::KeepAspectRatio));

    if (ui->stackedWidget->currentIndex() != 1) ui->stackedWidget->setCurrentIndex(1);
}

void NewTripScreen::loadAllCars() {
    qDebug() << "loadAllCars";

    queryModel->setQuery("SELECT vehicles.vehicle_id, concat(brands.name, ' ', car_types.model) AS car_name, body_type, price_per_hour FROM "
                         "    (SELECT vehicle_id, car_type_id, price_per_hour FROM vehicles "
                         "	  WHERE available = 1) AS vehicles "
                         "LEFT JOIN car_types ON vehicles.car_type_id = car_types.car_type_id "
                         "LEFT JOIN brands ON car_types.brand_id = brands.brand_id;");
    setTableColumName();
    ui->tableView->setColumnWidth(0, 0);
    ui->tableView->setColumnWidth(1, ui->tableView->width()*0.5);
    ui->tableView->setColumnWidth(2, ui->tableView->width()*0.3);
    ui->tableView->setColumnWidth(3, ui->tableView->width()*0.1);
    ui->tableView->hideColumn(0);
}

void NewTripScreen::loadCarsByPattern(QString pattern) {
    qDebug() << "loadCarsByPattern";

    queryModel->setQuery("SELECT vehicles.vehicle_id, concat(brands.name, ' ', car_types.model) AS car_name, body_type, price_per_hour FROM "
                         "    (SELECT vehicle_id, car_type_id, price_per_hour FROM vehicles "
                         "	  WHERE available = 1) AS vehicles "
                         "LEFT JOIN car_types ON vehicles.vehicle_id = car_types.car_type_id "
                         "LEFT JOIN brands ON car_types.brand_id = brands.brand_id "
                         "WHERE concat(brands.name, ' ', car_types.model) LIKE '%" + pattern + "%';");
    //setTableColumName();

    //ui->tableView->hideColumn(0);
}

void NewTripScreen::setTableColumName() {
    queryModel->setHeaderData(1, Qt::Horizontal, "Model");
    queryModel->setHeaderData(2, Qt::Horizontal, "Body");
    queryModel->setHeaderData(3, Qt::Horizontal, "zł/h");
}

void NewTripScreen::openEndTripPage() {
    ui->stackedWidget_Page->setCurrentIndex(1);
}

void NewTripScreen::openNewTripPage() {
    ui->lineEdit->clear();
    ui->stackedWidget_Page->setCurrentIndex(0);
    ui->stackedWidget->setCurrentIndex(0);

    loadAllCars();
}
