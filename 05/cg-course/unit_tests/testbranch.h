#ifndef TESTBRANCH_H
#define TESTBRANCH_H

#include <QObject>

class TestBranch : public QObject
{
    Q_OBJECT
public:
    explicit TestBranch(QObject *parent = nullptr);

private slots:
    void test_grow_usual();
    void test_grow_zero();
    void test_grow_many();

    void test_toPolygons_one();

signals:

};

#endif // TESTBRANCH_H
