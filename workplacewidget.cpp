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
                                 int userID) :
    QWidget(parent),
    ui(new Ui::WorkplaceWidget)
{
    ui->setupUi(this);

    db = _db;
    model = new QSqlQueryModel;

    // Checking access level

    QSqlQuery q("SELECT AccessLv FROM Users WHERE Id = ?");
    q.addBindValue(userID);
    q.exec();
    q.next();

    int id = q.value(1).toInt();

    switch (id)
    {
        case 0: // Admin

            this->setWindowTitle("Admin View");

            ui->userPage->setEnabled(true);
            ui->vendorPage->setEnabled(true);
            ui->adminPage->setEnabled(true);

            break;

        case 1: // Vendor

            this->setWindowTitle("Vendor View");

            ui->userPage->setEnabled(true);
            ui->vendorPage->setEnabled(true);

            break;

        case 2: // User

            this->setWindowTitle("User View");

            ui->userPage->setEnabled(true);

            break;

        default: // No access

            QMessageBox::critical(this,"Error",
                "Your access level is absent.\nPlease contact the administrator.");
            QCoreApplication::exit(0);

            break;
    }

    // Fill Cart table
    QSqlQuery q1("SELECT g.Name, g.Cost FROM Cart c, Goods g WHERE c.userID = ? AND g.Id = c.GoodId");
    q1.addBindValue(userID);
    FillTableQuery(ui->userTableView, q1);
}

void WorkplaceWidget::FillTableQuery(QTableView *table, QSqlQuery query)
{
    model->setQuery(query);
    table->setModel(model);
    table->show();
}

WorkplaceWidget::~WorkplaceWidget()
{
    delete ui;
}
