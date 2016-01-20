
#ifndef MANDELBROT_HPP_
#define MANDELBROT_HPP_

#include <string>
#include <sstream>
#include <vector>
#include <QObject>

#include "Complex.hpp"
#include "tools.hpp"

using namespace std;

namespace mandelbrot {

    class Mandelbrot: public QObject {
        Q_OBJECT

        public: static int const HOCH = 0;
        public: static int const RUNTER = 1;
        public: static int const LINKS = 2;
        public: static int const RECHTS = 3;


        private: int breite;
        private: int hoehe;

        private: Complex *caAusgang;
        private: Complex *ceAusgang;

        private: Complex *ca;
        private: Complex *ce;

        private: int iterationen;

        private: int* punkte;


        public: Mandelbrot(Complex *ca, Complex *ce, int iterationen, int breite, int hoehe) {
            this->ca = ca;
            this->ce = ce;
            this->breite = breite;
            this->hoehe = hoehe;
            this->iterationen = iterationen;

            caAusgang = new Complex(ca);
            ceAusgang = new Complex(ce);
        }

        public: ~Mandelbrot() {
            if(punkte) delete[] punkte;
        }


        public slots: void ruecksetzen() {
            ca->set(caAusgang);
            ce->set(ceAusgang);
            berechnen();
        }


        public slots: void verschieben(int richtung) {
            if(richtung == HOCH || richtung == RUNTER) {
                double deltaY = (ce->getImag() - ca->getImag()) * .1;

                deltaY *= richtung == HOCH ? 1 : -1;

                ca->setImag(ca->getImag() + deltaY);
                ce->setImag(ce->getImag() + deltaY);
            } else {
                double deltaX = (ce->getReal() - ca->getReal()) * .1;

                deltaX *= richtung == RECHTS ? 1 : -1;

                ca->setReal(ca->getReal() + deltaX);
                ce->setReal(ce->getReal() + deltaX);
            }

            berechnen();
        }

        public slots: void zoom(int ein) {
            zoom(-1, -1, ein == 0 ? true : false);
        }

        public: void zoom(int x, int y, bool ein) {
            if(x == -1) x = breite / 2;
            if(y == -1) y = hoehe  / 2;

            Complex c = punktZuComplex(x, y);

            double deltaX = ce->getReal() - ca->getReal();
            double deltaY = ce->getImag() - ca->getImag();

            deltaX = ein ? deltaX * .6 : deltaX / .6;
            deltaY = ein ? deltaY * .6 : deltaY / .6;
            deltaX *= .5;
            deltaY *= .5;

            ca->setReal(c.getReal() - deltaX);
            ca->setImag(c.getImag() - deltaY);
            ce->setReal(c.getReal() + deltaX);
            ce->setImag(c.getImag() + deltaY);

            berechnen();
        }

        public: void berechnen() {
            punkte = new int[breite * hoehe];

			for(int r = 0; r < breite; r++) {
				for(int s = 0; s < hoehe; s++) {
                    punkte[r + s * breite] = getIterationFuerPunkt(punktZuComplex(r, s));
				}
            }

            emit berechnet();
        }

        signals: void berechnet();

        private: int getIterationFuerPunkt(Complex z) {
            Complex c = z;

			for(int i = 0; i < iterationen; i++) {
                if(z.betrag() > 4) {
					return i;
				}

                double x = (z.getReal() * z.getReal() - z.getImag() * z.getImag()) + c.getReal();
                double y = (2 * z.getReal() * z.getImag() + c.getImag());

                z = Complex(x, y);
            }

			return iterationen;
        }

        public: Complex punktZuComplex(int r, int s) {
            double a = ((ce->getReal() - ca->getReal()) / breite) * r + ca->getReal();
            double b = ((ce->getImag() - ca->getImag()) / hoehe) * (hoehe - s) + ca->getImag();

            return Complex(a, b);
        }


        public: void setFromString(string str) {
            vector<string> werte = stringwerkzeuge::zerteile(str, ';');

            double caRe = stod(werte.at(0));
            double caIm = stod(werte.at(1));
            double ceRe = stod(werte.at(2));
            double ceIm = stod(werte.at(3));
            int iter = stoi(werte.at(4));

            setGrenzen(Complex(caRe, caIm), Complex(ceRe, ceIm));
            setIterationen(iter);

            berechnen();
        }

		public: void setDimension(int breite, int hoehe) {
			if(breite > 0 && hoehe > 0) {
				this->breite = breite;
				this->hoehe = hoehe;
                emit dimensionChanged(breite, hoehe);
			}
        }

        signals: void dimensionChanged(int breite, int hoehe);

        public: void setGrenzen(Complex ca, Complex ce) {
            if((ca.getReal() < ce.getReal()) && (ca.getImag() < ce.getImag())) {
                this->ca->set(ca);
                this->ce->set(ce);
            }
        }

		public: void setIterationen(int iterationen) {
            if(iterationen >= 0) {
				this->iterationen = iterationen;
			}
        }


        public: Complex *getCa() {
            return ca;
        }

        public: Complex *getCe() {
            return ce;
        }

		public: int getIterationen() {
			return iterationen;
        }

		public: int getBreite() {
			return breite;
        }

		public: int getHoehe() {
			return hoehe;
        }

		public: int getIterationZuPunkt(int i, int j) {
            return punkte[i + j * breite];
        }

		public: string toString() {
			string str = "";

			for(int s = 0; s < hoehe; s++) {
				for(int r = 0; r < breite; r++) {
					int iteration = getIterationZuPunkt(r, s);
					char zeichen  = iterationen == iteration ? ' ' : (char)(iteration % 26 + 65);

					str += zeichen;
				}
				str += "\n";
			}

			return str;
        }

        public: string getSpeicherString() {
            string temp = "";

            temp += to_string(ca->getReal()) + ";";
            temp += to_string(ca->getImag()) + ";";
            temp += to_string(ce->getReal()) + ";";
            temp += to_string(ce->getImag()) + ";";
            temp += to_string(iterationen);

            return temp;
        }
    };
}

#endif /* MANDELBROT_HPP_ */
