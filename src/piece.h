#ifndef PIECE_H
#define PIECE_H

#include <QRegularExpression>
#include <QTextStream>

#include "global.h"

class Piece
{
public:
    enum Flavor { DEFAULT, PROMOTED, JEWELED, NB_FLAVORS };

    Piece(const Player player, const QChar type, const Flavor flavor = DEFAULT);

    Player player() const { return mPlayer; };
    unsigned int typeIndex() const { return mTypeIndex; };

    QChar type() const;   // for instance: 王 or 歩
    QChar kanji() const;  // for instance: 玉 or と

    static Piece *loadBOD(QTextStream &stream);
    void saveBOD(QTextStream &stream) const;

    static bool isKing(const QChar type) { return type == u'王'; };
    static unsigned int typeIndex(const QChar type);

private:
    Player mPlayer;
    unsigned int mTypeIndex;
    Flavor mFlavor;
};

#endif
