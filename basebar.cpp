#include "basebar.h"

BaseBar::BaseBar(qsizetype initialMaxElements, QWidget *parent)
    : QWidget(parent),
    maxElements(initialMaxElements)
{
    gridLayout = new QGridLayout(this);
    vectorLabelText = QVector<QLabel*>(maxElements, nullptr);
    vectorLabelImage = QVector<QLabel*>(maxElements, nullptr);

    // tests
    /*
    setAutoFillBackground(true);
    QPalette palette = QPalette();
    palette.setColor(QPalette::Window, Qt::blue);
    setPalette(palette);
    */
}

BaseBar::~BaseBar() {
    // deallocating memory used by gridLayout
    if (gridLayout) {
        delete gridLayout;
        gridLayout = nullptr;
    }
    qDebug() << "GridLayout memory freed";
}

qsizetype BaseBar::getMaxElements() const {
    return maxElements;
}
