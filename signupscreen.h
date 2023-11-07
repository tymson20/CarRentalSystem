#ifndef SIGNUPSCREEN_H
#define SIGNUPSCREEN_H

#include <QWidget>

namespace Ui {
class SignUpScreen;
}

class SignUpScreen : public QWidget
{
    Q_OBJECT

public:
    explicit SignUpScreen(QWidget *parent = nullptr);
    ~SignUpScreen();

signals:
    void changeToLoginScreen();

private slots:
    void on_pushButton_LogIn_clicked();

    void on_pushButton_SignUp_clicked();

private:
    Ui::SignUpScreen *ui;
};

#endif // SIGNUPSCREEN_H
