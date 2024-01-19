#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("carrentalsystem");
    db.setUserName("root");
    db.setPassword("Password4321");

    if (db.open()) {
        qDebug() << "Connected";
    }
    else {
        QSqlError err = db.lastError();
        qDebug() << "Not connected. Error:" << err.text() << "\n";
    }

    QIcon icon(":/icons/images/icons/car-rental.png");
    setWindowIcon(icon);
    setWindowTitle("Car Rental System");

    const QString styleSheet = "MainWindow {background-color: #fafafa}";
    setStyleSheet(styleSheet);

    boxLayout = new QVBoxLayout;
    ui->centralwidget->setLayout(boxLayout);

    // Screens
    loginScreen = new LoginScreen;
    signUpScreen = nullptr;
    mainScreen = nullptr;
    historyScreen = nullptr;
    newTripScreen = nullptr;

    stackedWidget = new QStackedWidget;
    stackedWidget->addWidget(loginScreen);
    boxLayout->addWidget(stackedWidget);

    stackedWidget->setCurrentWidget(loginScreen);

    // connections
    QObject::connect(loginScreen, &LoginScreen::signUpButtonClicked, this, &MainWindow::changeWidgetToSignUpScreen);
    QObject::connect(loginScreen, &LoginScreen::loggedSuccessfully, this, &MainWindow::changeWidgetToMainScreen);
}

MainWindow::~MainWindow()
{
    delete ui;
    if (mainScreen) {
        delete mainScreen;
        mainScreen = nullptr;
    }
}

void MainWindow::changeWidgetToSignUpScreen() {
    if (!signUpScreen) {
        signUpScreen = new SignUpScreen;
        stackedWidget->addWidget(signUpScreen);

        QObject::connect(signUpScreen, &SignUpScreen::changeToLoginScreen, this, &MainWindow::changeWidgetToLoginScreen);
    }
    stackedWidget->setCurrentWidget(signUpScreen);
}

void MainWindow::changeWidgetToLoginScreen() {
    if (!loginScreen) {
        loginScreen = new LoginScreen;
        stackedWidget->addWidget(loginScreen);
    }
    stackedWidget->setCurrentWidget(loginScreen);
}

void MainWindow::changeWidgetToMainScreen(unsigned int user_id) {
    if (!mainScreen) {
        mainScreen = new MainScreen(user_id);
        stackedWidget->addWidget(mainScreen);

        QObject::connect(mainScreen, &MainScreen::historyButtonClicked, this, &MainWindow::changeWidgetToHistoryScreen);
        QObject::connect(mainScreen, &MainScreen::newTripButtonClicked, this, &MainWindow::changeWidgetToNewTripScreen);
    }
    mainScreen->updateUserData();
    stackedWidget->setCurrentWidget(mainScreen);
    if (signUpScreen) {
        delete signUpScreen;
        signUpScreen = nullptr;
    }
}

void MainWindow::changeWidgetToHistoryScreen(unsigned int userId) {
    if (!historyScreen) {
        historyScreen = new HistoryScreen(userId);
        stackedWidget->addWidget(historyScreen);

        QObject::connect(historyScreen, &HistoryScreen::backButtoncliked, this, &MainWindow::changeWidgetToMainScreen);
    }
    stackedWidget->setCurrentWidget(historyScreen);
}

void MainWindow::changeWidgetToNewTripScreen(unsigned int userId) {
    if (!newTripScreen) {
        newTripScreen = new NewTripScreen(userId, this);
        stackedWidget->addWidget(newTripScreen);

        QObject::connect(newTripScreen, &NewTripScreen::backButtoncliked, this, &MainWindow::changeWidgetToMainScreen);
    }
    stackedWidget->setCurrentWidget(newTripScreen);
}
