#include <QPainter>

#include "figure.h"
#include "point.h"

#define BOLD_POINT_SIZE 9

figure::figure(std::vector <relation> thinLines, std::vector <relation> boldLines, std::vector <point> boldPoints) {
    thinLines = thinLines;
    boldLines = boldLines;
    boldPoints = boldPoints;
}

figure::figure() {
    thinLines = {
        {{992, 716}, {1005, 693}},
        {{1005, 693}, {1033, 688}},
        {{1005, 693}, {1002, 426}},
        {{946, 471}, {1000, 454}},
        {{946, 471}, {1000, 424}},
        {{946, 471}, {946, 255}},
        {{946, 255}, {1000, 424}},
        {{946, 255}, {1007, 256}},
        {{946, 255}, {887, 428}},
        {{946, 255}, {972, 207}},
        {{887, 428}, {754, 198}},
        {{754, 198}, {923, 104}},
        {{754, 198}, {783, 480}},
        {{783, 480}, {635, 583}},
        {{783, 480}, {652, 751}},
        {{652, 751}, {635, 583}},
        {{652, 751}, {727, 729}},
        {{635, 583}, {727, 729}},
        {{754, 198}, {738, 86}},
        {{738, 86}, {573, 402}},
        {{573, 402}, {783, 480}},
        {{573, 402}, {604, 521}},
        {{529, 263}, {738, 86}},
        {{529, 263}, {754, 198}},
        {{529, 263}, {573, 402}},
        {{529, 263}, {462, 45}},
        {{529, 263}, {414, 517}},
        {{462, 45}, {414, 517}},
        {{414, 517}, {139, 182}},
        {{414, 517}, {121,  388}},
        {{414, 517}, {208, 588}},
        {{139, 182}, {121,  388}},
        {{333, 712}, {121,  388}},
        {{333, 712}, {139, 182}},
        {{333, 712}, {303, 770}},
        {{333, 712}, {174, 721}},
        {{443, 190}, {121,  388}},
        {{211, 108}, {443, 190}},
        {{616, 58}, {529, 263}},
    };

    boldLines = {
            {{972,  704}, {991,  717}},
            {{991,  717}, {991,  717}},
            {{991,  717}, {1013, 717}},
            {{1013, 717}, {1033, 691}},
            {{1033, 691}, {1029, 551}},
            {{1029, 551}, {1005, 478}},
            {{1005, 478}, {1028, 492}},
            {{1028, 492}, {1050, 480}},
            {{1050, 480}, {1000, 455}},
            {{1000, 455}, {1040, 476}},
            {{1040, 476}, {1056, 456}},
            {{1056, 456}, {1035, 450}},
            {{1035, 450}, {1003, 427}},
            {{1003, 427}, {1008, 256}},
            {{1008, 256}, {973,  211}},
            {{973,  211}, {926,  109}},
            {{926,  109}, {849,  56}},
            {{849,  56}, {737,  83}},
            {{737,  83}, {616,  62}},
            {{616,  62}, {461,  46}},
            {{461,  46}, {323,  61}},
            {{323,  61}, {211,  112}},
            {{211,  112}, {142,  182}},
            {{142,  182}, {138,  217}},
            {{138,  217}, {101,  298}},
            {{101,  298}, {82,   512}},
            {{82,   512}, {58,   540}},
            {{58,   540}, {71,   538}},
            {{71,   538}, {79,   551}},
            {{79,   551}, {87,   538}},
            {{87,   538}, {100,  540}},
            {{100,  540}, {82,   512}},
            {{82,   512}, {117,  407}},
            {{117,  407}, {121,  388}},
            {{121,  388}, {142,  496}},
            {{142,  496}, {205,  591}},
            {{205,  591}, {170,  727}},
            {{170,  727}, {301,  772}},
            {{301,  772}, {349,  763}},
            {{349,  763}, {334,  711}},
            {{334,  711}, {373,  559}},
            {{373,  559}, {415,  523}},
            {{415,  523}, {602,  526}},
            {{602,  526}, {634,  586}},
            {{634,  586}, {585,  721}},
            {{585,  721}, {651,  758}},
            {{651,  758}, {750,  776}},
            {{750,  776}, {724,  733}},
            {{724,  733}, {785,  482}},
            {{785,  482}, {826,  444}},
            {{826,  444}, {888,  431}},
            {{888,  431}, {910,  463}},
            {{910,  463}, {944,  474}},
            {{944,  474}, {965,  521}},
            {{965,  521}, {1005, 689}},
            {{1005, 689}, {976,  691}},
            {{976,  691}, {972,  704}},
    };

    boldPoints = {
            {946, 255},
    };

    int centerX = 0, centerY = 0;
    int numPoints = 0;

    for (int i = 0; i < thinLines.size(); i++) {
        centerX += thinLines[i].a.x;
        centerX += thinLines[i].b.x;
        centerY += thinLines[i].a.y;
        centerY += thinLines[i].b.y;

        numPoints += 2;
    }

    for (int i = 0; i < boldLines.size(); i++) {
        centerX += boldLines[i].a.x;
        centerX += boldLines[i].b.x;
        centerY += boldLines[i].a.y;
        centerY += boldLines[i].b.y;

        numPoints += 2;
    }

    for (int i = 0; i < boldPoints.size(); i++) {
        centerX += boldPoints[i].x;
        centerY += boldPoints[i].y;

        numPoints++;
    }

    centerX /= numPoints;
    centerY /= numPoints;

    for (int i = 0; i < thinLines.size(); i++) {
        thinLines[i].a.x -= centerX;
        thinLines[i].b.x -= centerX;
        thinLines[i].a.y -= centerY;
        thinLines[i].b.y -= centerY;
    }

    for (int i = 0; i < boldLines.size(); i++) {
        boldLines[i].a.x -= centerX;
        boldLines[i].b.x -= centerX;
        boldLines[i].a.y -= centerY;
        boldLines[i].b.y -= centerY;
    }

    for (int i = 0; i < boldPoints.size(); i++) {
        boldPoints[i].x -= centerX;
        boldPoints[i].y -= centerY;
    }
}

void figure::drop() {
    thinLines.clear();
    boldLines.clear();
    boldPoints.clear();
}

void drawPoint(QPainter *painter, QPoint point) {
    int delta = BOLD_POINT_SIZE / 2;
    QRect pointRect = QRect(point.x() - delta, point.y() - delta, BOLD_POINT_SIZE, BOLD_POINT_SIZE);
    painter->drawEllipse(pointRect);
}

void figure::move(int shiftX, int shiftY) {
    for (int i = 0; i < thinLines.size(); i++) {
        thinLines[i].a.move(shiftX, shiftY);
        thinLines[i].b.move(shiftX, shiftY);
    }

    for (int i = 0; i < boldLines.size(); i++) {
        boldLines[i].a.move(shiftX, shiftY);
        boldLines[i].b.move(shiftX, shiftY);
    }

    for (int i = 0; i < boldPoints.size(); i++) {
        boldPoints[i].move(shiftX, shiftY);
    }
}

void figure::revertMove(int shiftX, int shiftY) {
    move(-shiftX, -shiftY);
}

void figure::resize(point base, double ratioX, double ratioY) {
    for (int i = 0; i < thinLines.size(); i++) {
        thinLines[i].a.resize(base, ratioX, ratioY);
        thinLines[i].b.resize(base, ratioX, ratioY);
    }

    for (int i = 0; i < boldLines.size(); i++) {
        boldLines[i].a.resize(base, ratioX, ratioY);
        boldLines[i].b.resize(base, ratioX, ratioY);
    }

    for (int i = 0; i < boldPoints.size(); i++) {
        boldPoints[i].resize(base, ratioX, ratioY);
    }
}

void figure::revertResize(point base, double ratioX, double ratioY) {
    resize(base, 1 / ratioX, 1 / ratioY);
}

void figure::rotate(point base, int angle) {
    for (int i = 0; i < thinLines.size(); i++) {
        thinLines[i].a.rotate(base, angle);
        thinLines[i].b.rotate(base, angle);
    }

    for (int i = 0; i < boldLines.size(); i++) {
        boldLines[i].a.rotate(base, angle);
        boldLines[i].b.rotate(base, angle);
    }

    for (int i = 0; i < boldPoints.size(); i++) {
        boldPoints[i].rotate(base, angle);
    }
}

void figure::revertRotate(point base, int angle) {
    rotate(base, -angle);
}

void figure::draw(QPainter *painter, QColor color, point topLeft, point bottomRight, int canvasWidth, int canvasHeight) {
    painter->setBrush(color);
    painter->setPen(color);

    painter->setPen(QPen(color, 1.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    for (int i = 0; i < thinLines.size(); i++) {
        QPoint qtPointA = thinLines[i].a.toQPoint(topLeft, bottomRight, canvasWidth, canvasHeight);
        QPoint qtPointB = thinLines[i].b.toQPoint(topLeft, bottomRight, canvasWidth, canvasHeight);
        painter->drawLine(qtPointA, qtPointB);
    }

    painter->setPen(QPen(color, 3.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    for (int i = 0; i < boldLines.size(); i++) {
        QPoint qtPointA = boldLines[i].a.toQPoint(topLeft, bottomRight, canvasWidth, canvasHeight);
        QPoint qtPointB = boldLines[i].b.toQPoint(topLeft, bottomRight, canvasWidth, canvasHeight);
        painter->drawLine(qtPointA, qtPointB);
    }

    painter->setPen(color);
    for (int i = 0; i < boldPoints.size(); i++) {
        QPoint qtPoint = boldPoints[i].toQPoint(topLeft, bottomRight, canvasWidth, canvasHeight);
        drawPoint(painter, qtPoint);
    }
}
