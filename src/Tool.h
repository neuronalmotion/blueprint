#ifndef TOOL_H
#define TOOL_H

#include <QAction>

#include "model/Shape.h"

namespace blueprint {

class Tool
{
public:

    enum Type {
        SELECTION,
        ELLIPSE,
        LINE,
        RECTANGLE,
        TEXT,
    };

    Tool(const Type& type, QAction* action);
    ~Tool();

    static Shape::ShapeType shapeType(const Tool::Type& toolType);
    Shape::ShapeType shapeType() const;

    inline Type getType() const { return mType; }
    inline QAction* getAction() const { return mAction; }
    inline void setChecked(bool checked) { mAction->setChecked(checked); }

private:
    Type mType;
    QAction* mAction;
};

}

#endif // TOOL_H
