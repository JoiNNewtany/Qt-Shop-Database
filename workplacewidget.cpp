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
                "SELECT c.Id, g.Name, g.Cost FROM Cart c, Goods g WHERE c.userID = ? AND g.Id = c.GoodId",
                userID);
    ui->addToCartButton->setEnabled(false);
    ui->removeFromCartButton->setEnabled(true);
}

void WorkplaceWidget::on_displayShopButton_clicked()
{
    UpdateModel(ui->userTableView, "SELECT * FROM Goods", -1);
    ui->addToCartButton->setEnabled(true);
    ui->removeFromCartButton->setEnabled(false);
}

void WorkplaceWidget::on_tabWidget_currentChanged(int index)
{
    switch (index)
    {
        case 0: // User

            UpdateModel(ui->userTableView, "SELECT * FROM Goods", -1);
            ui->addToCartButton->setEnabled(true);
            ui->removeFromCartButton->setEnabled(false);

            break;

        case 1: // Vendor

            UpdateModel(ui->vendorTableView, "SELECT * FROM Goods", -1);

            break;

        case 2: // Admin

            UpdateModel(ui->adminTableView, "SELECT * FROM Users", -1);

            break;
    }
}

void WorkplaceWidget::on_addGoodButton_clicked()
{
    // Add a good

    if (ui->goodNameEdit->text().length() > 0)
    {
        QSqlQuery q("INSERT INTO Goods(Id, Name, Cost) VALUES (NULL, ?, ?)");
        q.addBindValue(ui->goodNameEdit->text());
        q.addBindValue(ui->goodPriceEdit->value());
        q.exec();

        UpdateModel(ui->vendorTableView, "SELECT * FROM Goods", -1);
    }
}

void WorkplaceWidget::on_addToCartButton_clicked()
{
    QSqlQuery addToCartCheck("SELECT COUNT(*) FROM Goods");
    addToCartCheck.exec();
    addToCartCheck.next();

    if (ui->cartIdSpinBox->value() <= addToCartCheck.value(0).toInt())
    {
        QSqlQuery addToCart("INSERT INTO Cart (UserId, GoodId) VALUES (?, ?)");
        addToCart.addBindValue(userID);
        addToCart.addBindValue(ui->cartIdSpinBox->value());
        qDebug() << addToCart.exec();
    }
}

void WorkplaceWidget::on_removeGoodButton_clicked()
{
    QSqlQuery removeGood("DELETE FROM Goods WHERE Goods.Id = ?");
    removeGood.addBindValue(ui->goodIdSpinBox->value());
    qDebug() << removeGood.exec();

    UpdateModel(ui->vendorTableView, "SELECT * FROM Goods", -1);
}

void WorkplaceWidget::on_removeFromCartButton_clicked()
{
    QSqlQuery removeCart("DELETE FROM Cart WHERE Cart.Id = ?");
    removeCart.addBindValue(ui->cartIdSpinBox->value());
    qDebug() << removeCart.exec();

    UpdateModel(ui->userTableView,
        "SELECT c.Id, g.Name, g.Cost FROM Cart c, Goods g WHERE c.userID = ? AND g.Id = c.GoodId",
        userID);
}


void WorkplaceWidget::on_changeGoodButton_clicked()
{
    QSqlQuery changeGoodCheck("SELECT Id FROM Goods WHERE Id = ?");
    changeGoodCheck.addBindValue(ui->goodIdSpinBox->value());
    changeGoodCheck.exec();
    changeGoodCheck.next();

    if (!changeGoodCheck.value(0).isNull() &&
        ui->goodNameEdit->text().length() > 0)
    {
        QSqlQuery updateGood("UPDATE Goods SET Name = ?, Cost = ? WHERE Id = ?");
        updateGood.addBindValue(ui->goodNameEdit->text());
        updateGood.addBindValue(ui->goodPriceEdit->value());
        updateGood.addBindValue(ui->goodIdSpinBox->value());
        qDebug() << updateGood.exec();

        UpdateModel(ui->vendorTableView, "SELECT * FROM Goods", -1);
    }
}

void WorkplaceWidget::on_changeUserButton_clicked()
{
    QSqlQuery changeUserCheck("SELECT Id FROM Users WHERE Id = ?");
    changeUserCheck.addBindValue(ui->userIdSpinBox->value());
    changeUserCheck.exec();
    changeUserCheck.next();

    if (!changeUserCheck.value(0).isNull() &&
        ui->userNameEdit->text().length() > 0 &&
        ui->userPasswordEdit->text().length() > 0)
    {
        QSqlQuery updateUser("UPDATE Users SET Username = ?, Password = ?, AccessLv = ? WHERE Id = ?");
        updateUser.addBindValue(ui->userNameEdit->text());
        updateUser.addBindValue(ui->userPasswordEdit->text());
        updateUser.addBindValue(ui->userAccessLvEdit->currentIndex());
        updateUser.addBindValue(ui->userIdSpinBox->value());
        qDebug() << updateUser.exec();

        UpdateModel(ui->adminTableView, "SELECT * FROM Users", -1);
    }
}

void WorkplaceWidget::on_addUserButton_clicked()
{
    if (ui->userNameEdit->text().length() > 0 &&
        ui->userPasswordEdit->text().length())
    {
        QSqlQuery q("INSERT INTO Users(Username, Password, AccessLv) VALUES (?, ?, ?)");
        q.addBindValue(ui->userNameEdit->text());
        q.addBindValue(ui->userPasswordEdit->text());
        q.addBindValue(ui->userAccessLvEdit->currentIndex());
        q.exec();

        UpdateModel(ui->adminTableView, "SELECT * FROM Users", -1);
    }
}

void WorkplaceWidget::on_removeUserButton_clicked()
{
    QSqlQuery removeUser("DELETE FROM Users WHERE Users.Id = ?");
    removeUser.addBindValue(ui->userIdSpinBox->value());
    qDebug() << removeUser.exec();

    UpdateModel(ui->adminTableView, "SELECT * FROM Users", -1);
}
