#include "favouritecartypesbar.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QPixmap>

#include <QDebug>

FavouriteCarTypesBar::FavouriteCarTypesBar(qsizetype initialMaxElements, unsigned int initialUserId, QWidget* parent)
    : BaseBar(initialMaxElements, parent), userId(initialUserId)
{
    currentNumberOfElements = 0;
}

FavouriteCarTypesBar::~FavouriteCarTypesBar() {
    // deallocating memory used by QLabels in vectorLabelText and vectorLabelImage
    for (qsizetype i = 0; i < currentNumberOfElements; i++) {
        if (vectorLabelText[i]) {
            delete vectorLabelText[i];
            vectorLabelText[i] = nullptr;
        }
        if (vectorLabelImage[i]) {
            delete vectorLabelImage[i];
            vectorLabelImage[i] = nullptr;
        }
    }
    qDebug() << "QLabels memory freed FavouriteCarTypes";
}

qsizetype FavouriteCarTypesBar::getCurrentNumberOfElements() const
{
    return currentNumberOfElements;
}

void FavouriteCarTypesBar::fillBar() {
    QSqlQuery queryGetFavouriteCarTypes;
    queryGetFavouriteCarTypes.prepare("SELECT concat(name, ' ', model) AS car_name, image FROM favourite_car_types "
                                      "LEFT JOIN car_types ON car_types.car_type_id = favourite_car_types.car_type_id "
                                      "LEFT JOIN brands ON brands.brand_id = car_types.brand_id "
                                      "WHERE user_id = :user_id "
                                      "LIMIT :limit;");
    queryGetFavouriteCarTypes.bindValue(":user_id", userId);
    queryGetFavouriteCarTypes.bindValue(":limit", maxElements);
    if (!queryGetFavouriteCarTypes.exec()) {
        QSqlError err = queryGetFavouriteCarTypes.lastError();
        qDebug() << "Error in FavouriteCarTypesBar.fillBar():" << err.text();
    }
    else {
        qsizetype i = 0;
        while (queryGetFavouriteCarTypes.next()) {
            QLabel* labelName = new QLabel(queryGetFavouriteCarTypes.value("car_name").toString(), this);
            labelName->setAlignment(Qt::AlignCenter);
            vectorLabelText[i] = labelName;
            labelName = nullptr;
            QLabel* labelImage = new QLabel(this);
            if (!queryGetFavouriteCarTypes.value("image").isNull()) {
                QPixmap image;
                image.loadFromData(queryGetFavouriteCarTypes.value("image").toByteArray());
                labelImage->setPixmap(image.scaledToHeight(64, Qt::SmoothTransformation));
            }
            else {
                // load default car_type image from resource file
                QPixmap defaultImage(":/default_pictures/images/default_car_85x64px.png");
                labelImage->setPixmap(defaultImage);
            }
            labelImage->setAlignment(Qt::AlignCenter);
            //labelImage->setStyleSheet("background-color: gray");
            vectorLabelImage[i] = labelImage;
            labelImage = nullptr;
            gridLayout->addWidget(vectorLabelImage[i], 0, i);
            //gridLayout->addWidget(vectorLabelImage[i], 0, i);
            gridLayout->addWidget(vectorLabelText[i], 1, i);
            i++;
        }
        currentNumberOfElements = i;
    }
}
