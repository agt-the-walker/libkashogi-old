#ifndef PIECE_H
#define PIECE_H

#include <QTextStream>

#include "global.h"

class Piece
{
public:
    enum Type { KING, ROOK, BISHOP, GOLD, SILVER, KNIGHT, LANCE, PAWN,
                NB_TYPES };
    enum Flavor { NORMAL, PROMOTED, JEWELED, NB_FLAVORS };

    Piece(Player player, Type type, Flavor flavor = NORMAL);

    static Piece *loadBOD(QTextStream &stream);
    void saveBOD(QTextStream &stream) const;

private:
    Player mPlayer;
    Type mType;
    Flavor mFlavor;
};

#endif
