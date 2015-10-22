#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <QIODevice>
#include <QString>

namespace blueprint
{

class Blueprint;

class FileUtils
{
public:

    static bool saveBlueprintToFile(const Blueprint& blueprint, const QString& filepath);
    static Blueprint* loadBlueprintFromFile(const QString& filepath);

private:
    FileUtils();
};
}

#endif // FILEUTILS_H
