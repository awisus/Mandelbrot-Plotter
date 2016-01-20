#ifndef FENSTER_HPP
#define FENSTER_HPP

#include <cstdlib>
#include <iostream>
#include <string>

#include <Grafik.hpp>
#include <QDir>
#include <QFileDialog>
#include <QGraphicsView>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QSignalMapper>
#include <QString>

#include "ui_fenster.h"
#include "Complex.hpp"
#include "GrafikView.hpp"
#include "Mandelbrot.hpp"

using namespace mandelbrot;

namespace Ui {
    class Fenster;
}

class Fenster : public QMainWindow {
    Q_OBJECT

    private: Ui::Fenster* ui;
    private: GrafikView* view;

    private: QGroupBox *gbEingabe;
    private: QLineEdit *leCaRe;
    private: QLineEdit *leCaIm;
    private: QLineEdit *leCeRe;
    private: QLineEdit *leCeIm;
    private: QLineEdit *leDimBreite;
    private: QLineEdit *leDimHoehe;
    private: QLineEdit *leIter;
    private: QLabel *lbStatus;

    private: Mandelbrot* model;

    /* ---------------------------------------- */

    public: explicit Fenster(Mandelbrot *model, QWidget *parent = 0) : QMainWindow(parent) {
        this->model = model;

        initUI();
        verbinden();
        setTexts();

        setGroesse(model->getBreite(), model->getHoehe());

        QIcon icon(":/images/icon.png");
        setWindowIcon(icon);
    }

    public: ~Fenster() {
        delete leCaRe;
        delete leCaIm;
        delete leCeRe;
        delete leCeIm;
        delete leDimBreite;
        delete leDimHoehe;
        delete leIter;
        delete gbEingabe;

        delete lbStatus;

        delete view;
        delete ui;
    }

    /* ---------------------------------------- */

    private: void initUI() {
        ui = new Ui::Fenster;
        ui->setupUi(this);

        view = new GrafikView(model, this);

        gbEingabe = findChild<QGroupBox*>(QString("groupBoxEingabe"));

        leCaRe = findChild<QLineEdit*>(QString("lineEditCaRe"));
        leCaIm = findChild<QLineEdit*>(QString("lineEditCaIm"));
        leCeRe = findChild<QLineEdit*>(QString("lineEditCeRe"));
        leCeIm = findChild<QLineEdit*>(QString("lineEditCeIm"));
        leDimBreite = findChild<QLineEdit*>(QString("lineEditDimensionBreite"));
        leDimHoehe = findChild<QLineEdit*>(QString("lineEditDimensionHoehe"));
        leIter = findChild<QLineEdit*>(QString("lineEditIter"));

        lbStatus = findChild<QLabel*>(QString("labelStatus"));
    }

    /* ---------------------------------------- */

    private: void verbinden() {
        mapAktionIter();
        mapAktionFarben();
        mapAktionVerschieben();
        mapAktionZoom();

        /* ---------------------------------------- */
        connect(ui->actionExport_als_png, SIGNAL(triggered()), this, SLOT(exportPNG()));

        connect(ui->actionWerte_speichern, SIGNAL(triggered()), this, SLOT(dialogSpeichern()));
        connect(ui->actionWerte_laden, SIGNAL(triggered()), this, SLOT(dialogLaden()));

        connect(ui->actionSchliessen, SIGNAL(triggered()), this, SLOT(beenden()));
        connect(ui->pushButtonRuecksetzen, SIGNAL(clicked()), this, SLOT(setTexts()));
        connect(ui->pushButtonBerechnen, SIGNAL(clicked()), this, SLOT(setWerte()));

        connect(ui->pushButtonZentrieren, SIGNAL(clicked()), model, SLOT(ruecksetzen()));

        /* ---------------------------------------- */
        connect(view, SIGNAL(mausUeber(QString)), this, SLOT(setTitel(QString)));

        /* ---------------------------------------- */
        connect(model, SIGNAL(dimensionChanged(int, int)), this, SLOT(setGroesse(int, int)));
        connect(model, SIGNAL(berechnet()), this, SLOT(nachBerechnung()));
    }

    private: void mapAktionIter() {
        QSignalMapper *signalMapper = new QSignalMapper(this);

        connect(ui->action20, SIGNAL(triggered()), signalMapper, SLOT(map()));
        connect(ui->action50, SIGNAL(triggered()), signalMapper, SLOT(map()));
        connect(ui->action80, SIGNAL(triggered()), signalMapper, SLOT(map()));
        connect(ui->action100, SIGNAL(triggered()), signalMapper, SLOT(map()));
        connect(ui->action120, SIGNAL(triggered()), signalMapper, SLOT(map()));
        connect(ui->action150, SIGNAL(triggered()), signalMapper, SLOT(map()));
        connect(ui->action200, SIGNAL(triggered()), signalMapper, SLOT(map()));
        connect(ui->action400, SIGNAL(triggered()), signalMapper, SLOT(map()));
        connect(ui->action600, SIGNAL(triggered()), signalMapper, SLOT(map()));
        connect(ui->action800, SIGNAL(triggered()), signalMapper, SLOT(map()));
        connect(ui->action1000, SIGNAL(triggered()), signalMapper, SLOT(map()));
        connect(ui->action1500, SIGNAL(triggered()), signalMapper, SLOT(map()));
        connect(ui->action2000, SIGNAL(triggered()), signalMapper, SLOT(map()));

        signalMapper->setMapping(ui->action20, 20);
        signalMapper->setMapping(ui->action50, 50);
        signalMapper->setMapping(ui->action80, 80);
        signalMapper->setMapping(ui->action100, 100);
        signalMapper->setMapping(ui->action120, 120);
        signalMapper->setMapping(ui->action150, 150);
        signalMapper->setMapping(ui->action200, 200);
        signalMapper->setMapping(ui->action400, 400);
        signalMapper->setMapping(ui->action600, 600);
        signalMapper->setMapping(ui->action800, 800);
        signalMapper->setMapping(ui->action1000, 1000);
        signalMapper->setMapping(ui->action1500, 1500);
        signalMapper->setMapping(ui->action2000, 2000);

        connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(berechneMitIter(int)));
    }

    private slots: void berechneMitIter(int iter) {
        model->setIterationen(iter);
        model->berechnen();
    }

    private: void mapAktionFarben() {
        QSignalMapper *signalMapper = new QSignalMapper(this);

        connect(ui->actionStandard, SIGNAL(triggered()), signalMapper, SLOT(map()));
        connect(ui->actionRot, SIGNAL(triggered()), signalMapper, SLOT(map()));
        connect(ui->actionGr_n, SIGNAL(triggered()), signalMapper, SLOT(map()));
        connect(ui->actionGelb, SIGNAL(triggered()), signalMapper, SLOT(map()));
        connect(ui->actionBlau, SIGNAL(triggered()), signalMapper, SLOT(map()));
        connect(ui->actionOrange, SIGNAL(triggered()), signalMapper, SLOT(map()));

        signalMapper->setMapping(ui->actionStandard, Grafik::STANDARD);
        signalMapper->setMapping(ui->actionRot, Grafik::ROT);
        signalMapper->setMapping(ui->actionGr_n, Grafik::GRUEN);
        signalMapper->setMapping(ui->actionGelb, Grafik::GELB);
        signalMapper->setMapping(ui->actionBlau, Grafik::BLAU);
        signalMapper->setMapping(ui->actionOrange, Grafik::ORANGE);

        connect(signalMapper, SIGNAL(mapped(int)), view, SLOT(setFarbe(int)));
    }

    private: void mapAktionVerschieben() {
        QSignalMapper *signalMapper = new QSignalMapper(this);

        connect(ui->pushButtonHoch, SIGNAL(clicked()), signalMapper, SLOT(map()));
        connect(ui->pushButtonRunter, SIGNAL(clicked()), signalMapper, SLOT(map()));
        connect(ui->pushButtonLinks, SIGNAL(clicked()), signalMapper, SLOT(map()));
        connect(ui->pushButtonRechts, SIGNAL(clicked()), signalMapper, SLOT(map()));

        signalMapper->setMapping(ui->pushButtonHoch, Mandelbrot::HOCH);
        signalMapper->setMapping(ui->pushButtonRunter, Mandelbrot::RUNTER);
        signalMapper->setMapping(ui->pushButtonLinks, Mandelbrot::LINKS);
        signalMapper->setMapping(ui->pushButtonRechts, Mandelbrot::RECHTS);

        connect(signalMapper, SIGNAL(mapped(int)), model, SLOT(verschieben(int)));
    }

    private: void mapAktionZoom() {
        QSignalMapper *signalMapper = new QSignalMapper(this);

        connect(ui->pushButtonZoomEin, SIGNAL(clicked()), signalMapper, SLOT(map()));
        connect(ui->pushButtonZoomAus, SIGNAL(clicked()), signalMapper, SLOT(map()));

        signalMapper->setMapping(ui->pushButtonZoomEin, 0);
        signalMapper->setMapping(ui->pushButtonZoomAus, 1);

        connect(signalMapper, SIGNAL(mapped(int)), model, SLOT(zoom(int)));
    }

    /* ---------------------------------------- */

    private slots: void exportPNG() {
        QString filter("PNG (*.png);; All Files (*.*)");
        QString standardFilter("PNG (*.png)");

        QString datei = QFileDialog::getSaveFileName(this, "Export als PNG", QDir::currentPath(), filter, &standardFilter);

        if(!datei.isNull()) {
            if(!datei.endsWith(".png")) {
                datei += ".png";
            }

            QPixmap pixMap;
            pixMap = view->grab();
            pixMap.save(datei);
        }
    }

    /* ---------------------------------------- */

    private slots: void dialogSpeichern() {
        QString filter("Comma seperated values (*.csv);; All Files (*.*)");
        QString standardFilter("Comma seperated values (*.csv)");

        QString datei = QFileDialog::getSaveFileName(this, "Speichern", QDir::currentPath(), filter, &standardFilter);

        if(!datei.isNull()) {
            if(!datei.endsWith(".csv")) {
                datei += ".csv";
            }

            QFile f(datei);
            f.open(QIODevice::WriteOnly);
            f.write(model->getSpeicherString().c_str(), model->getSpeicherString().length());
            f.close();
        }
    }

    private slots: void dialogLaden() {
        QString filter("Comma seperated values (*.csv);; All Files (*.*)");
        QString standardFilter("All Files (*.*)");

        QString datei = QFileDialog::getOpenFileName(this, "Laden", QDir::currentPath(), filter, &standardFilter);

        if(!datei.isNull()) {
            char temp[512];
            int laenge;

            QFile f(datei);
            f.open(QIODevice::ReadOnly);
            laenge = f.read(temp, 512);
            f.close();

            temp[laenge] = '\0';

            model->setFromString(string(temp));
        }
    }

    /* ---------------------------------------- */

    public slots: void nachBerechnung() {
        setTexts();
        view->updateGrafik();
    }

    private slots: void beenden() {
        delete model;
        qApp->quit();
    }

    /* ---------------------------------------- */

    private slots: void setGroesse(int breite, int hoehe) {
        int breiteEingabe = gbEingabe->width();
        view->setGroesse(breiteEingabe, breite, hoehe);
        setFixedSize(breite + breiteEingabe, hoehe);
    }

    private slots: void setTexts() {
        leCaRe->setText(QString{"%1"}.arg(model->getCa()->getReal(), 9, 'f', 9, '0'));
        leCaIm->setText(QString{"%1"}.arg(model->getCa()->getImag(), 9, 'f', 9, '0'));
        leCeRe->setText(QString{"%1"}.arg(model->getCe()->getReal(), 9, 'f', 9, '0'));
        leCeIm->setText(QString{"%1"}.arg(model->getCe()->getImag(), 9, 'f', 9, '0'));
        leDimBreite->setText(QString::number(model->getBreite()));
        leDimHoehe->setText(QString::number(model->getHoehe()));
        leIter->setText(QString::number(model->getIterationen()));
    }

    public slots: void setWerte() {
        double caRe = leCaRe->text().toDouble();
        double caIm = leCaIm->text().toDouble();
        double ceRe = leCeRe->text().toDouble();
        double ceIm = leCeIm->text().toDouble();
        int breite = leDimBreite->text().toInt();
        int hoehe = leDimHoehe->text().toInt();
        int iter = leIter->text().toInt();

        model->setGrenzen(Complex(caRe, caIm), Complex(ceRe, ceIm));
        model->setDimension(breite, hoehe);
        model->setIterationen(iter);

        lbStatus->setText("Berechne...");
        model->berechnen();
        lbStatus->setText("Bereit");
    }

    public slots: void setTitel(QString titel) {
        setWindowTitle("Mandelbrotmenge | " +titel);
    }
};

#endif // FENSTER_HPP
