#include <QtTest/QtTest>

class PieceTest: public QObject
{
    Q_OBJECT

private slots:
    void invalid();
    void invalid_data();

    void invalidBOD();

    void unknownBOD();
    void unknownBOD_data();

    void identity();
    void identity_data();

    void accessors();
    void accessors_data();

    void invalidType();
};
