#include "piece.h"
#include "testpiece.h"

Q_DECLARE_METATYPE(Piece::Type)
Q_DECLARE_METATYPE(Piece::Flavor)

void TestPiece::invalid()
{
    QFETCH(Piece::Type, type);
    QFETCH(Piece::Flavor, flavor);

    try {
        Piece piece(SENTE, type, flavor);
        QVERIFY(false);
    } catch (std::invalid_argument &e) {
        QCOMPARE(e.what(), "piece type and flavor don't match");
    }
}

void TestPiece::invalid_data()
{
    QTest::addColumn<Piece::Type>("type");
    QTest::addColumn<Piece::Flavor>("flavor");

    QTest::newRow("King cannot be promoted") << Piece::KING << Piece::PROMOTED;
    QTest::newRow("Gold cannot be promoted") << Piece::GOLD << Piece::PROMOTED;
    QTest::newRow("Rook cannot be jeweled")  << Piece::ROOK << Piece::JEWELED;
}

void TestPiece::invalidBOD()
{
    QTextStream in(" ");  // too short

    try {
        Piece::loadBOD(in);
        QVERIFY(false);
    } catch (std::runtime_error &e) {
        QCOMPARE(e.what(), "incomplete piece");
    }
}

void TestPiece::unknownBOD()
{
    QFETCH(QString, inBod);

    QTextStream in(&inBod);
    Piece *piece = Piece::loadBOD(in);
    QVERIFY(piece == nullptr);
}

void TestPiece::unknownBOD_data()
{
    QTest::addColumn<QString>("inBod");

    QTest::newRow("Unknown sente piece") << " 竜";
    QTest::newRow("Unknown gote piece")  << "v今";
    QTest::newRow("No piece")            << " ・";
}

void TestPiece::identity()
{
    QFETCH(QString, inBod);

    QTextStream in(&inBod);
    Piece *piece = Piece::loadBOD(in);

    QString outBod;
    QTextStream out(&outBod);
    piece->saveBOD(out);

    delete piece;

    QCOMPARE(outBod, inBod);
}

void TestPiece::identity_data()
{
    QTest::addColumn<QString>("inBod");

    QTest::newRow("Sente king")            << " 王";
    QTest::newRow("Sente jeweled general") << " 玉";
    QTest::newRow("Sente rook")            << " 飛";
    QTest::newRow("Sente promoted rook")   << " 龍";
    QTest::newRow("Gote pawn")             << "v歩";
    QTest::newRow("Gote promoted pawn")    << "vと";
}

void TestPiece::defaultCode()
{
    QFETCH(Piece::Type, type);
    QFETCH(QChar, code);

    QChar actualCode = Piece::defaultCode(type);
    QCOMPARE(actualCode, code);
    QCOMPARE(Piece::type(code), type);
}

void TestPiece::defaultCode_data()
{
    QTest::addColumn<Piece::Type>("type");
    QTest::addColumn<QChar>("code");

    QTest::newRow("King") << Piece::KING << QChar(u'王');
    QTest::newRow("Rook") << Piece::ROOK << QChar(u'飛');
    QTest::newRow("Pawn") << Piece::PAWN << QChar(u'歩');
}

void TestPiece::invalidCode()
{
    try {
        Piece::type(u'玉');
        QVERIFY(false);
    } catch (std::runtime_error &e) {
        QCOMPARE(e.what(), "unknown piece");
    }
}
