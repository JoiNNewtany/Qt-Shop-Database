#include "workplacewidget.h"
#include "ui_workplacewidget.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QSqlQueryModel>

WorkplaceWidget::WorkplaceWidget(QWidget *parent,
                                 QSqlDatabase *_db,
                                 int _userID) :
    QWidget(parent),
    ui(new Ui::WorkplaceWidget)
{
    ui->setupUi(this);

    db = _db;
    userID = _userID;
    model = new QSqlQueryModel;

    db->open();

    ui->userTableView->setModel(model);
    ui->vendorTableView->setModel(model);
    ui->adminTableView->setModel(model);

    // Checking access level

    QSqlQuery accessLv("SELECT AccessLv FROM Users WHERE Id = ?");
    accessLv.addBindValue(userID);
    accessLv.exec();
    accessLv.next();

    int id = accessLv.value(0).toInt();

    switch (id)
    {
        case 0: // Admin

            this->setWindowTitle("Admin View");

            // Display vendor and admin items
            //UpdateModel(ui->vendorTableView, "SELECT Name, Cost FROM Goods", -1);
            //UpdateModel(ui->adminTableView, "SELECT * FROM Users", -1);

            break;

        case 1: // Vendor

            this->setWindowTitle("Vendor View");

            ui->adminPage->~QWidget();

            // Display vendor items

            //UpdateModel(ui->vendorTableView, "SELECT Name, Cost FROM Goods", -1);

            break;

        case 2: // User

            this->setWindowTitle("User View");

            ui->adminPage->~QWidget();
            ui->vendorPage->~QWidget();

            break;

        default: // No access

            QMessageBox::critical(this,"Error",
                "Your access level is absent.\nPlease contact the administrator.");
            QCoreApplication::exit(0);

            break;
    }

    // Display shop items

    //UpdateModel(ui->userTableView, "SELECT Name, Cost FROM Goods", -1);
}

void WorkplaceWidget::UpdateModel(QTableView *table, QString query, int userid)
{
    // Fill a table view

    QSqlQuery q(query);

    // Using cringy implementation because that's enough here

    if (userid >= 0)
    {
        q.addBindValue(userid);
    }

    q.exec();
    model->setQuery(q);
    table->show();
}

WorkplaceWidget::~WorkplaceWidget()
{
    delete ui;
}

// ==== Slots ==== //

void WorkplaceWidget::on_displayCartButton_clicked()
{
    UpdateModel(ui->userTableView,
                "SELECT g.Name, g.Cost FROM Cart c, Goods g WHERE c.userID = ? AND g.Id = c.GoodId",
                userID);
    ui->addToCartButton->setEnabled(false);
    ui->removeFromCartButton->setEnabled(true);
}

void WorkplaceWidget::on_displayShopButton_clicked()
{
    UpdateModel(ui->userTableView, "SELECT Name, Cost FROM Goods", -1);
    ui->addToCartButton->setEnabled(true);
    ui->removeFromCartButton->setEnabled(false);
}

void WorkplaceWidget::on_tabWidget_currentChanged(int index)
{
    switch (index)
    {
        case 0: // User
            UpdateModel(ui->userTableView, "SELECT Name, Cost FROM Goods", -1);
            ui->addToCartButton->setEnabled(true);
            ui->removeFromCartButton->setEnabled(false);
        break;

        case 1: // Vendor
            UpdateModel(ui->vendorTableView, "SELECT Name, Cost FROM Goods", -1);
        break;

        case 2: // Admin
            UpdateModel(ui->adminTableView, "SELECT * FROM Users", -1);
        break;
    }
}
