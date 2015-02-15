#include "position.h"
#include "positiontest.h"

Q_DECLARE_METATYPE(Player)

void PositionTest::at()
{
    QFETCH(int, row);
    QFETCH(int, column);
    QFETCH(Player, player);
    QFETCH(QChar, code);

    QFile file(QT_TESTCASE_BUILDDIR "/../examples/start.bod");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        QFAIL("Cannot open file");

    QTextStream in(&file);
    Position position(in);

    const Piece *piece = position.at(row, column);
    QCOMPARE(piece->player(), player);
    QCOMPARE(piece->code(), code);
}

void PositionTest::at_data()
{
    QTest::addColumn<int>("row");
    QTest::addColumn<int>("column");
    QTest::addColumn<Player>("player");
    QTest::addColumn<QChar>("code");

    QTest::newRow("Sente jeweled general") << 9 << 5 << SENTE << QChar(u'玉');
    QTest::newRow("Sente rook")            << 8 << 2 << SENTE << QChar(u'飛');
    QTest::newRow("Gote bishop")           << 2 << 2 << GOTE  << QChar(u'角');
    QTest::newRow("Gote king")             << 1 << 5 << GOTE  << QChar(u'王');
}

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
