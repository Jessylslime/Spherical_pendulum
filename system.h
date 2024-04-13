#ifndef SYSTEM_H
#define SYSTEM_H

#include <vector>
#include <QFile>
#include <QTextStream>
#include <cmath>

class System
{
public:
    System(double g, double w, double a, double r, double m, double alpha, double T0, double v_t0, double v_f0): g(g), w(w), a(a), r(r), alpha(alpha), m(m) {
        state[0] = T0/180*M_PI;
        state[1] = v_t0;
        state[2] = 0.0;
        state[3] = v_f0;
    };
    void F(double t, double *state, double *right);
    void rk4_Step (double dt);
    double th_() const {return state[0];}
    double fi_() const {return state[2];}
    double Vth_() const {return state[1];}
    double Vfi_() const {return state[3];}
    double t_() const {return t;}
    QTextStream& params(QTextStream& os){
        os << QString("g: ") << g << '\n';
        os << QString("w: ") << w << '\n';
        os << QString("a: ") << a << '\n';
        os << QString("r: ") << r << '\n';
        os << QString("alpha: ") << alpha << '\n';
        os << QString("m: ") << m << '\n';
        os << QString("T0: ") << state[0] << '\n';
        os << QString("v_t0: ") << state[1] << '\n';
        os << QString("F0: ") << state[2] << '\n';
        os << QString("v_f0: ") << state[3] << '\n';
        return os;
    };

    void makestep(double dt);
    void RungeKutta4(double h);
    void f(double t, double y[], double dydt[]);
private:
    double g, w, a, r, alpha, m;
    double state[4];
    double t = 0;
};

#endif // SYSTEM_H
