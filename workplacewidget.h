#ifndef WORKPLACEWIDGET_H
#define WORKPLACEWIDGET_H

#include <QWidget>

namespace Ui {
class WorkplaceWidget;
}

class WorkplaceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WorkplaceWidget(QWidget *parent = nullptr);
    ~WorkplaceWidget();

private:
    Ui::WorkplaceWidget *ui;
};

#endif // WORKPLACEWIDGET_H
