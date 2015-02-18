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
    unsigned int nbCaptured(Player player, Piece::Type type) const;

    static const int BOARD_SIZE = 9;

private:
    Position(const Position&) = delete;
    Position& operator=(const Position&) = delete;

    typedef Piece * board_t[BOARD_SIZE][BOARD_SIZE];
    typedef unsigned int captured_t[Piece::NB_TYPES];

    static void destroyBoard(board_t board);

    static void loadCapturedPieces(QTextStream &stream,
                                   captured_t capturedPieces[NB_PLAYERS]);
    static void loadCapturedPiece(QString capturedPiece,
                                  captured_t capturedPieces);
    static void loadBoard(QTextStream &stream, board_t board);

    void saveCapturedPieces(QTextStream &stream, Player player) const;
    void saveBoard(QTextStream &stream) const;

    board_t mBoard {};
    captured_t mCapturedPieces[NB_PLAYERS] {};
};

#endif
