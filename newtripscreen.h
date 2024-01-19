#ifndef NEWTRIPSCREEN_H
#define NEWTRIPSCREEN_H

#include <QWidget>
#include <QSqlQueryModel>

namespace Ui {
class NewTripScreen;
}

class NewTripScreen : public QWidget
{
    Q_OBJECT

public:
    explicit NewTripScreen(unsigned int initialUserId, QWidget *parent = nullptr);
    ~NewTripScreen();

signals:
    void backButtoncliked(unsigned int userId);

private slots:
    void on_pushButton_Back_clicked();

    void on_pushButton_EndTrip_clicked();

    void on_pushButton_Search_clicked();

    void on_pushButton_Rent_clicked();

    void displayCarParameters(const QModelIndex& current, const QModelIndex& previous);

private:
    void loadAllCars();

    void loadCarsByPattern(QString pattern);

    void setTableColumName();

private:
    void openEndTripPage();

    void openNewTripPage();

private:
    Ui::NewTripScreen *ui;
    unsigned int userId;
    unsigned int currentRentalId;
    unsigned int selectedCarId;
    QSqlQueryModel* queryModel;
};

#endif // NEWTRIPSCREEN_H
