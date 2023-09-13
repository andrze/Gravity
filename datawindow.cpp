#include "datawindow.h"
#include "data.h"
#include "datawidget.h"
#include "ui_datawindow.h"
#include <QFileDialog>
#include <algorithm>
#include <fstream>
#include <memory>

#include <iostream>

DataWindow::DataWindow(QMainWindow* parent)
    : QMainWindow(parent)
    , ui(new Ui::DataWindow)
{
    ui->setupUi(this);
    QChar u0(0x70, 0x20), u1(0xB9, 0x00), u2(0xB2, 0x00), u3(0xB3, 0x00);
    ui->r_0Button->setText(tr("1/r") + u0);
    ui->r_1Button->setText(tr("1/r") + u1);
    ui->r_2Button->setText(tr("1/r") + u2);
    ui->r_3Button->setText(tr("1/r") + u3);
}

DataWindow::DataWindow(size_t n_planets)
    : DataWindow()
{
    input.clear();
    for (size_t i = 0; i < n_planets; i++) {
        input.push_back(std::make_unique<DataWidget>(i));
        ui->dataContainer->addWidget(input[i].get());
        input[i]->show();
    }
    setupWidgets();
    connectSignals();
}

DataWindow::DataWindow(Data data)
    : DataWindow()
{
    input.clear();
    for (size_t i = 0; i < data.planets.size(); i++) {
        input.push_back(std::make_unique<DataWidget>(i, data.planets[i]));
        ui->dataContainer->addWidget(input[i].get());
        input[i]->show();
    }
    switch (data.forceExponent) {
    case 0:
        ui->r_0Button->setChecked(true);
        break;
    case 1:
        ui->r_1Button->setChecked(true);
        break;
    case 3:
        ui->r_3Button->setChecked(true);
        break;
    default:
        ui->r_2Button->setChecked(true);
        break;
    }
    setupWidgets();
    connectSignals();
}

DataWindow::~DataWindow()
{
    delete ui;
}

void DataWindow::connectSignals()
{
    for (auto&& w : input) {
        connect(w.get(), SIGNAL(deleteWidget(size_t)), this, SLOT(deleteWidget(size_t)));
    }
}

Data DataWindow::extract_data()
{
    std::vector<Planet> planets;
    for (auto&& widget : input) {
        planets.push_back(widget->planet());
    }
    int forceExponent;
    if (ui->r_0Button->isChecked()) {
        forceExponent = 0;
    } else if (ui->r_1Button->isChecked()) {
        forceExponent = 1;
    } else if (ui->r_2Button->isChecked()) {
        forceExponent = 2;
    } else {
        forceExponent = 3;
    }

    return Data(planets, forceExponent);
}

void DataWindow::on_simulateButton_clicked()
{
    emit confirmed();
}

void DataWindow::on_searchButton_clicked()
{
    std::string filename = QFileDialog::getSaveFileName(this, tr("Save file"), "", tr("Data files (*.dat)")).toStdString();
    if (filename.size() == 0) {
        ui->infoLabel->setText(tr("Input a filename"));
        return;
    }
    if (filename.size() < 4 || filename.substr(filename.size() - 4, 4) != ".dat") {
        filename += ".dat";
    }
    ui->saveButton->setEnabled(true);
    ui->saveLine->setText(tr(&filename[0]));
}

void DataWindow::on_saveButton_clicked()
{
    std::string filename = ui->saveLine->text().toStdString();
    std::ofstream file(filename);
    file << extract_data();
    file.close();
    ui->infoLabel->setText(tr("Data saved to file."));
}

void DataWindow::setupWidgets()
{
    size_t n_planets = input.size();
    int widget_width = 130, widget_height = 420,
        line_height = 25, line_interval = 5 + line_height,
        first_line = this->height() - 5 - line_height, second_line = first_line - line_interval, third_line = second_line - line_interval,
        layout_width = (widget_width + 5) * n_planets - 5;
    this->setFixedWidth(std::max(static_cast<int>((5 + widget_width) * n_planets + 15), 400));
    ui->horizontalLayoutWidget->setGeometry((this->width() - layout_width) / 2, 80, layout_width, widget_height);
    ui->label->setGeometry(this->width() / 2 - 85, 0, 170, line_height);
    ui->addPlanetButton->setGeometry(20, third_line, 110, line_height);
    ui->simulateButton->setGeometry(this->width() - 90, first_line, 70, line_height);
    ui->saveButton->setGeometry(this->width() - 90, second_line, 70, line_height);
    ui->searchButton->setGeometry(this->width() - 120, second_line, 20, line_height);
    ui->saveLine->setGeometry(20, second_line, this->width() - 140, line_height);
    ui->infoLabel->setGeometry(20, first_line, this->width() - 180, line_height);
    ui->returnButton->setGeometry(this->width() - 160, first_line, 60, line_height);
    ui->label_4->setGeometry(20, 25, 200, 20);
    ui->horizontalLayoutWidget_2->setGeometry(20, 45, 360, 30);
}

void DataWindow::on_addPlanetButton_clicked()
{
    size_t size = input.size();
    input.push_back(std::make_unique<DataWidget>(size));
    ui->dataContainer->addWidget(input[size].get());
    connect(input[size].get(), SIGNAL(deleteWidget(size_t)), this, SLOT(deleteWidget(size_t)));
    setupWidgets();
}

void DataWindow::deleteWidget(size_t index)
{
    if (input.size() <= 2) {
        return;
    }
    input.erase(input.begin() + index);

    for (size_t i = index; i < input.size(); i++) {
        input[i]->reindex(i);
    }

    setupWidgets();
}

void DataWindow::on_r_0Button_clicked()
{
    ui->r_1Button->setChecked(false);
    ui->r_2Button->setChecked(false);
    ui->r_3Button->setChecked(false);
}

void DataWindow::on_r_1Button_clicked()
{
    ui->r_0Button->setChecked(false);
    ui->r_2Button->setChecked(false);
    ui->r_3Button->setChecked(false);
}

void DataWindow::on_r_2Button_clicked()
{
    ui->r_1Button->setChecked(false);
    ui->r_0Button->setChecked(false);
    ui->r_3Button->setChecked(false);
}

void DataWindow::on_r_3Button_clicked()
{
    ui->r_1Button->setChecked(false);
    ui->r_2Button->setChecked(false);
    ui->r_0Button->setChecked(false);
}

void DataWindow::on_returnButton_clicked()
{
    this->hide();
}
