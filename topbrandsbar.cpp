#include "topbrandsbar.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QPixmap>

#include <QDebug>

TopBrandsBar::TopBrandsBar(qsizetype initialMaxElements, QWidget *parent)
    : BaseBar(initialMaxElements, parent)
{
    currentNumberOfElements = 0;
    horizontalSpacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
}

TopBrandsBar::~TopBrandsBar() {
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
    delete horizontalSpacer;
    horizontalSpacer = nullptr;
    qDebug() << "QLabels memory freed TopBrands";
}

qsizetype TopBrandsBar::getCurrentNumberOfElements() const {
    return currentNumberOfElements;
}

void TopBrandsBar::fillBar() {
    QSqlQuery queryGetTopBrands;
    queryGetTopBrands.prepare("SELECT name, logo FROM rentals "
                              "LEFT JOIN vehicles ON vehicles.vehicle_id = rentals.vehicle_id "
                              "LEFT JOIN car_types ON car_types.car_type_id = vehicles.car_type_id "
                              "LEFT JOIN brands ON brands.brand_id = car_types.brand_id "
                              "GROUP BY brands.brand_id "
                              "ORDER BY count(*) DESC "
                              "LIMIT :limit;");
    queryGetTopBrands.bindValue(":limit", maxElements);
    if (!queryGetTopBrands.exec()) {
        QSqlError err = queryGetTopBrands.lastError();
        qDebug() << "Error in TopBrandsBar.fillBar():" << err.text();
    }
    else {
        qsizetype i = 0;
        while(queryGetTopBrands.next()) {
            QLabel* labelName = new QLabel(queryGetTopBrands.value("name").toString(), this);
            labelName->setAlignment(Qt::AlignCenter);
            vectorLabelText[i] = labelName;
            labelName = nullptr;
            if (!queryGetTopBrands.value("logo").isNull()) {
                QPixmap pixmap;
                pixmap.loadFromData(queryGetTopBrands.value("logo").toByteArray());
                QLabel* labelLogo = new QLabel(this);
                labelLogo->setPixmap(pixmap);
                labelLogo->setAlignment(Qt::AlignCenter);
                vectorLabelImage[i] = labelLogo;
                labelLogo = nullptr;
                gridLayout->addWidget(vectorLabelImage[i], 0, i);
            }
            else {
                // load default brand image from resource file
                QPixmap defaultBrandImage(":/default_pictures/images/brands_logos/no_image_64x64px.jpg");
                vectorLabelImage[i] = new QLabel(this);
                vectorLabelImage[i]->setPixmap(defaultBrandImage);
                gridLayout->addWidget(vectorLabelImage[i], 0, i);
            }
            //gridLayout->addWidget(vectorLabelImage[i], 0, i);
            gridLayout->addWidget(vectorLabelText[i], 1, i);
            i++;
        }
        currentNumberOfElements = i;
        gridLayout->addItem(horizontalSpacer, 0, currentNumberOfElements, 2, 1);
    }
}
