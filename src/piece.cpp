#include <stdexcept>

#include "piece.h"

static const QChar flavorCodes[Piece::NB_TYPES][Piece::NB_FLAVORS] = {
    // standard
    { u'王',    0 , u'玉'},
    { u'飛', u'龍',    0 },
    { u'角', u'馬',    0 },
    { u'金',    0 ,    0 },
    { u'銀', u'全',    0 },
    { u'桂', u'圭',    0 },
    { u'香', u'杏',    0 },
    { u'歩', u'と',    0 },

    // Tori
    { u'象', u'鵰',    0 },
    { u'鶴',     0,    0 },
    { u'雉',     0,    0 },
    { u'左',     0,    0 },
    { u'右',     0,    0 },
    { u'燕', u'鴈',    0 },

    // Okisaki
    { u'妃',     0,    0 },
    { u'跳', u'今',    0 },
    { u'反', u'仝',    0 },
};

Piece::Piece(Player player, Type type, Flavor flavor) {
    if (player < NB_PLAYERS)
        mPlayer = player;
    else
        throw std::invalid_argument("Unknown player");

    if (type < NB_TYPES)
        mType = type;
    else
        throw std::invalid_argument("Unknown piece type");

    if (flavor < NB_FLAVORS)
        mFlavor = flavor;
    else
        throw std::invalid_argument("Unknown piece flavor");

    if (flavorCodes[type][flavor] == 0)
        throw std::invalid_argument("Piece type and flavor don't match");
}

QChar Piece::code() const
{
    return flavorCodes[mType][mFlavor];
}

Piece *Piece::loadBOD(QTextStream &stream)
{
    QString buffer = stream.read(2);
    if (buffer.size() < 2)
        throw std::runtime_error("Incomplete piece");

    Player player = (buffer[0] == QChar::fromLatin1('v') ? GOTE : SENTE);

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

QChar Piece::defaultCode(Type type)
{
    return flavorCodes[type][DEFAULT];
}

Piece::Type Piece::type(QChar defaultCode)
{
    for (int type = 0; type < NB_TYPES; type++)
        if (defaultCode == flavorCodes[type][DEFAULT])
            return static_cast<Type>(type);

    throw std::runtime_error("Unknown piece");
}
