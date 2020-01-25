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
                             int _userID);
    void UpdateModel(QTableView*, QString, int);
    ~WorkplaceWidget();

private slots:
    void on_displayCartButton_clicked();

    void on_displayShopButton_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_addGoodButton_clicked();

    void on_removeGoodButton_clicked();

    void on_addToCartButton_clicked();

    void on_removeFromCartButton_clicked();

    void on_removeUserButton_clicked();

private:

    Ui::WorkplaceWidget *ui;
    int userID;
    QSqlDatabase *db;           // Shop Database
    QSqlQueryModel *model;      // Stored Model
};

#endif // WORKPLACEWIDGET_H
