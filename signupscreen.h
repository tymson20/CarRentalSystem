#ifndef SIGNUPSCREEN_H
#define SIGNUPSCREEN_H

#include <QWidget>
#include <QSqlDatabase>

namespace Ui {
class SignUpScreen;
}

class SignUpScreen : public QWidget
{
    Q_OBJECT

public:
    explicit SignUpScreen(QSqlDatabase* db_conn, QWidget *parent = nullptr);
    ~SignUpScreen();

private:
    Ui::SignUpScreen *ui;
    QSqlDatabase* db;
};

#endif // SIGNUPSCREEN_H
