#include "Tool.h"

using namespace blueprint;

Tool::Tool(const Tool::Type& type, QAction* action)
    : mType(type),
      mAction(action)
{
}

Tool::~Tool()
{
    mAction = nullptr;
}

Shape::ShapeType Tool::shapeType(const Type& toolType)
{
    Shape::ShapeType shapeType;
    switch (toolType) {
    case Tool::Type::RECTANGLE:
        shapeType = Shape::ShapeType::RECTANGLE;
        break;

    case Tool::Type::ELLIPSE:
        shapeType = Shape::ShapeType::ELLIPSE;
        break;

    case Tool::Type::LINE:
        shapeType = Shape::ShapeType::LINE;
        break;

    case Tool::Type::TEXT:
        shapeType = Shape::ShapeType::TEXT;
        break;
    default:
        qFatal("Trying to convert invalid source tool: %d", toolType);
        break;
    }
    return shapeType;
}

Shape::ShapeType Tool::shapeType() const
{
    return Tool::shapeType(mType);
}



