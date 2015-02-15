#include <QtTest/QtTest>

class PositionTest: public QObject
{
    Q_OBJECT

private slots:
    void at();
    void at_data();

    void identity();
    void identity_data();

    void invalidBOD();
    void invalidBOD_data();
};
