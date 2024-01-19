#ifndef FAVOURITECARTYPESBAR_H
#define FAVOURITECARTYPESBAR_H

#include "basebar.h"

class FavouriteCarTypesBar : public BaseBar
{
public:
    explicit FavouriteCarTypesBar(qsizetype initialMaxElements, unsigned int initialUserId, QWidget* parent = nullptr);
    ~FavouriteCarTypesBar();

    qsizetype getCurrentNumberOfElements() const;

    void fillBar() override;

private:
    qsizetype currentNumberOfElements;
    unsigned int userId;
};

#endif // FAVOURITECARTYPESBAR_H
