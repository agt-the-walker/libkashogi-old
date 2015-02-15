#ifndef POSITION_H
#define POSITION_H

#include <QTextStream>

#include "global.h"
#include "piece.h"

class Position
{
public:
    Position();
    ~Position();

    Position(QTextStream &stream);

    void saveBOD(QTextStream &stream) const;
    const Piece *at(unsigned int row, unsigned int column) const;

    static const int BOARD_SIZE = 9;

private:
    Position(const Position&) = delete;
    Position& operator=(const Position&) = delete;

    void reset();
    void destroyBoard();

    void loadCapturedPieces(QTextStream &stream);
    void loadCapturedPiece(QString capturedPiece, Player player);
    void loadBoard(QTextStream &stream);

    void saveCapturedPieces(QTextStream &stream, Player player) const;
    void saveBoard(QTextStream &stream) const;

    Piece *mPieces[BOARD_SIZE][BOARD_SIZE];
    unsigned int mCapturedPieces[NB_PLAYERS][Piece::NB_TYPES];
};

#endif
