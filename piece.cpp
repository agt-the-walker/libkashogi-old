#include <stdexcept>
#include <QDebug>

#include "piece.h"

static const int flavorCodes[Piece::NB_TYPES][Piece::NB_FLAVORS] = {
    { u'王',    0 , u'玉'},
    { u'飛', u'龍',    0 },
    { u'角', u'馬',    0 },
    { u'金',    0 ,    0 },
    { u'銀', u'全',    0 },
    { u'桂', u'圭',    0 },
    { u'香', u'杏',    0 },
    { u'歩', u'と',    0 },
};

Piece::Piece(Player player, Type type, Flavor flavor) {
    if (player < NB_PLAYERS)
        mPlayer = player;
    else
        throw std::invalid_argument("unknown player");

    if (type < NB_TYPES)
        mType = type;
    else
        throw std::invalid_argument("unknown piece type");

    if (flavor < NB_FLAVORS)
        mFlavor = flavor;
    else
        throw std::invalid_argument("unknown piece flavor");

    if (flavorCodes[type][flavor] == 0)
        throw std::invalid_argument("piece type and flavor don't match");
}

Piece *Piece::loadBOD(QTextStream &stream)
{
    QString buffer = stream.read(2);
    if (buffer.size() < 2)
        throw std::invalid_argument("incomplete piece");

    Player player = (buffer[0] == 'v' ? GOTE : SENTE);

    for (int type = 0; type < NB_TYPES; type++)
        for (int flavor = 0; flavor < NB_FLAVORS; flavor++)
            if (buffer[1] == flavorCodes[type][flavor])
                return new Piece(player, static_cast<Type>(type),
                                         static_cast<Flavor>(flavor));

    return nullptr;
}

void Piece::saveBOD(QTextStream &stream) const
{
    stream << (mPlayer == SENTE ? ' ' : 'v')
           << QChar(flavorCodes[mType][mFlavor]);
}
