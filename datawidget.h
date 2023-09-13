#ifndef DATAWIDGET_H
#define DATAWIDGET_H

#include "planet.h"
#include <QWidget>

namespace Ui {
class DataWidget;
}

class DataWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DataWidget(QWidget *parent = 0);
    explicit DataWidget(size_t number);
    explicit DataWidget(size_t number, Planet planet);

    ~DataWidget();

    Planet planet();
    size_t index=0;
    void reindex(size_t);

signals:
    void deleteWidget(size_t);

private slots:
    void on_deleteButton_clicked();

private:
    Ui::DataWidget *ui;
};

#endif // DATAWIDGET_H
