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

Position::~Position()
{
    destroyBoard(mBoard);
}

void Position::loadBOD(QTextStream &stream)
{
    // we use temporary storage so that this object is left untouched if
    //  this method raises an exception
    Piece *board[BOARD_SIZE][BOARD_SIZE] {};
    unsigned int capturedPieces[NB_PLAYERS][Piece::NB_TYPES] {};

    loadCapturedPieces(stream, capturedPieces);
    try {
        loadBoard(stream, board);
        loadCapturedPieces(stream, capturedPieces);
    } catch (std::exception) {
        destroyBoard(board);
        throw;
    }

    memcpy(mBoard, board, sizeof(board));
    memcpy(mCapturedPieces, capturedPieces, sizeof(capturedPieces));
}

void Position::saveBOD(QTextStream &stream) const
{
    saveCapturedPieces(stream, GOTE);
    saveBoard(stream);
    saveCapturedPieces(stream, SENTE);
}

const Piece *Position::at(unsigned int row, unsigned int column) const
{
    if (row == 0 || row > BOARD_SIZE || column == 0 || column > BOARD_SIZE)
        throw std::out_of_range("invalid square");

    return mBoard[row-1][column-1];
}

void Position::destroyBoard(Piece *board[BOARD_SIZE][BOARD_SIZE])
{
    for (int row = 0; row < BOARD_SIZE; row++)
        for (int column = 0; column < BOARD_SIZE; column++)
            delete board[row][column];
}

void Position::loadCapturedPieces(QTextStream &stream,
        unsigned int capturedPieces[NB_PLAYERS][Piece::NB_TYPES])
{
    QStringList fields = stream.readLine().split(capturedPieceLabel);

    if (fields.count() == 2) {
        int player = playerNames.indexOf(fields[0]);
        if (player == -1)
            throw std::runtime_error("Unknown player");

        if (fields[1].isEmpty())
            return;

        for (auto &capturedPiece: fields[1].split(ideographicSpace))
            loadCapturedPiece(capturedPiece, capturedPieces[player]);
    }
}

void Position::loadCapturedPiece(QString capturedPiece,
                                 unsigned int capturedPieces[Piece::NB_TYPES])
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

    capturedPieces[type] += count;
}

void Position::loadBoard(QTextStream &stream,
                         Piece *board[BOARD_SIZE][BOARD_SIZE])
{
    int row = 0;

    while (row < BOARD_SIZE) {
        QString first = stream.read(1);
        if (first != verticalSide) {
            stream.readLine();
            continue;
        }

        for (int column = BOARD_SIZE-1; column >= 0; column--)
            board[row][column] = Piece::loadBOD(stream);

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
    static const QChar unicodeNumerals[BOARD_SIZE] = {
        u'１', u'２', u'３', u'４', u'５', u'６', u'７', u'８', u'９'
    };
    for (int column = BOARD_SIZE-1; column >= 0; column--) {
        stream << unicodeNumerals[column];
        if (column > 0)
            stream << ' ';
    }
    stream << endl;

    static const QString horizontalSide = "+---------------------------+";

    stream << horizontalSide << endl;

    for (int row = 0; row < BOARD_SIZE; row++) {
        stream << verticalSide;

        for (int column = BOARD_SIZE-1; column >= 0; column--) {
            Piece *piece = mBoard[row][column];
            if (piece != nullptr)
                piece->saveBOD(stream);
            else
                stream << QString(" ・");
        }

        stream << verticalSide << japaneseNumerals[row+1] << endl;
    }

    stream << horizontalSide << endl;
}
