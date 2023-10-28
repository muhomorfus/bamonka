#include <QApplication>
#include <QtTest>

#include <string>
#include <unistd.h>
#include <iostream>

#include "inc/mainwindow.h"
#include "inc/scene.h"
#include "unit_tests/testpolygon.h"
#include "unit_tests/testbranch.h"
#include "unit_tests/testforest.h"
#include "inc/forest.h"

int main(int argc, char *argv[])
{
    if (argc == 1) {
        QApplication a(argc, argv);
        MainWindow w;
        w.show();
        return a.exec();
    }

    char *outputFile = nullptr;
    int iterations = 0;
    int threads = 0;

    bool output = false;

    int arg = 0;
    while((arg = getopt(argc, argv, "i:o:ubt:")) != -1) {
        switch(arg) {
        case 'o':
            outputFile = optarg;
            break;
        case 'i':
            iterations = std::stoi(optarg);
            break;
        case 't':
            threads = std::stoi(optarg);
            break;
        case 'u':
            return QTest::qExec(new TestPolygon) +
                   QTest::qExec(new TestBranch) +
                   QTest::qExec(new TestForest);
        case 'b':
            output = true;
            break;
        case '?':
            return EXIT_FAILURE;
        }
    }

    auto scene = Scene();
    Forest f = Forest(
        10,
        0,
        QVector2D(-100, 0),
        QVector2D(1300, 700),
        500,
        QColor(159,140,103),
        Qt::darkRed,
        QColor(149,190,108),
        0.4,
        0.5,
        60
    );
    scene.setForest(f);
    scene.setBackground(QColor(229,234,206));
    scene.growForest(100, iterations);

    using std::chrono::duration_cast;
    using std::chrono::microseconds;

    auto end = std::chrono::steady_clock::now();
    auto start = std::chrono::steady_clock::now();
    start = std::chrono::steady_clock::now();

    auto image = scene.paint(threads);

    end = std::chrono::steady_clock::now();

    if (output) {
        std::cout << (int)duration_cast<microseconds>(end - start).count() / 1000 << "\n";

        if (!outputFile) {
            return EXIT_SUCCESS;
        }
    }

    if (!outputFile) {
        return EXIT_FAILURE;
    }

    image->save(outputFile);

    return EXIT_SUCCESS;
}
