#ifndef BASEBAR_H
#define BASEBAR_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QVector>

class BaseBar : public QWidget
{
    Q_OBJECT
public:
    explicit BaseBar(qsizetype initialMaxElements, QWidget *parent = nullptr);
    virtual ~BaseBar();

    qsizetype getMaxElements() const;

    virtual void fillBar() = 0;

signals:

protected:
    QGridLayout* gridLayout = nullptr;
    const qsizetype maxElements; // maximal number of elements in bar
    QVector<QLabel*> vectorLabelText;
    QVector<QLabel*> vectorLabelImage;
};

#endif // BASEBAR_H
