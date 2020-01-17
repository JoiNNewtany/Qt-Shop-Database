#ifndef WORKPLACEWIDGET_H
#define WORKPLACEWIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QTableView>

namespace Ui {
class WorkplaceWidget;
}

class WorkplaceWidget : public QWidget
{
    Q_OBJECT

public:

    // Passing database and user's id from loginwidget

    explicit WorkplaceWidget(QWidget *parent,
                             QSqlDatabase *_db,
                             int userID);
    void FillTableQuery(QTableView*, QSqlQuery);
    ~WorkplaceWidget();

private:

    Ui::WorkplaceWidget *ui;
    QSqlDatabase *db;           // Shop Database
    QSqlQueryModel *model;      // Stored Model
};

#endif // WORKPLACEWIDGET_H
