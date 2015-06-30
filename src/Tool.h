#ifndef TOOL_H
#define TOOL_H

#include <QAction>

class Tool
{
public:

    enum Type {
        SELECTION,
        ELLIPSE,
        LINE,
        RECTANGLE
    };

    Tool(const Type& type, QAction* action);
    ~Tool();

    inline Type getType() const { return mType; }
    inline QAction* getAction() const { return mAction; }
    inline void setChecked(bool checked) { mAction->setChecked(checked); }

private:
    Type mType;
    QAction* mAction;
};

#endif // TOOL_H
