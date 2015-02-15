#ifndef POSITION_H
#define POSITION_H

#include <QTextStream>

#include "global.h"
#include "piece.h"

class Position
{
public:
    Position() {};
    ~Position();

    void loadBOD(QTextStream &stream);
    void saveBOD(QTextStream &stream) const;
    const Piece *at(unsigned int row, unsigned int column) const;

    static const int BOARD_SIZE = 9;

private:
    Position(const Position&) = delete;
    Position& operator=(const Position&) = delete;

    static void destroyBoard(Piece *board[BOARD_SIZE][BOARD_SIZE]);

    static void loadCapturedPieces(QTextStream &stream,
            unsigned int capturedPieces[NB_PLAYERS][Piece::NB_TYPES]);
    static void loadCapturedPiece(QString capturedPiece,
            unsigned int capturedPieces[Piece::NB_TYPES]);
    static void loadBoard(QTextStream &stream,
            Piece *board[BOARD_SIZE][BOARD_SIZE]);

    void saveCapturedPieces(QTextStream &stream, Player player) const;
    void saveBoard(QTextStream &stream) const;

    Piece *mBoard[BOARD_SIZE][BOARD_SIZE] {};
    unsigned int mCapturedPieces[NB_PLAYERS][Piece::NB_TYPES] {};
};

#endif
