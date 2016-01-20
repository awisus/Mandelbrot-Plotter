#ifndef GRAFIKVIEW
#define GRAFIKVIEW

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLabel>
#include <QMouseEvent>
#include <QString>

#include "Grafik.hpp"
#include "Mandelbrot.hpp"

using namespace mandelbrot;

class GrafikView : public QGraphicsView {
    Q_OBJECT

    private: QLabel *lbIter;

    private: QGraphicsScene* scene;
    private: Grafik* grafik;

    private: Mandelbrot *model;


    public: GrafikView(Mandelbrot *model, QWidget *parent = 0): QGraphicsView(parent) {
        this->model = model;

        lbIter = parent->findChild<QLabel*>(QString("labelStatusIter"));

        grafik = new Grafik(model);
        scene = new QGraphicsScene();

        scene->addItem(grafik);

        setScene(scene);

        setMouseTracking(true);
    }

    public: ~GrafikView() {
        delete grafik;
        delete scene;

        delete lbIter;
    }


    public: void mousePressEvent(QMouseEvent *e) {
        if(e->button() == Qt::LeftButton) {
            model->zoom(e->x(), e->y(), true);
        }
    }

    public: void mouseMoveEvent(QMouseEvent *e) {
        QString stringMausUeber = QString("Maus über ");
        QString stringIter = QString("Iterationen: ");

        QString zahl = model->punktZuComplex(e->x(), e->y()).toQString(12);
        QString iter = QString::number(model->getIterationZuPunkt(e->x(), e->y()));

        lbIter->setText(stringIter + iter);

        emit mausUeber(stringMausUeber + zahl);
    }

    signals: void mausUeber(QString ueber);


    public slots: void updateGrafik() {
        grafik->update();
    }


    public: void setGroesse(int breiteEingabe, int breite, int hoehe) {
        scene->setSceneRect(0, 0, breite - 2, hoehe - 2);
        setGeometry(breiteEingabe, 0, breite, hoehe);
    }

    public slots: void setFarbe(int index) {
        grafik->setFarbe(index);
    }
};

#endif // GRAFIKVIEW

