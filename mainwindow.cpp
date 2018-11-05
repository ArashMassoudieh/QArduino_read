#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <iostream>
#include <cstdio>
#include <qdebug.h>
#include <qtimer.h>
#include <qstringlist.h>
#include <qdatetime.h>
#include <qsqlquery.h>
// OS Specific sleep
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "serial/serial.h"

using std::string;
using std::exception;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;

void my_sleep(unsigned long milliseconds) {
#ifdef _WIN32
      Sleep(milliseconds); // 100 ms
#else
      usleep(milliseconds*1000); // 100 ms
#endif
}

void enumerate_ports()
{
    vector<serial::PortInfo> devices_found = serial::list_ports();

    vector<serial::PortInfo>::iterator iter = devices_found.begin();

    while( iter != devices_found.end() )
    {
        serial::PortInfo device = *iter++;

        printf( "(%s, %s, %s)\n", device.port.c_str(), device.description.c_str(), device.hardware_id.c_str() );
        qDebug()<<device.port.c_str()<< device.description.c_str()<<device.hardware_id.c_str();
    }
}

void print_usage()
{
    cerr << "Usage: test_serial {-e|<serial port address>} ";
    cerr << "<baudrate> [test string]" << endl;
}

int run(int argc, char **argv)
{
  if(argc < 2) {
      print_usage();
    return 0;
  }

  // Argument 1 is the serial port or enumerate flag
  string port(argv[1]);

  if( port == "-e" ) {
      enumerate_ports();
      return 0;
  }
  else if( argc < 3 ) {
      print_usage();
      return 1;
  }

  // Argument 2 is the baudrate
  unsigned long baud = 0;
#if defined(WIN32) && !defined(__MINGW32__)
  sscanf_s(argv[2], "%lu", &baud);
#else
  sscanf(argv[2], "%lu", &baud);
#endif

  // port, baudrate, timeout in milliseconds
  serial::Serial my_serial(port, baud, serial::Timeout::simpleTimeout(1000));

  cout << "Is the serial port open?";
  if(my_serial.isOpen())
    cout << " Yes." << endl;
  else
    cout << " No." << endl;

  // Get the Test string
  int count = 0;
  string test_string;
  if (argc == 4) {
    test_string = argv[3];
  } else {
    test_string = "Testing.";
  }

  // Test the timeout, there should be 1 second between prints
  cout << "Timeout == 1000ms, asking for 1 more byte than written." << endl;
  while (count < 10) {
    size_t bytes_wrote = my_serial.write(test_string);

    string result = my_serial.read(test_string.length()+1);

    cout << "Iteration: " << count << ", Bytes written: ";
    cout << bytes_wrote << ", Bytes read: ";
    cout << result.length() << ", String read: " << result << endl;

    count += 1;
  }

  // Test the timeout at 250ms
  my_serial.setTimeout(serial::Timeout::max(), 250, 0, 250, 0);
  count = 0;
  cout << "Timeout == 250ms, asking for 1 more byte than written." << endl;
  while (count < 10) {
    size_t bytes_wrote = my_serial.write(test_string);

    string result = my_serial.read(test_string.length()+1);

    cout << "Iteration: " << count << ", Bytes written: ";
    cout << bytes_wrote << ", Bytes read: ";
    cout << result.length() << ", String read: " << result << endl;

    count += 1;
  }

  // Test the timeout at 250ms, but asking exactly for what was written
  count = 0;
  cout << "Timeout == 250ms, asking for exactly what was written." << endl;
  while (count < 10) {
    size_t bytes_wrote = my_serial.write(test_string);

    string result = my_serial.read(test_string.length());

    cout << "Iteration: " << count << ", Bytes written: ";
    cout << bytes_wrote << ", Bytes read: ";
    cout << result.length() << ", String read: " << result << endl;

    count += 1;
  }

  // Test the timeout at 250ms, but asking for 1 less than what was written
  count = 0;
  cout << "Timeout == 250ms, asking for 1 less than was written." << endl;
  while (count < 10) {
    size_t bytes_wrote = my_serial.write(test_string);

    string result = my_serial.read(test_string.length()-1);

    cout << "Iteration: " << count << ", Bytes written: ";
    cout << bytes_wrote << ", Bytes read: ";
    cout << result.length() << ", String read: " << result << endl;

    count += 1;
  }

  return 0;
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(on_recieve_data_clicked()));
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000);
    series = new QLineSeries();

    chart = new QChart();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("Light Intensity");
    chart->legend()->hide();
    _max = qreal(QDateTime::currentDateTime().toSecsSinceEpoch()/86400.0+1.0/24.0/60.0);
    _min = qreal(QDateTime::currentDateTime().toSecsSinceEpoch()/86400.0);
    chart->axisX()->setMin(_min);
    chart->axisX()->setMax(_max);
    chart->axisY()->setMax(300);
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setChart(chart);
    ui->horizontalLayout_6->addWidget(chartView);



}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::update()
{
    if (recieving_data)
    {
        string port = "/dev/ttyACM0";
        unsigned long baud = 0;
        serial::Serial my_serial(port, baud, serial::Timeout::simpleTimeout(1000));
        QString result = QString::fromStdString(my_serial.read(1000));
        qDebug()<<result;
        QStringList resultssplitbycrtn = result.split(QRegExp("\r\n"));
        QStringList results;
        if (resultssplitbycrtn.size()>2)
        {   results = resultssplitbycrtn[resultssplitbycrtn.size()-2].split(",");
            qDebug()<<"Long string: "<<resultssplitbycrtn[resultssplitbycrtn.size()-1];
        }
        QSqlQuery qry;
        for (int i = 0; i<results.size(); i++)
        {   QString str = results[i].trimmed();
            qDebug()<<str;
            QDateTime _time = QDateTime::currentDateTime();
            if (str.contains("Light1"))
            {
                double value = str.right(str.size()-QString("Light1 = ").size()).trimmed().toFloat();
                ui->Value1->setText(QString::number(value));
                qreal t = qreal(_time.toSecsSinceEpoch()/86400.0);
                series->append(t,qreal(str.right(str.size()-QString("Light1 = ").size()).trimmed().toInt()));
                if (t>_max)
                {   _max = qreal(QDateTime::currentDateTime().toSecsSinceEpoch()/86400.0+1.0/24.0/60.0);
                    _min = qreal(QDateTime::currentDateTime().toSecsSinceEpoch()/86400.0-1.0/24.0/60.0);
                    chart->axisX()->setMin(_min);
                    chart->axisX()->setMax(_max);
                    QString sql = "INSERT INTO `BioRetentionData`.`SensorData` (`Time`, `Value`, `Sensor`) VALUES ('" + _time.toString("yyyy/MM/dd hh:mm:ss") + "', '" + value + "', 'Light1')";
                    qry.prepare(sql);
                    qry.exec();
                }

                chart->update();
                chartView->update();
                qDebug()<<QDateTime::currentDateTime().toSecsSinceEpoch()/86400.00;
            }
            if (str.contains("Light2"))
            {   double value = str.right(str.size()-QString("Light2 = ").size()).trimmed().toFloat();
                ui->Value2->setText(QString::number(value));
                QDateTime _time = QDateTime::currentDateTime();
                QString sql = "INSERT INTO `BioRetentionData`.`SensorData` (`Time`, `Value`, `Sensor`) VALUES ('" + _time.toString("yyyy/MM/dd hh:mm:ss") + "', '" + value + "', 'Light2')";
                qry.prepare(sql);
                if (!qry.exec())
                    qDebug()<<qry.lastError();
            }
            if (str.contains("Light3"))
            {
                double value = str.right(str.size()-QString("Light3 = ").size()).trimmed().toFloat();
                ui->Value3->setText(QString::number(value));
                QDateTime _time = QDateTime::currentDateTime();
                QString sql = "INSERT INTO `BioRetentionData`.`SensorData` (`Time`, `Value`, `Sensor`) VALUES ('" + _time.toString("yyyy/MM/dd hh:mm:ss") + "', '" + value + "', 'Light3')";
                qry.prepare(sql);
                qry.exec();

            }
            if (str.contains("Light4"))
            {   double value = str.right(str.size()-QString("Light4 = ").size()).trimmed().toFloat();
                ui->Value4->setText(QString::number(value));
                QDateTime _time = QDateTime::currentDateTime();
                QString sql = "INSERT INTO `BioRetentionData`.`SensorData` (`Time`, `Value`, `Sensor`) VALUES ('" + _time.toString("yyyy/MM/dd hh:mm:ss") + "', '" + value + "', 'Light4')";
                qry.prepare(sql);
                qry.exec();
            }
            ui->lblTime->setText(_time.toString("yyyy.MM.dd, hh:mm:ss"));
        }

        QCoreApplication::processEvents();

    }

}

void MainWindow::on_recieve_data_clicked()
{
    enumerate_ports();
    string str;
    enumerate_ports();
    if (!recieving_data)
    {   recieving_data = true;
        ui->pushButton->setText("Stop");

    }
    else
    {   recieving_data = false;
        ui->pushButton->setText("Start");
    }
}
