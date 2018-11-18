#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "connectmysql.h"
#include "qcustomplot.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    bool recieving_data = false;
    QTimer *timer;
    QCustomPlot* plot;
    qreal _max, _min;
    connectmysql dbconnect;
    void setupplot(QCustomPlot *customPlot);
    QTimer dataTimer;
    QCPItemTracer *itemDemoPhaseTracer;
    QVector<QCPGraphData> timeData;
    bool ArduinoPresent = true;
private slots:
    void on_recieve_data_clicked();
    void update();

};

#endif // MAINWINDOW_H
