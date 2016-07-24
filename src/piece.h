#ifndef PIECE_H
#define PIECE_H

#include <QTextStream>

#include "global.h"

class Piece
{
public:
    enum Type { KING, ROOK, BISHOP, GOLD, SILVER, KNIGHT, LANCE, PAWN,
                FALCON, CRANE, PHEASANT, LEFT_QUAIL, RIGHT_QUAIL, SWALLOW,
                QUEEN, FIDE_KNIGHT, REVERSE_CHARIOT,
                NB_TYPES };
    enum Flavor { DEFAULT, PROMOTED, JEWELED, NB_FLAVORS };

    Piece(Player player, Type type, Flavor flavor = DEFAULT);

    QChar code() const;
    Player player() const { return mPlayer; };

    static Piece *loadBOD(QTextStream &stream);
    void saveBOD(QTextStream &stream) const;

    static QChar defaultCode(Type type);
    static Type type(QChar defaultCode);

private:
    Player mPlayer;
    Type mType;
    Flavor mFlavor;
};

#endif
