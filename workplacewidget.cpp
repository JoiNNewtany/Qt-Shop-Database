#include "workplacewidget.h"
#include "ui_workplacewidget.h"

WorkplaceWidget::WorkplaceWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WorkplaceWidget)
{
    ui->setupUi(this);
}

WorkplaceWidget::~WorkplaceWidget()
{
    delete ui;
}
