#include "position.h"
#include "positiontest.h"

Q_DECLARE_METATYPE(Player)
Q_DECLARE_METATYPE(Piece::Type)

void PositionTest::at()
{
    QFETCH(int, row);
    QFETCH(int, column);
    QFETCH(Player, player);
    QFETCH(QChar, code);

    QFile file(QStringLiteral(QT_TESTCASE_BUILDDIR) +
               QLatin1String("/../examples/start.bod"));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        QFAIL("Cannot open file");

    QTextStream in(&file);
    Position position;
    position.loadBOD(in);

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

void PositionTest::nbCaptured()
{
    QFETCH(Player, player);
    QFETCH(Piece::Type, type);
    QFETCH(unsigned int, nbCaptures);

    QFile file(QStringLiteral(QT_TESTCASE_BUILDDIR) +
               QLatin1String("/../examples/captures.bod"));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        QFAIL("Cannot open file");

    QTextStream in(&file);
    Position position;
    position.loadBOD(in);

    QCOMPARE(position.nbCaptured(player, type), nbCaptures);
}

void PositionTest::nbCaptured_data()
{
    QTest::addColumn<Player>("player");
    QTest::addColumn<Piece::Type>("type");
    QTest::addColumn<unsigned int>("nbCaptures");

    QTest::newRow("Sente rooks") << SENTE << Piece::ROOK << 2u;
    QTest::newRow("Sente pawns") << SENTE << Piece::PAWN << 18u;
    QTest::newRow("Gote rooks")  << GOTE  << Piece::ROOK << 0u;
    QTest::newRow("Gote pawns")  << GOTE  << Piece::PAWN << 0u;
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

    Position position;
    position.loadBOD(in);
    position.saveBOD(out);

    QCOMPARE(outBod, inBod);

    QFileInfo fi(bodPath);
    if (fi.baseName() == QLatin1String("empty")) {
        QString emptyBod;
        QTextStream out(&emptyBod);

        Position position;
        position.saveBOD(out);

        QCOMPARE(emptyBod, inBod);
    }

    fi = fi.dir().path() + QStringLiteral("/variant/") + fi.fileName();
    if (fi.exists()) {
        QFile file(fi.filePath());
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            QFAIL("Cannot open variant file");
        QTextStream in(&file);

        QString variantBod;
        QTextStream out(&variantBod);

        Position position;
        position.loadBOD(in);
        position.saveBOD(out);

        QCOMPARE(variantBod, inBod);
    }
}

void PositionTest::identity_data()
{
    QTest::addColumn<QString>("bodPath");

    QDir dir(QStringLiteral(QT_TESTCASE_BUILDDIR) +
             QLatin1String("/../examples"));
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
    Position position;

    try {
        position.loadBOD(in);
        QVERIFY(false);
    } catch (std::runtime_error &e) {
        QFileInfo fi(bodPath);
        QCOMPARE(e.what(), qPrintable(fi.baseName()));
    }
}

void PositionTest::invalidBOD_data()
{
    QTest::addColumn<QString>("bodPath");

    QDir dir(QStringLiteral(QT_TESTCASE_BUILDDIR) +
             QLatin1String("/../examples/invalid"));
    for (QString &entry: dir.entryList(QDir::Files))
        QTest::newRow(qPrintable(entry)) << dir.filePath(entry);
}
