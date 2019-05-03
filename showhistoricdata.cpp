#include "showhistoricdata.h"
#include "ui_showhistoricdata.h"
#include <qcustomplot.h>
#include <qsql.h>
#include <qsqlquery.h>


ShowHistoricData::ShowHistoricData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowHistoricData)
{

    ui->setupUi(this);
    plot = new QCustomPlot();
    plot->addGraph();
    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat("MM/dd, hh:mm");
    plot->xAxis->setTicker(dateTicker);
    //plot->yAxis->setLabel(dataseriesinfo[i].QuantityName);
    plot->update();
    plot->replot();
    ui->horizontalGraph->addWidget(plot);
    connect(ui->Refresh,SIGNAL(clicked()),this,SLOT(on_refresh()));
    QSqlQuery qry;

    QString sql = "SELECT RainGage.SensorData.Sensor FROM RainGage.SensorData group by RainGage.SensorData.Sensor";
    qry.prepare(sql);
    qry.exec();
    while (qry.next()) {
        QString variable = qry.value(0).toString();
        ui->comboBox->addItem(variable);
    }
}

ShowHistoricData::~ShowHistoricData()
{
    delete ui;
}

void ShowHistoricData::on_refresh()
{
    QSqlQuery qry;
    plot->yAxis->setLabel(ui->comboBox->itemData(ui->comboBox->currentIndex()).toString());
    QString sql = "SELECT * FROM RainGage.SensorData where RainGage.SensorData.Time > '" + ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss") + "' and RainGage.SensorData.Time < '" + ui->dateTimeEdit_2->dateTime().toString("yyyy-MM-dd hh:mm:ss") +"' AND RainGage.SensorData.Sensor = '" + ui->comboBox->currentText() + "'";
    qDebug()<<sql;
    qry.prepare(sql);
    qry.exec();
    double max_val = 0;
    while (qry.next()) {
        QCPGraphData datapoint;
        datapoint.key = qry.value(0).toDateTime().toTime_t();
        qDebug()<<datapoint.key;
        datapoint.value = qry.value(1).toDouble();
        max_val = qMax(max_val,datapoint.value);
        plot->graph(0)->addData(datapoint.key,datapoint.value);
    }
    plot->yAxis->setRange(0, max_val);
    plot->yAxis->setLabel(ui->comboBox->currentText());
    qDebug() << ui->dateTimeEdit->dateTime().toTime_t();
    plot->xAxis->setRange(ui->dateTimeEdit->dateTime().toTime_t(),ui->dateTimeEdit_2->dateTime().toTime_t());
    plot->xAxis->setLabel("Date/Time");
    plot->replot();
}
