#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include <QWidget>
#include <QPixmap>
#include "topbrandsbar.h"
#include "favouritecartypesbar.h"
#include "lastrentalbar.h"

namespace Ui {
class MainScreen;
}

class MainScreen : public QWidget
{
    Q_OBJECT

public:
    explicit MainScreen(unsigned int user_id, QWidget *parent = nullptr);
    ~MainScreen();

    unsigned int getUser_id() const;

    void updateUserData();

signals:
    void historyButtonClicked(unsigned int userId);

    void newTripButtonClicked(unsigned int userId);

private slots:
    void on_pushButton_History_clicked();

    void on_pushButton_NewTrip_clicked();

private:
    Ui::MainScreen *ui;
    unsigned int user_id;
    QString email;
    QString first_name;
    QString last_name;
    QString phone_number;
    float covered_distance;
    QPixmap avatar;
    TopBrandsBar* topBrandsBar;
    FavouriteCarTypesBar* favouriteCarTypesBar;
    LastRentalBar* lastRentalBar;
};

#endif // MAINSCREEN_H
