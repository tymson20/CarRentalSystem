#ifndef LOGINSCREEN_H
#define LOGINSCREEN_H

#include <QWidget>
#include <QSqlDatabase>

namespace Ui {
class LoginScreen;
}

class LoginScreen : public QWidget
{
    Q_OBJECT

public:
    explicit LoginScreen(QSqlDatabase* db_conn, QWidget *parent = nullptr);
    ~LoginScreen();

private:
    Ui::LoginScreen *ui;
    QSqlDatabase* db;
};

#endif // LOGINSCREEN_H