#ifndef LASTRENTALBAR_H
#define LASTRENTALBAR_H

#include "basebar.h"


class LastRentalBar : public BaseBar
{
public:
    explicit LastRentalBar(unsigned int initialUserId, QWidget* parent = nullptr);
    ~LastRentalBar();

    void fillBar() override;

private:
    unsigned int userId;
};

#endif // LASTRENTALBAR_H
