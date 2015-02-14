#include "position.h"

static const QStringList playerNames = { "先手", "後手" };

static const QChar japaneseNumerals[] = {
    0, u'一', u'二', u'三', u'四', u'五', u'六', u'七', u'八', u'九', u'十'
};
static const int topJapaneseNumeral =
    sizeof(japaneseNumerals) / sizeof(japaneseNumerals[0]) - 1;

static const QChar ideographicSpace = u'　',
                   verticalSide = '|';

static const QString capturedPieceLabel = "の持駒：";

Position::Position()
{
    reset();
}

Position::~Position()
{
    destroyBoard();
}

Position::Position(QTextStream &stream)
{
    reset();

    loadCapturedPieces(stream);
    try {
        loadBoard(stream);
        loadCapturedPieces(stream);
    } catch (std::exception) {
        destroyBoard();
        throw;
    }
}

void Position::saveBOD(QTextStream &stream) const
{
    saveCapturedPieces(stream, GOTE);
    saveBoard(stream);
    saveCapturedPieces(stream, SENTE);
}

void Position::reset()
{
    for (int row = 0; row < BOARD_SIZE; row++)
        for (int col = 0; col < BOARD_SIZE; col++)
            mPieces[row][col] = nullptr;

    for (int player = 0; player < NB_PLAYERS; player++)
        for (int type = 0; type < Piece::NB_TYPES; type++)
            mCapturedPieces[player][type] = 0;
}

void Position::destroyBoard()
{
    for (int row = 0; row < BOARD_SIZE; row++)
        for (int col = 0; col < BOARD_SIZE; col++)
            delete mPieces[row][col];
}

void Position::loadCapturedPieces(QTextStream &stream)
{
    QStringList fields = stream.readLine().split(capturedPieceLabel);

    if (fields.count() == 2) {
        int player = playerNames.indexOf(fields[0]);
        if (player == -1)
            throw std::runtime_error("Unknown player");

        if (fields[1].isEmpty())
            return;

        QStringList capturedPieces = fields[1].split(ideographicSpace);
        for (auto &capturedPiece: capturedPieces)
            loadCapturedPiece(capturedPiece, static_cast<Player>(player));
    }
}

void Position::loadCapturedPiece(QString capturedPiece, Player player)
{
    if (capturedPiece.isEmpty())
        throw std::runtime_error("Missing piece");

    Piece::Type type = Piece::type(capturedPiece[0]);
    if (type == Piece::KING)
        throw std::runtime_error("Captured piece cannot be the king");

    int count = 0;
    if (capturedPiece.length() == 1)
        count += 1;
    else
        for (int i = 1; i < capturedPiece.length(); i++)
            for (int num = 1; num <= topJapaneseNumeral; num++)
                if (capturedPiece[i] == japaneseNumerals[num])
                    count += num;

    mCapturedPieces[player][type] += count;
}

void Position::loadBoard(QTextStream &stream)
{
    int row = 1;

    while (row <= BOARD_SIZE) {
        QString first = stream.read(1);
        if (first != verticalSide) {
            stream.readLine();
            continue;
        }

        for (int column = BOARD_SIZE; column >= 1; column--)
            mPieces[row-1][column-1] = Piece::loadBOD(stream);

        stream.readLine();
        row += 1;
    }

    stream.readLine();  // bottom side of the board
}

void Position::saveCapturedPieces(QTextStream &stream, Player player) const
{
    stream << playerNames[player] << capturedPieceLabel;

    QStringList words;
    for (int type = 0; type < Piece::NB_TYPES; type++) {
        unsigned int nbPieces = mCapturedPieces[player][type];

        if (nbPieces == 0)
            continue;

        QString word = Piece::defaultCode(static_cast<Piece::Type>(type));

        while (nbPieces >= topJapaneseNumeral) {
            word.append(japaneseNumerals[topJapaneseNumeral]);
            nbPieces -= topJapaneseNumeral;
        }
        if (nbPieces > 1)
            word.append(japaneseNumerals[nbPieces]);

        words << word;
    }

    stream << words.join(ideographicSpace) << endl;
}

void Position::saveBoard(QTextStream &stream) const
{
    static const QChar unicodeNumerals[] = {
        0, u'１', u'２', u'３', u'４', u'５', u'６', u'７', u'８', u'９'
    };
    for (int column = BOARD_SIZE; column >= 1; column--) {
        stream << unicodeNumerals[column];
        if (column > 1)
            stream << ' ';
    }
    stream << endl;

    static const QString horizontalSide = "+---------------------------+";

    stream << horizontalSide << endl;

    for (int row = 1; row <= BOARD_SIZE; row++) {
        stream << verticalSide;

        for (int column = BOARD_SIZE; column >= 1; column--) {
            Piece *piece = mPieces[row-1][column-1];
            if (piece != nullptr)
                piece->saveBOD(stream);
            else
                stream << QString(" ・");
        }

        stream << verticalSide << japaneseNumerals[row] << endl;
    }

    stream << horizontalSide << endl;
}
