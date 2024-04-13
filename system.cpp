#include "system.h"
#include <cmath>


void System::F(double t, double *state, double *right) {
    double J = m*r*r;
    double g_t = g + a*w*w*sin(w*t);
    double Q_f = -alpha/m*state[3];
    double Q_th = -alpha/m*state[1];


    right[1] = sin(state[0])*cos(state[0])*state[3]*state[3]/J - g_t/r*sin(state[0]) + Q_th;
    right[0] = state[1];
    right[2] = state[3];
    right[3] = -2*state[1]*state[3]/tan(state[0]) + Q_f;
}

void System::rk4_Step (double dt) {
    int n = 4;
    double k[4][4];
    double arg_F[4];
    F(t, state, k[0]);
    for (int i = 0; i < n; i++) {
        arg_F[i] = state[i] + 0.5 * k[0][i] * dt;
    }
    F(t, arg_F, k[1]);
    for (int i = 0; i < n; i++) {
        arg_F[i] = state[i] + 0.5 * k[1][i] * dt;
    }
    F(t, arg_F, k[2]);
    for (int i = 0; i < n; i++) {
        arg_F[i] = state[i] + k[2][i] * dt;
    }
    F(t, arg_F, k[3]);
    for (int i = 0; i < n; i++) {
        state[i] = state[i] + (k[0][i] + 2*k[1][i] + 2*k[2][i] + k[3][i])*dt/6.0;
    }

    t = t+dt;

}


void System::RungeKutta4(double h) {
    int n = 4;
    double k1[n], k2[n], k3[n], k4[n], y[n];
    int i;

    for (i = 0; i < n; i++) {
        y[i] = state[i];
    }

    F(t, y, k1);
    for (i = 0; i < n; i++) {
        y[i] = state[i] + h * k1[i] / 2.0;
    }
    F(t + h / 2.0, y, k2);
    for (i = 0; i < n; i++) {
        y[i] = state[i] + h * k2[i] / 2.0;
    }
    F(t + h / 2.0, y, k3);
    for (i = 0; i < n; i++) {
        y[i] = state[i] + h * k3[i];
    }
    F(t + h, y, k4);

    for (i = 0; i < n; i++) {
        state[i] = state[i] + h * (k1[i] + 2.0 * k2[i] + 2.0 * k3[i] + k4[i]) / 6.0;
    }
    while(state[2] > 2.0*M_PI) {
        state[2] = state[2] - (2.0*M_PI);
    }
    t = t + h;
}
