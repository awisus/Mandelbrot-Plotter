#include <QApplication>
#include <iostream>

#include "Fenster.hpp"
#include "Mandelbrot.hpp"
#include "Complex.hpp"

using namespace mandelbrot;
using namespace std;

int main(int argc, char *argv[]) {

    Complex ca(-2, -1);
    Complex ce(0.5, 1);
    Mandelbrot *mandelbrot = new Mandelbrot(&ca, &ce, 80, 80, 32);
    mandelbrot->berechnen();

    cout << mandelbrot->toString() << endl;

    /* ---------------------------------------- */

    mandelbrot->setDimension(750, 600);
    mandelbrot->berechnen();

    /* ---------------------------------------- */

    QApplication a(argc, argv);
    Fenster w(mandelbrot);
    w.setWindowTitle("Mandelbrotmenge");
    w.show();

    /* ---------------------------------------- */

    return a.exec();
}
