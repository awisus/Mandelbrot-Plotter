#ifndef GRAFIK_HPP
#define GRAFIK_HPP

#include <QGraphicsItem>
#include <QPainter>
#include <QPoint>
#include <QColor>

#include "Mandelbrot.hpp"

using namespace mandelbrot;

#define ANZAHL_FARBEN 6

class Grafik : public QGraphicsItem {

    public: static int const STANDARD = 0;
    public: static int const ROT = 1;
    public: static int const GRUEN = 2;
    public: static int const GELB = 3;
    public: static int const BLAU = 4;
    public: static int const ORANGE = 5;

    private: int farbenMasken[ANZAHL_FARBEN][3] = {
         {30, 0, 50}
        ,{60, 0, 0}
        ,{0, 60, 10}
        ,{45, 50, 0}
        ,{0, 0, 50}
        ,{75, 30, 0}
    };

    private: int *farbenMaske;

    private: QRect bRect;
    private: Mandelbrot *model;


    public: Grafik(Mandelbrot *model) {
        this->model = model;
        farbenMaske = farbenMasken[0];
        setBoundingRect(0, 0, model->getBreite() - 2, model->getHoehe() - 2);
    }


    public: void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) {
        (void)(item);
        (void)(widget);

        QPoint point(0, 0);
        QColor farbe(0, 0, 0);
        int iter = 0;

        for(int i = 0; i < model->getBreite(); i++) {
            for(int j = 0; j < model->getHoehe(); j++) {
                iter = model->getIterationZuPunkt(i, j);

                point.setX(i);
                point.setY(j);

                setColourForIteration(farbe, iter);
                painter->setPen(farbe);
                painter->drawPoint(point);
            }
        }
    }

    private: void setColourForIteration(QColor &farbe, int iter) {
        if(model->getIterationen() == iter) {
            farbe.setRed(0);
            farbe.setGreen(0);
            farbe.setBlue(0);
            return;
        }

        double anteil = (double)iter / (double)model->getIterationen();

        int r = getFarbAnteilMitDif(anteil, farbenMaske[0]);
        int g = getFarbAnteilMitDif(anteil, farbenMaske[1]);
        int b = getFarbAnteilMitDif(anteil, farbenMaske[2]);

        farbe.setRed(r);
        farbe.setGreen(g);
        farbe.setBlue(b);
    }

    private: int getFarbAnteilMitDif(double anteil, int diff) {
        return (int)(anteil * (255 - diff)) + diff;
    }

    public: QRectF boundingRect() const {
        return bRect;
    }

    public: void setBoundingRect(int x1, int y1, int x2, int y2) {
        bRect = QRect(x1, y1, x2, y2);
    }

    public: void setFarbe(int index) {
        if(index >= 0 && index < ANZAHL_FARBEN) {
            farbenMaske = farbenMasken[index];
            update();
        }
    }
};

#endif // GRAFIK_HPP
