#include "vector2d.h"
#include "planet.h"
#include "datawidget.h"
#include "ui_datawidget.h"

DataWidget::DataWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataWidget)
{
    ui->setupUi(this);
}

DataWidget::DataWidget(size_t number) :
    DataWidget()
{
    index=number;
    ui->title->setText(QString("Planet %1").arg(number+1));
}

DataWidget::DataWidget(size_t number, Planet planet) :
    DataWidget()
{
    index=number;
    ui->title->setText(QString("Planet %1").arg(number+1));
    ui->pos_x->setValue(planet.location.x);
    ui->pos_y->setValue(planet.location.y);
    ui->vel_x->setValue(planet.velocity.x);
    ui->vel_y->setValue(planet.velocity.y);
    ui->color_r->setValue(planet.color.red());
    ui->color_g->setValue(planet.color.green());
    ui->color_b->setValue(planet.color.blue());
    ui->mass->setValue(planet.mass);
}

DataWidget::~DataWidget()
{
    delete ui;
}

Planet DataWidget::planet(){
    Planet planet;
    planet.location=Vector2d(ui->pos_x->value(), ui->pos_y->value());
    planet.velocity=Vector2d(ui->vel_x->value(), ui->vel_y->value());
    planet.color=QColor(ui->color_r->value(), ui->color_g->value(), ui->color_b->value());
    planet.mass = ui->mass->value();
    return planet;
}

void DataWidget::reindex(size_t index){
    this->index = index;
    ui->title->setText(QString("Planet %1").arg(index+1));
}

void DataWidget::on_deleteButton_clicked()
{
    emit deleteWidget(index);
}
