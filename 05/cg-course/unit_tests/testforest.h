#ifndef TESTFOREST_H
#define TESTFOREST_H

#include <QObject>

class TestForest : public QObject
{
    Q_OBJECT
public:
    explicit TestForest(QObject *parent = nullptr);

private slots:
    void test_Forest_one();
    void test_Forest_many();

    void test_grow_usual();

    void test_toPolygons_usual();

signals:

};

#endif // TESTFOREST_H
