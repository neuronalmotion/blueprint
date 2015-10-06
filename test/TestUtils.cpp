#include "TestUtils.h"

void noMessageOutput(QtMsgType, const QMessageLogContext &context, const QString &msg) {}
void TestUtils::toggleLogOutput(bool enable)
{
    if (enable) {
        qInstallMessageHandler(0);
    } else {
        qInstallMessageHandler(noMessageOutput);
    }
}

TestUtils::TestUtils()
{

}

