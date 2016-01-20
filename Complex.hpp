
#ifndef COMPLEX_HPP_
#define COMPLEX_HPP_

#include <string>
#include <sstream>

#include <QString>

using namespace std;

namespace mandelbrot {

    class Complex {

        private: double real;
        private: double imag;


        public: Complex(double real, double imag) {
            this->real = real;
            this->imag = imag;
        }

        public: Complex(Complex *c) {
            this->real = c->real;
            this->imag = c->imag;
        }


        public: double betrag() {
			return real * real + imag * imag;
        }

        public: double getReal() {
            return real;
        }

        public: double getImag() {
            return imag;
        }


        public: void setReal(double real) {
            this->real = real;
        }

        public: void setImag(double imag) {
            this->imag = imag;
        }

        public: void set(Complex c) {
            this->real = c.getReal();
            this->imag = c.getImag();
        }


        public: string toString() {
			string zeichen;

            double a = real, b = 0.;

            zeichen = imag < 0.0 ? " - " : " + ";
            b = imag < 0.0 ? -imag : imag;

			ostringstream strs;
			strs << a << zeichen << b << 'i';

			return strs.str();
        }

        public: QString toQString(int genauigkeit) {
            QString zeichen;

            double a = real, b = 0.;

            zeichen = imag < 0.0 ? " - " : " + ";
            b = imag < 0.0 ? -imag : imag;

            QString aus;
            aus  = QString{"%1"}.arg(a, genauigkeit, 'f', genauigkeit, '0');
            aus += zeichen;
            aus += QString{"%1"}.arg(b, genauigkeit, 'f', genauigkeit, '0');
            aus += "i";

            return aus;
        }
    };
}

#endif /* COMPLEX_HPP_ */
