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

