#ifndef TOPBRANDSBAR_H
#define TOPBRANDSBAR_H

#include "basebar.h"

class TopBrandsBar : public BaseBar
{
public:
    explicit TopBrandsBar(qsizetype initialMaxElements, QWidget *parent = nullptr);
    ~TopBrandsBar();

    qsizetype getCurrentNumberOfElements() const;

    void fillBar() override;

signals:

private:
    qsizetype currentNumberOfElements;
    QSpacerItem* horizontalSpacer;
};

#endif // TOPBRANDSBAR_H
