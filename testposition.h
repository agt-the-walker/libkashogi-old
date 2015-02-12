#include <QtTest/QtTest>

class TestPosition: public QObject
{
    Q_OBJECT

private slots:
    void empty();

    void identity();
    void identity_data();
};
