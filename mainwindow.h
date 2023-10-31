#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QSqlDatabase>
#include <QSqlError>

#include "loginscreen.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QSqlDatabase* db;
    QVBoxLayout* boxLayout;
    QStackedWidget* stackedWidget;
    LoginScreen* loginScreen;
};
#endif // MAINWINDOW_H
