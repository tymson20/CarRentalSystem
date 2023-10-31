#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSqlDatabase db_conn = QSqlDatabase::addDatabase("QMYSQL");
    db_conn.setHostName("127.0.0.1");
    db_conn.setDatabaseName("test");
    db_conn.setUserName("root");
    db_conn.setPassword("Password4321");
    db = &db_conn;

    if (db->open()) {
        qDebug() << "Connected";
    }
    else {
        QSqlError err = db->lastError();
        qDebug() << "Not connected. Error:" << err.text() << "\n";
    }

    boxLayout = new QVBoxLayout;
    ui->centralwidget->setLayout(boxLayout);

    // Screens
    loginScreen = new LoginScreen(db);

    stackedWidget = new QStackedWidget;
    stackedWidget->addWidget(loginScreen);
    boxLayout->addWidget(stackedWidget);

    stackedWidget->setCurrentWidget(loginScreen);
}

MainWindow::~MainWindow()
{
    delete ui;
}

