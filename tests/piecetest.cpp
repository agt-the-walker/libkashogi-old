#include "piece.h"
#include "piecetest.h"

Q_DECLARE_METATYPE(Piece::Type)
Q_DECLARE_METATYPE(Piece::Flavor)

void PieceTest::invalid()
{
    QFETCH(Piece::Type, type);
    QFETCH(Piece::Flavor, flavor);

    try {
        Piece piece(SENTE, type, flavor);
        QVERIFY(false);
    } catch (std::invalid_argument &e) {
        QCOMPARE(e.what(), "Piece type and flavor don't match");
    }
}

void PieceTest::invalid_data()
{
    QTest::addColumn<Piece::Type>("type");
    QTest::addColumn<Piece::Flavor>("flavor");

    QTest::newRow("King cannot be promoted") << Piece::KING << Piece::PROMOTED;
    QTest::newRow("Gold cannot be promoted") << Piece::GOLD << Piece::PROMOTED;
    QTest::newRow("Rook cannot be jeweled")  << Piece::ROOK << Piece::JEWELED;
}

void PieceTest::invalidBOD()
{
    QTextStream in(" ");  // too short

    try {
        Piece::loadBOD(in);
        QVERIFY(false);
    } catch (std::runtime_error &e) {
        QCOMPARE(e.what(), "Incomplete piece");
    }
}

void PieceTest::unknownBOD()
{
    QFETCH(QString, inBod);

    QTextStream in(&inBod);
    Piece *piece = Piece::loadBOD(in);
    QVERIFY(!piece);
}

void PieceTest::unknownBOD_data()
{
    QTest::addColumn<QString>("inBod");

    QTest::newRow("Unknown sente piece") << " 竜";
    QTest::newRow("Unknown gote piece")  << "v个";
    QTest::newRow("No piece")            << " ・";
}

void PieceTest::identity()
{
    QFETCH(QString, inBod);
    QFETCH(QChar, code);

    QTextStream in(&inBod);
    Piece *piece = Piece::loadBOD(in);

    QCOMPARE(piece->code(), code);

    QString outBod;
    QTextStream out(&outBod);
    piece->saveBOD(out);

    delete piece;

    QCOMPARE(outBod, inBod);
}

void PieceTest::identity_data()
{
    QTest::addColumn<QString>("inBod");
    QTest::addColumn<QChar>("code");

    QTest::newRow("Sente king")            << " 王" << QChar(u'王');
    QTest::newRow("Sente jeweled general") << " 玉" << QChar(u'玉');
    QTest::newRow("Sente rook")            << " 飛" << QChar(u'飛');
    QTest::newRow("Sente promoted rook")   << " 龍" << QChar(u'龍');
    QTest::newRow("Gote pawn")             << "v歩" << QChar(u'歩');
    QTest::newRow("Gote promoted pawn")    << "vと" << QChar(u'と');
}

void PieceTest::defaultCode()
{
    QFETCH(Piece::Type, type);
    QFETCH(QChar, code);

    QChar actualCode = Piece::defaultCode(type);
    QCOMPARE(actualCode, code);
    QCOMPARE(Piece::type(code), type);
}

void PieceTest::defaultCode_data()
{
    QTest::addColumn<Piece::Type>("type");
    QTest::addColumn<QChar>("code");

    QTest::newRow("King") << Piece::KING << QChar(u'王');
    QTest::newRow("Rook") << Piece::ROOK << QChar(u'飛');
    QTest::newRow("Pawn") << Piece::PAWN << QChar(u'歩');
    QTest::newRow("Reverse Chariot") << Piece::REVERSE_CHARIOT << QChar(u'反');
}

void PieceTest::invalidCode()
{
    try {
        Piece::type(u'玉');
        QVERIFY(false);
    } catch (std::runtime_error &e) {
        QCOMPARE(e.what(), "Unknown piece");
    }
}
