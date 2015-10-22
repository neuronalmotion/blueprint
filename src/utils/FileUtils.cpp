#include "FileUtils.h"

#include <QFile>

#include "io/Parcel.h"
#include "io/XmlInputOutput.h"
#include "model/Blueprint.h"

using namespace blueprint;

FileUtils::FileUtils()
{

}

bool FileUtils::saveBlueprintToFile(const Blueprint& blueprint, const QString& filepath)
{
    Q_ASSERT(!filepath.isNull());
    QFile output(filepath);
    output.open(QIODevice::WriteOnly);

    XmlInputOutput xml;
    Parcel* parcel = blueprint.toParcel();
    xml.write(output, *parcel);
    delete parcel;

    output.close();
    // TODO really handle error cases...
    return true;
}

Blueprint* FileUtils::loadBlueprintFromFile(const QString& filepath)
{
    QFile input(filepath);
    input.open(QIODevice::ReadOnly);

    XmlInputOutput xml;
    Parcel* parcel = xml.read(input);

    Blueprint* blueprint = new Blueprint();
    blueprint->fromParcel(*parcel);
    delete parcel;

    input.close();
    return blueprint;
}
