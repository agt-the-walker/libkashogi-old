#include "position.h"
#include "testposition.h"

void TestPosition::empty()
{
    QFile file("examples/empty.bod");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        QFAIL("Cannot open file");
    QTextStream in(&file);
    QString inBod = in.readAll();

    QString outBod;
    QTextStream out(&outBod);

    Position position;
    position.saveBOD(out);

    QCOMPARE(outBod, inBod);
}

void TestPosition::identity()
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
}

void TestPosition::identity_data()
{
    QTest::addColumn<QString>("bodPath");

    QDir dir("examples");
    for (QString &entry: dir.entryList(QDir::Files))
        QTest::newRow(qPrintable(entry)) << dir.filePath(entry);
}
