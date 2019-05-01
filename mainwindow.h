#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "connectmysql.h"
#include "qcustomplot.h"
#include <QVector>

struct _dataseriesinfo
{
    double max_val = 1;
    QString QuantityName = "";
    QString ArduinoKeyword;

};

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
    QVector<QCustomPlot*> plots;
    qreal _max, _min;
    connectmysql dbconnect;
    void setupplot(QCustomPlot *customPlot);
    QTimer dataTimer;
    QCPItemTracer *itemDemoPhaseTracer;
    QVector<QCPGraphData> timeData;
    bool ArduinoPresent = true;
    QVector<_dataseriesinfo> dataseriesinfo;
    int numdataseries = 2;

    QVector<QHBoxLayout*> horizontalLayouts;
    QVector<QLabel*> Labels;
    QVector<QLabel*> Values;


private slots:
    void on_recieve_data_clicked();
    void update();

};

#endif // MAINWINDOW_H
