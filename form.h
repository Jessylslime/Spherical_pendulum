#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QTextEdit>
#include <memory>
#include <QTimer>
#include "system.h"
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <vector>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>


class Form : public QWidget
{
    Q_OBJECT

public:
    Form(QWidget *parent = nullptr);
    ~Form();
public slots:
    void start();
    void evolute();

    void stop();
private:
    QLabel *labelk1, *labelk2, *labelk3, *labelk4, *labelT0, *labelAlpha, *labelV_t0, *labelV_f0, *labelM;
    QDoubleSpinBox *doubleSpinboxK1, *doubleSpinboxK2, *doubleSpinboxK3, *doubleSpinboxK4,
        *doubleSpinboxT0, *doubleSpinboxAlpha, *doubleSpinboxV_t0, *doubleSpinboxV_f0, *doubleSpinboxM;
    QPushButton *pushButtonStart, *pushButtonStop;
    QTextEdit *textEditLog;
    QwtPlot *plotTheta, *plotFi;
    QwtPlotMagnifier *magnifierTheta, *magnifierFi;
    QwtPlotPanner *d_pannerTheta, *d_pannerFi;

    QTimer *timer;

    std::unique_ptr<System> sys_;
    std::vector<double> xx, tt, yy;

    QwtPlotCurve *curveTX, *curveTY;
};
#endif // FORM_H
