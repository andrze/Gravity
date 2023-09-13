#ifndef DATAWINDOW_H
#define DATAWINDOW_H

#include <QMainWindow>
#include <memory>
#include <vector>
#include "datawidget.h"
#include "data.h"

namespace Ui {
class DataWindow;
}

class DataWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DataWindow(QMainWindow *parent = 0);
    explicit DataWindow(size_t n_planets);
    explicit DataWindow(Data data);

    ~DataWindow();

    void connectSignals();

    std::vector< std::unique_ptr<DataWidget> > input;
    int minimumHeight();
    Data extract_data();
signals:
    void confirmed();

private slots:
    void on_simulateButton_clicked();
    void on_searchButton_clicked();
    void on_saveButton_clicked();
    void on_addPlanetButton_clicked();
    void deleteWidget(size_t index);
    void on_r_0Button_clicked();
    void on_r_1Button_clicked();
    void on_r_2Button_clicked();
    void on_r_3Button_clicked();
    void on_returnButton_clicked();

private:
    Ui::DataWindow *ui;
    void setupWidgets();
};

#endif // DATAWINDOW_H
