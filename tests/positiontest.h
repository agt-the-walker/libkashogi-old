#include <QtTest/QtTest>

class PositionTest: public QObject
{
    Q_OBJECT

private slots:
    void identity();
    void identity_data();

    void invalidBOD();
    void invalidBOD_data();
};
