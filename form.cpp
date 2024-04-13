#include "form.h"
#include <QLayout>
#include "system.h"

#include <QFile>
#include <filesystem>

#include <qwt_text.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>
#include <cmath>
#include <cstdlib>
#include <iostream>


Form::Form(QWidget *parent)
    : QWidget(parent)
{
    timer = new QTimer(this);
    timer -> setInterval(0);

    curveTX = new QwtPlotCurve("X");
    curveTX -> setPen(QColor("Red"), 3.0);
    curveTY = new QwtPlotCurve("Y");
    curveTY -> setPen(QColor("Blue"), 3.0);

    labelk1 = new QLabel("g");
    labelk2 = new QLabel("Цикл. Част.");
    labelk3 = new QLabel("Амплитуда");
    labelk4 = new QLabel("R");

    labelT0 = new QLabel("Theta_0");
    labelAlpha = new QLabel("alpha");
    labelV_t0 = new QLabel("V_t0");
    labelV_f0 = new QLabel("V_f0");

    labelM = new QLabel("m");

    doubleSpinboxK1 = new QDoubleSpinBox;
    doubleSpinboxK1 -> setDecimals(5);
    doubleSpinboxK1 -> setMaximum(1000);
    doubleSpinboxK1 -> setValue(1);

    doubleSpinboxK2 = new QDoubleSpinBox;
    doubleSpinboxK2 -> setDecimals(5);
    doubleSpinboxK2 -> setMaximum(1000);

    doubleSpinboxK3 = new QDoubleSpinBox;
    doubleSpinboxK3 -> setDecimals(5);
    doubleSpinboxK3 -> setMaximum(1000);

    doubleSpinboxK4 = new QDoubleSpinBox;
    doubleSpinboxK4 -> setDecimals(5);
    doubleSpinboxK4 -> setMaximum(1000);
    doubleSpinboxK4 -> setValue(1);

    doubleSpinboxT0 = new QDoubleSpinBox;
    doubleSpinboxT0 -> setDecimals(5);
    doubleSpinboxT0 -> setMaximum(1000);
    doubleSpinboxT0 -> setValue(10);

    doubleSpinboxAlpha = new QDoubleSpinBox;
    doubleSpinboxAlpha -> setDecimals(5);
    doubleSpinboxAlpha -> setMaximum(1000);

    doubleSpinboxV_t0 = new QDoubleSpinBox;
    doubleSpinboxV_t0 -> setDecimals(5);
    doubleSpinboxV_t0 -> setMaximum(1000);

    doubleSpinboxV_f0 = new QDoubleSpinBox;
    doubleSpinboxV_f0 -> setDecimals(5);
    doubleSpinboxV_f0 -> setMaximum(1000);

    doubleSpinboxM = new QDoubleSpinBox;
    doubleSpinboxM -> setDecimals(5);
    doubleSpinboxM->setValue(1);
    doubleSpinboxM -> setMaximum(1000);

    pushButtonStart = new QPushButton("Start");
    pushButtonStop = new QPushButton("Stop");
    pushButtonStop -> setEnabled(false);
    textEditLog = new QTextEdit;

    plotTheta = new QwtPlot(QwtText("Theta(time)"));
    curveTX->attach(plotTheta);
    plotTheta -> setFooter(QwtText("time (sec)"));
    plotTheta -> setAxisTitle(0, QwtText("Theta (rad)"));
    plotFi = new QwtPlot(QwtText("Fi(time)"));
    curveTY->attach(plotFi);
    plotFi -> setFooter(QwtText("time (sec)"));
    plotFi -> setAxisTitle(0, QwtText("Fi (rad)"));

    QwtPlotMagnifier *magnifierTheta = new QwtPlotMagnifier(plotTheta->canvas());
    magnifierTheta->setMouseButton(Qt::MiddleButton);
    QwtPlotPanner *d_pannerTheta = new QwtPlotPanner(plotTheta->canvas() );
    d_pannerTheta ->setMouseButton(Qt::LeftButton);

    QwtPlotMagnifier *magnifierFi = new QwtPlotMagnifier(plotFi->canvas());
    magnifierFi->setMouseButton(Qt::MiddleButton);
    QwtPlotPanner *d_pannerFi = new QwtPlotPanner(plotFi->canvas() );
    d_pannerFi ->setMouseButton(Qt::LeftButton);

    QGridLayout *layoutParamets = new QGridLayout;
    layoutParamets->addWidget(labelk1, 0, 0, Qt::AlignRight);
    layoutParamets->addWidget(doubleSpinboxK1, 0, 1);
    layoutParamets->addWidget(labelk2, 0, 2, Qt::AlignRight);
    layoutParamets->addWidget(doubleSpinboxK2, 0, 3);

    layoutParamets->addWidget(labelk3, 1, 0, Qt::AlignRight);
    layoutParamets->addWidget(doubleSpinboxK3, 1, 1);
    layoutParamets->addWidget(labelk4, 1, 2, Qt::AlignRight);
    layoutParamets->addWidget(doubleSpinboxK4, 1, 3);

    layoutParamets->addWidget(labelT0, 2, 0, Qt::AlignRight);
    layoutParamets->addWidget(doubleSpinboxT0, 2, 1);
    layoutParamets->addWidget(labelAlpha, 2, 2, Qt::AlignRight);
    layoutParamets->addWidget(doubleSpinboxAlpha, 2, 3);

    layoutParamets->addWidget(labelV_t0, 3, 0, Qt::AlignRight);
    layoutParamets->addWidget(doubleSpinboxV_t0, 3, 1);
    layoutParamets->addWidget(labelV_f0, 3, 2, Qt::AlignRight);
    layoutParamets->addWidget(doubleSpinboxV_f0, 3, 3);

    layoutParamets->addWidget(labelM, 4, 0, Qt::AlignRight);
    layoutParamets->addWidget(doubleSpinboxM, 4, 1);

    QHBoxLayout *layoutButtons = new QHBoxLayout;
    layoutButtons->addWidget(pushButtonStart);
    layoutButtons->addWidget(pushButtonStop);

    QVBoxLayout *layoutLeft = new QVBoxLayout;
    layoutLeft->addLayout(layoutParamets);
    layoutLeft->addLayout(layoutButtons);
    layoutLeft->addWidget(textEditLog);

    QVBoxLayout *layoutRight = new QVBoxLayout;
    layoutRight -> addWidget(plotTheta);
    layoutRight -> addWidget(plotFi);

    QGridLayout *layoutMain = new QGridLayout;
    layoutMain -> addLayout(layoutLeft, 0, 0);
    layoutMain -> addLayout(layoutRight, 0, 1);
    layoutMain -> setColumnStretch(1, 1);


    this->setLayout(layoutMain);

    connect(pushButtonStart, &QPushButton::clicked, this, &Form::start);
    connect(pushButtonStop, &QPushButton::clicked, this, &Form::stop);
    connect(timer, &QTimer::timeout, this, &Form::evolute);


}
void Form::start() {
    sys_.reset(new System(doubleSpinboxK1->value(),
                      doubleSpinboxK2->value(),
                      doubleSpinboxK3->value(),
                      doubleSpinboxK4->value(),
                      doubleSpinboxM->value(),
                      doubleSpinboxAlpha->value(),
                      doubleSpinboxT0->value(),
                      doubleSpinboxV_t0->value(),
                      doubleSpinboxV_f0->value()
                      ));
    int i = 0;
    std::string path = "C:/games/User_Files/QT/work/Data/";
    //std::filesystem::directory_entry dir{path + std::to_string(i)};
    bool Flag = true;
    while(Flag) {
        std::filesystem::directory_entry dir{path + std::to_string(i)};
        if (!dir.exists()) {
            Flag = false;
            continue;
        }
        i++;
    };
    std::filesystem::create_directory(path + std::to_string(i));
    QString param = QString("C:/games/User_Files/QT/work/Data/") + QString::number(i) + QString("/param.txt");

    QFile file(param);
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        sys_->params(stream);
    }
    file.close();
    textEditLog -> clear();
    xx.clear();
    yy.clear();
    tt.clear();
    pushButtonStart -> setEnabled(false);
    doubleSpinboxK1 -> setEnabled(false);
    doubleSpinboxK2 -> setEnabled(false);
    doubleSpinboxK3 -> setEnabled(false);
    doubleSpinboxK4 -> setEnabled(false);
    doubleSpinboxT0 -> setEnabled(false);
    doubleSpinboxAlpha -> setEnabled(false);
    doubleSpinboxV_t0 -> setEnabled(false);
    doubleSpinboxV_f0 -> setEnabled(false);
    doubleSpinboxV_f0 -> setDecimals(6);
    doubleSpinboxM -> setEnabled(false);

    pushButtonStop -> setEnabled(true);
    timer -> start();
}

void Form::stop() {
    timer -> stop();
    pushButtonStart -> setEnabled(true);
    pushButtonStop -> setEnabled(false);
    doubleSpinboxK1 -> setEnabled(true);
    doubleSpinboxK2 -> setEnabled(true);
    doubleSpinboxK3 -> setEnabled(true);
    doubleSpinboxK4 -> setEnabled(true);
    doubleSpinboxT0 -> setEnabled(true);
    doubleSpinboxAlpha -> setEnabled(true);
    doubleSpinboxV_t0 -> setEnabled(true);
    doubleSpinboxV_f0 -> setEnabled(true);
    doubleSpinboxM -> setEnabled(true);

    int i = 0;
    std::string path = "C:/games/User_Files/QT/work/Data/";
    std::filesystem::directory_entry dir{path + std::to_string(i)};
    bool Flag = true;
    while(Flag) {
        std::filesystem::directory_entry dir{path + std::to_string(i)};
        if (!dir.exists()) {
            Flag = false;
            i--;
            continue;
        }
        i++;
    };
    QString data = QString("C:/games/User_Files/QT/work/Data/") + QString::number(i) + QString("/data.txt");

    QFile file(data);
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream << textEditLog->toPlainText();
    }
    file.close();
    QString PyScript = QString("C:/games/User_Files/QT/work/Runner.bat");
    QFile Py(PyScript);
    if (Py.open(QIODevice::ReadWrite)) {
        QTextStream stream(&Py);
        stream << QString("@echo off") << QString("\n") << QString("python C:/games/User_Files/QT/work/xyz.py ") << QString::number(i);
    }
    Py.close();
    std::string command = ".\\RunScript.vbs";
    int result = std::system(command.c_str());

    if (result == 0) {
        std::cout << "Success" << std::endl;
    } else {
        std::cerr << "RIP" << std::endl;
    }
}

void Form::evolute()
{
    //double A = doubleSpinboxT0->value(),m = doubleSpinboxM->value(), b = doubleSpinboxAlpha->value()/2.0/m, g = doubleSpinboxK1->value(),
      //  r = doubleSpinboxK4->value(), w = sqrt(r/g - b*b);
    //double theory = A*exp(-b*sys_->t_())*cos(w*sys_->t_());
    textEditLog->append(
        QString::number(sys_->t_()) + " " +
        QString::number(sys_->th_()/M_PI*180) + " " +
        QString::number(sys_->fi_()/M_PI*180) + " " +
        QString::number(sys_->Vth_()) + " " +
        QString::number(sys_->Vfi_()));

    tt.push_back(sys_->t_());
    xx.push_back(sys_->th_()/M_PI*180);
    yy.push_back(sys_->fi_()/M_PI*180);
    curveTX->setRawSamples(tt.data(), xx.data(), tt.size());
    curveTY->setRawSamples(tt.data(), yy.data(), tt.size());
    plotTheta->replot();
    plotFi->replot();
    for (int i = 0; i < 100; i++)
        sys_ -> RungeKutta4(0.001);
}

Form::~Form()
{
}

