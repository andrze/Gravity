#include "simwidget.h"
#include "ui_simwidget.h"
#include <QPainter>
#include <QPainterPath>
#include <memory>
#include <cmath>

QPoint calc_parameters(double unit, QPoint center, Vector2d location){
    int x = center.x() + location.x*unit;
    int y = center.y() + location.y*unit;
    return QPoint(x,y);
}

SimWidget::SimWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SimWidget)
{
    ui->setupUi(this);
    timer = std::make_unique<QTimer>(this);
    connect(timer.get(), SIGNAL(timeout()), this, SLOT(sim_move()));
}

SimWidget::~SimWidget()
{
    delete ui;
}

void SimWidget::init(){
    data.init();
    QPalette p = palette();
    QColor color(0,0,0);
    p.setColor(backgroundRole(), color);
    setPalette(p);
    this->update();
}

float color_calc(int phase, int color){
    return std::abs( color-100+100*sin(static_cast<double>(phase)/100) );
}

QColor full_color_calc(int phase, QColor color){
    float r=color_calc(phase+100, color.red()),
            g=color_calc(phase+200, color.green()),
            b=color_calc(phase, color.blue());
    float color_sum = r + g + b;
    if( color_sum != 0 ){
        r = 255./color_sum*r;
        g = 255./color_sum*g;
        b = 255./color_sum*b;
    }
    return QColor(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
}

void SimWidget::sim_move(){
    int non_drawn_moves=50;
    QPoint center(width()/2, height()/2);
    for (int i=0; i < non_drawn_moves; i++){
        data.move();
    }
    for (auto &&p: data.planets){
        if (p.simulated){
            QPoint end = calc_parameters(unit, center, p.location);
            QColor color = full_color_calc(phase, p.color);
            int color_sum = color.red()+color.green()+color.blue();
            if( color_sum == 0 ){
                points.push_back(ColorPoint({ end, full_color_calc(phase-1,p.color) }));
            } else {
                points.push_back(ColorPoint({end, color}));
            }
        }
    }
    phase++;
    this->update();
}

void SimWidget::paintEvent(QPaintEvent *){
    QPainter painter(this);
    for(auto&& p: points){
        QPainterPath path;
        path.addEllipse(p.point, 4, 4);
        painter.fillPath(path, p.color);
    }
    points.clear();
}

void SimWidget::start(){
    this->timer->start(5);
}

void SimWidget::stop(){
    this->timer->stop();
}

bool SimWidget::isActive(){
    return this->timer->isActive();
}
