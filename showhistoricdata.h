#ifndef SHOWHISTORICDATA_H
#define SHOWHISTORICDATA_H

#include <QDialog>
#include <qcustomplot.h>

namespace Ui {
class ShowHistoricData;
}

class ShowHistoricData : public QDialog
{
    Q_OBJECT

public:
    explicit ShowHistoricData(QWidget *parent = nullptr);
    ~ShowHistoricData();
    QCustomPlot *plot;

private:
    Ui::ShowHistoricData *ui;

private slots:
    void on_refresh();
};

#endif // SHOWHISTORICDATA_H
