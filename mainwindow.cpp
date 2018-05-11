#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <time.h>

using namespace QtCharts;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);

    srand (time(NULL));

    QObject::connect(ui->runBtn, SIGNAL (clicked()), this, SLOT (run()));
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::run(){
    int nmrIt = ui->nrItBox->text().toInt();

    int cnt[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int cntAck = 0;
    int cntDecl = 0;

    if(nmrIt == 0){
        QMessageBox::warning(this, "Kein Iterationsanzahl", "Es wurde keine Anzahl von Iterationen angegeben.\nBitte geben Sie oben eine Zahl ein und starten erneut!");
        return;
    }

    //Check the selected simulation method
    if(ui->uniRadio->isChecked()){
        for(int i = 0; i<nmrIt; i++){
            int val = rand()%8;
            //qDebug() << val;
            cnt[val]++;

            if(val >= 6){
                cntDecl++;
            }else{
                cntAck++;
            }
        }
    }else if(ui->expRadio->isChecked()){
        for(int i = 0; i<nmrIt; i++){
            double val = (double)rand()/(RAND_MAX);
            double y = -1.0 * std::log(val);
            if(y == -1*std::numeric_limits<float>::infinity()){
                y = 8;
             }
            cnt[(int)y]++;

            if(y >= 6){
                cntDecl++;
            }else{
                cntAck++;
            }
        }
    }

    ui->accPakBox->setText(std::to_string(cntAck).c_str());
    ui->declPakBox->setText(std::to_string(cntDecl).c_str());

    QBarSet *set0 = new QBarSet("Delay");

    for(int i = 0; i<8; i++){
        *set0 << cnt[i];
    }

    QBarSeries *series = new QBarSeries();
    series->append(set0);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Verteilung der Laufzeit");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QStringList categories;
    categories << "19ms" << "20ms" << "21ms" << "22ms" << "23ms" << "24ms" << "25ms" << "26ms" << "27ms";
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);
    chart->createDefaultAxes();
    chart->setAxisX(axis, series);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    ui->graphicsView->setChart(chart);
}
