#include "simwindow.h"
#include "ui_simwindow.h"
#include <QTimer>

SimWindow::SimWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SimWindow)
{
    ui->setupUi(this);
}

SimWindow::SimWindow(Data data) :
    SimWindow()
{
    this->data = data;
    ui->widget->data = data;
    ui->widget->init();
}

SimWindow::~SimWindow()
{
    delete ui;
}

void SimWindow::on_pauseButton_clicked()
{
    if(ui->widget->isActive()){
        ui->pauseButton->setText(tr("Resume"));
        ui->widget->stop();
    }
    else{
        ui->pauseButton->setText(tr("Pause"));
        ui->widget->start();
    }
}

void SimWindow::on_stopButton_clicked(){
    this->ui->widget->stop();
    this->close();
}


void SimWindow::on_restartButton_clicked(){
    ui->widget->stop();
    ui->widget->data = data;
    ui->widget->init();
    ui->pauseButton->setText(tr("Start"));
}

void SimWindow::on_speedSlider_valueChanged(int value)
{
    double step = 0.000002*value;
    data.step = step;
    ui->widget->data.step = step;
}

void SimWindow::on_dataEditButton_clicked()
{
    emit editData();
}
