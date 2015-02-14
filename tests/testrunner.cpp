#include "piecetest.h"
#include "positiontest.h"

int main(int argc, char **argv)
{
    PieceTest pieceTests;
    PositionTest positionTests;

    QObject *classes[] = { &pieceTests, &positionTests };

    int ret = EXIT_SUCCESS;
    for (auto &klass: classes)
        if (QTest::qExec(klass, argc, argv) != EXIT_SUCCESS)
            ret = EXIT_FAILURE;

    return ret;
}
