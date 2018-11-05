#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include "connectmysql.h"

QT_CHARTS_USE_NAMESPACE

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
    QChart *chart;
    QChartView *chartView;
    QLineSeries *series;
    qreal _max, _min;
    connectmysql dbconnect;
private slots:
    void on_recieve_data_clicked();
    void update();

};

#endif // MAINWINDOW_H
