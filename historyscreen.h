#ifndef HISTORYSCREEN_H
#define HISTORYSCREEN_H

#include <QWidget>
#include <QSqlQueryModel>

namespace Ui {
class HistoryScreen;
}

class HistoryScreen : public QWidget
{
    Q_OBJECT

public:
    explicit HistoryScreen(unsigned int initialUserId, QWidget *parent = nullptr);
    ~HistoryScreen();

signals:
    void backButtoncliked(unsigned int userId);

private slots:
    void on_pushButton_Back_clicked();

private:
    Ui::HistoryScreen *ui;
    unsigned int userId;
    QSqlQueryModel* queryModel;
};

#endif // HISTORYSCREEN_H
