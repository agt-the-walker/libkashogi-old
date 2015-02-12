#include "testpiece.h"
#include "testposition.h"

int main(int argc, char **argv)
{
    TestPiece testPiece;
    TestPosition testPosition;

    QObject *classes[] = { &testPiece, &testPosition };

    int ret = EXIT_SUCCESS;
    for (auto &klass: classes)
        if (QTest::qExec(klass, argc, argv) != EXIT_SUCCESS)
            ret = EXIT_FAILURE;

    return ret;
}
