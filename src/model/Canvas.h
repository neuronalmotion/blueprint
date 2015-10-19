#ifndef CANVAS_H
#define CANVAS_H

#include <QList>

#include "ShapeBezier.h"

namespace blueprint {

class Canvas : public ShapeBezier
{
public:
    Canvas(Shape* parentShape = nullptr);
    ~Canvas();

    Parcel* toParcel() const override;

};
}

#endif // CANVAS_H
