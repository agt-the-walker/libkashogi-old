#include "position.h"

static const QStringList playerNames { QStringLiteral("先手"),
                                       QStringLiteral("後手") };

static const QChar japaneseNumerals[] = {
    0, u'一', u'二', u'三', u'四', u'五', u'六', u'七', u'八', u'九', u'十'
};
static const int topJapaneseNumeral =
    sizeof(japaneseNumerals) / sizeof(japaneseNumerals[0]) - 1;

static const QChar verticalSide = u'|';

static const QString capturedPieceLabel = QStringLiteral("の持駒：");

Position::~Position()
{
    destroyBoard(mBoard);
}

void Position::loadBOD(QTextStream &stream)
{
    stream.setCodec("UTF-8");

    // we use temporary storage so that this object is left untouched if
    //  this method raises an exception
    board_t board {};
    captured_t capturedPieces[NB_PLAYERS] {};

    loadCapturedPieces(stream, capturedPieces);
    try {
        loadBoard(stream, board);
        loadCapturedPieces(stream, capturedPieces);
    } catch (std::exception const&) {
        destroyBoard(board);
        throw;
    }

    memcpy(mBoard, board, sizeof(board));
    for (int player = 0; player < NB_PLAYERS; player++)
        mCapturedPieces[player] = std::move(capturedPieces[player]);
}

void Position::saveBOD(QTextStream &stream) const
{
    stream.setCodec("UTF-8");

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

unsigned int Position::nbCaptured(Player player, QChar type) const
{
    return mCapturedPieces[player][type];
}

void Position::destroyBoard(board_t board)
{
    for (int row = 0; row < BOARD_SIZE; row++)
        for (int column = 0; column < BOARD_SIZE; column++)
            delete board[row][column];
}

void Position::loadCapturedPieces(QTextStream &stream,
                                  captured_t capturedPieces[NB_PLAYERS])
{
    QStringList fields = stream.readLine().split(capturedPieceLabel);

    if (fields.count() == 2) {
        int player = playerNames.indexOf(fields[0]);
        if (player == -1)
            throw std::runtime_error("Unknown player");

        if (fields[1].isEmpty() || fields[1] == QStringLiteral("なし"))
            return;

        for (auto &capturedPiece: fields[1].split(
                QRegularExpression(QStringLiteral("\\s+"),
                        QRegularExpression::UseUnicodePropertiesOption),
                Qt::SkipEmptyParts))
            loadCapturedPiece(capturedPiece, capturedPieces[player]);
    }
}

void Position::loadCapturedPiece(QString capturedPiece,
                                 captured_t &capturedPieces)
{
    QChar type = capturedPiece[0];
    if (Piece::isKing(type))
        throw std::runtime_error("Captured piece cannot be the king");
    Piece::typeIndex(type);  // XXX: raises an exception if unknown piece (ugly)

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

void Position::loadBoard(QTextStream &stream, board_t board)
{
    int row = 0;

    while (row < BOARD_SIZE && !stream.atEnd()) {
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
    QMap<unsigned int, QChar> typeFromIndex;  // of captured pieces

    for (auto it = mCapturedPieces[player].constBegin(); 
              it != mCapturedPieces[player].constEnd(); ++it) {
        QChar type = it.key();
        typeFromIndex[Piece::typeIndex(type)] = type;
    }

    for (auto it = typeFromIndex.constBegin(); 
              it != typeFromIndex.constEnd(); ++it) {
        QChar type = it.value();
        unsigned int nbPieces = mCapturedPieces[player][type];

        if (nbPieces == 0)
            continue;

        QString word = type;

        while (nbPieces >= topJapaneseNumeral) {
            word.append(japaneseNumerals[topJapaneseNumeral]);
            nbPieces -= topJapaneseNumeral;
        }
        if (nbPieces > 1)
            word.append(japaneseNumerals[nbPieces]);

        words << word;
    }

    static const QChar ideographicSpace = u'　';
    stream << words.join(ideographicSpace) << Qt::endl;
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
    stream << Qt::endl;

    static const QString horizontalSide =
            QStringLiteral("+---------------------------+");

    stream << horizontalSide << Qt::endl;

    for (int row = 0; row < BOARD_SIZE; row++) {
        stream << verticalSide;

        for (int column = BOARD_SIZE-1; column >= 0; column--) {
            Piece *piece = mBoard[row][column];
            if (piece)
                piece->saveBOD(stream);
            else
                stream << QStringLiteral(" ・");
        }

        stream << verticalSide << japaneseNumerals[row+1] << Qt::endl;
    }

    stream << horizontalSide << Qt::endl;
}
