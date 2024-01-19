#ifndef LOGINSCREEN_H
#define LOGINSCREEN_H

#include <QWidget>
#include <QPixmap>

namespace Ui {
class LoginScreen;
}

class LoginScreen : public QWidget
{
    Q_OBJECT

public:
    explicit LoginScreen(QWidget *parent = nullptr);
    ~LoginScreen();

signals:
    void signUpButtonClicked();

    void loggedSuccessfully(unsigned int user_id);

private slots:
    void on_pushButton_SignUp_clicked();

    void on_pushButton_LogIn_clicked();

private:
    Ui::LoginScreen *ui;
    QPixmap pixmap;
};

#endif // LOGINSCREEN_H
