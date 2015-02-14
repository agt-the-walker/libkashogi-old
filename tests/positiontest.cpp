#include "position.h"
#include "positiontest.h"

void PositionTest::identity()
{
    QFETCH(QString, bodPath);

    QFile file(bodPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        QFAIL("Cannot open file");
    QTextStream in(&file);
    QString inBod = in.readAll();
    in.seek(0);

    QString outBod;
    QTextStream out(&outBod);

    Position position(in);
    position.saveBOD(out);

    QCOMPARE(outBod, inBod);

    QFileInfo fi(bodPath);
    if (fi.baseName() == "empty") {
        QString emptyBod;
        QTextStream out(&emptyBod);

        Position position;
        position.saveBOD(out);

        QCOMPARE(emptyBod, inBod);
    }
}

void PositionTest::identity_data()
{
    QTest::addColumn<QString>("bodPath");

    QDir dir(QT_TESTCASE_BUILDDIR "/../examples");
    for (QString &entry: dir.entryList(QDir::Files))
        QTest::newRow(qPrintable(entry)) << dir.filePath(entry);
}

void PositionTest::invalidBOD()
{
    QFETCH(QString, bodPath);

    QFile file(bodPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        QFAIL("Cannot open file");

    QTextStream in(&file);

    try {
        Position position(in);
        QVERIFY(false);
    } catch (std::runtime_error &e) {
        QFileInfo fi(bodPath);
        QCOMPARE(e.what(), qPrintable(fi.baseName()));
    }
}

void PositionTest::invalidBOD_data()
{
    QTest::addColumn<QString>("bodPath");

    QDir dir(QT_TESTCASE_BUILDDIR "/../examples/invalid");
    for (QString &entry: dir.entryList(QDir::Files))
        QTest::newRow(qPrintable(entry)) << dir.filePath(entry);
}
