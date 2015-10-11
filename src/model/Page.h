#ifndef PAGE_H
#define PAGE_H

#include <QList>

#include "ShapeRectangle.h"

namespace blueprint {
class Canvas;

class Page : public ShapeRectangle
{
public:
    Page(Shape* parentShape = 0);
    ~Page();

    Parcel* toParcel() const override;

};
}

#endif // PAGE_H
