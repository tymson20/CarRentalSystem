#include "lastrentalbar.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QPixmap>

#include <QDebug>

LastRentalBar::LastRentalBar(unsigned int initialUserId, QWidget* parent)
    : BaseBar(3, parent), userId(initialUserId) {}

LastRentalBar::~LastRentalBar() {
    // deallocating memory used by QLabels in vectorLabelText and vectorLabelImage
    for (qsizetype i = 0; i < maxElements; i++) {
        if (vectorLabelText[i]) {
            delete vectorLabelText[i];
            vectorLabelText[i] = nullptr;
        }
        if (vectorLabelImage[i]) {
            delete vectorLabelImage[i];
            vectorLabelImage[i] = nullptr;
        }
    }
    qDebug() << "QLabels memory freed LastRental";
}

void LastRentalBar::fillBar() {
    QSqlQuery queryGetLastRental;
    queryGetLastRental.prepare("SELECT distance, concat(brands.name, ' ', model) AS car_name, "
                               "timediff(end_datetime, start_datetime) AS duration FROM "
                               "    (SELECT * FROM rentals "
                               "    WHERE driver_id = :user_id "
                               "    ORDER BY end_datetime DESC "
                               "    LIMIT 1) AS rental "
                               "LEFT JOIN vehicles ON vehicles.vehicle_id = rental.vehicle_id "
                               "LEFT JOIN car_types ON car_types.car_type_id = vehicles.car_type_id "
                               "LEFT JOIN brands ON brands.brand_id = car_types.brand_id;");
    queryGetLastRental.bindValue(":user_id", userId);
    if (!queryGetLastRental.exec()) {
        QSqlError err = queryGetLastRental.lastError();
        qDebug() << "Error in LastRentalBar.fillBar():" << err.text();
    }
    else {
        if (!queryGetLastRental.next()) {
            // There is no last rental
        }
        else {
            // Distance
            QLabel* labelImage = new QLabel;
            labelImage->setPixmap(QPixmap(":/icons/images/icons/new_trip.png"));
            labelImage->setAlignment(Qt::AlignCenter);
            vectorLabelImage[0] = labelImage;
            gridLayout->addWidget(vectorLabelImage[0], 0, 0);
            QLabel* labelText = new QLabel(queryGetLastRental.value("distance").toString() + " km", this);
            labelText->setAlignment(Qt::AlignCenter);
            vectorLabelText[0] = labelText;
            gridLayout->addWidget(vectorLabelText[0], 1, 0);
            // Car model
            labelImage = new QLabel;
            labelImage->setPixmap(QPixmap(":/icons/images/icons/car.png"));
            labelImage->setAlignment(Qt::AlignCenter);
            vectorLabelImage[1] = labelImage;
            gridLayout->addWidget(vectorLabelImage[1], 0, 1);
            labelText = new QLabel(queryGetLastRental.value("car_name").toString(), this);
            labelText->setAlignment(Qt::AlignCenter);
            vectorLabelText[1] = labelText;
            gridLayout->addWidget(vectorLabelText[1], 1, 1);
            // Duration
            labelImage = new QLabel;
            labelImage->setPixmap(QPixmap(":/icons/images/icons/clock.png"));
            labelImage->setAlignment(Qt::AlignCenter);
            vectorLabelImage[2] = labelImage;
            gridLayout->addWidget(vectorLabelImage[2], 0, 2);
            labelImage = nullptr;
            labelText = new QLabel(queryGetLastRental.value("duration").toString(), this);
            labelText->setAlignment(Qt::AlignCenter);
            vectorLabelText[2] = labelText;
            labelText = nullptr;
            gridLayout->addWidget(vectorLabelText[2], 1, 2);
        }
    }
}
