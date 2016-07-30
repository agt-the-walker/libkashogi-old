#include "piece.h"
#include "piecetest.h"

Q_DECLARE_METATYPE(Piece::Flavor)

void PieceTest::invalid()
{
    QFETCH(QChar, type);
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
    QTest::addColumn<QChar>("type");
    QTest::addColumn<Piece::Flavor>("flavor");

    QTest::newRow("King cannot be promoted") << QChar(u'王') << Piece::PROMOTED;
    QTest::newRow("Gold cannot be promoted") << QChar(u'金') << Piece::PROMOTED;
    QTest::newRow("Rook cannot be jeweled")  << QChar(u'飛') << Piece::JEWELED;
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
    QFETCH(QChar, kanji);

    QTextStream in(&inBod);
    Piece *piece = Piece::loadBOD(in);

    QCOMPARE(piece->kanji(), kanji);

    QString outBod;
    QTextStream out(&outBod);
    piece->saveBOD(out);

    delete piece;

    QCOMPARE(outBod, inBod);
}

void PieceTest::identity_data()
{
    QTest::addColumn<QString>("inBod");
    QTest::addColumn<QChar>("kanji");

    QTest::newRow("Sente king")            << " 王" << QChar(u'王');
    QTest::newRow("Sente jeweled general") << " 玉" << QChar(u'玉');
    QTest::newRow("Sente rook")            << " 飛" << QChar(u'飛');
    QTest::newRow("Sente promoted rook")   << " 龍" << QChar(u'龍');
    QTest::newRow("Gote pawn")             << "v歩" << QChar(u'歩');
    QTest::newRow("Gote promoted pawn")    << "vと" << QChar(u'と');
}

void PieceTest::accessors()
{
    QFETCH(QChar, type);
    QFETCH(Piece::Flavor, flavor);
    QFETCH(bool, isKing);

    Piece piece(SENTE, type, flavor);
    QCOMPARE(piece.type(), type);
    QCOMPARE(piece.player(), SENTE);
    QCOMPARE(Piece::isKing(type), isKing);
}

void PieceTest::accessors_data()
{
    QTest::addColumn<QChar>("type");
    QTest::addColumn<Piece::Flavor>("flavor");
    QTest::addColumn<bool>("isKing");

    QTest::newRow("King")            << QChar(u'王') << Piece::DEFAULT  << true;
    QTest::newRow("Jeweled general") << QChar(u'王') << Piece::JEWELED  << true;
    QTest::newRow("Rook")            << QChar(u'飛') << Piece::DEFAULT  << false;
    QTest::newRow("Promoted rook")   << QChar(u'飛') << Piece::PROMOTED << false;
    QTest::newRow("Pawn")            << QChar(u'歩') << Piece::DEFAULT  << false;
    QTest::newRow("Promoted pawn")   << QChar(u'歩') << Piece::PROMOTED << false;
    QTest::newRow("Reverse Chariot") << QChar(u'反') << Piece::DEFAULT  << false;
}

void PieceTest::invalidType()
{
    try {
        Piece::typeIndex(u'玉');
        QVERIFY(false);
    } catch (std::invalid_argument &e) {
        QCOMPARE(e.what(), "Unknown piece type");
    }
}
