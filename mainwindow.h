#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QVBoxLayout>

#include "loginscreen.h"
#include "signupscreen.h"
#include "mainscreen.h"
#include "historyscreen.h"
#include "newtripscreen.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void changeWidgetToSignUpScreen();

    void changeWidgetToLoginScreen();

    void changeWidgetToMainScreen(unsigned int user_id);

    void changeWidgetToHistoryScreen(unsigned int userId);

    void changeWidgetToNewTripScreen(unsigned int userId);

private:
    Ui::MainWindow *ui;
    QVBoxLayout* boxLayout;
    QStackedWidget* stackedWidget;
    LoginScreen* loginScreen;
    SignUpScreen* signUpScreen;
    MainScreen* mainScreen;
    HistoryScreen* historyScreen;
    NewTripScreen* newTripScreen;
};
#endif // MAINWINDOW_H
