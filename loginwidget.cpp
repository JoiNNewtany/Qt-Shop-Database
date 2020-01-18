#include "loginwidget.h"
#include "ui_loginwidget.h"
#include "workplacewidget.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QSqlQuery>
#include <QtDebug>

LoginWidget::LoginWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginWidget)
{
    ui->setupUi(this);

    db = QSqlDatabase::addDatabase("QSQLITE");
    QString dbname = QFileDialog::getOpenFileName(this, "OPEN",
                                                  QApplication::applicationDirPath(), "*.db");
    db.setDatabaseName(dbname);
    db.open();

    if(db.isOpenError()) // Doesn't work
    {
        QMessageBox::critical(this,"Error", "Could not open database.\nApplication will exit.");
        QCoreApplication::exit(0);
    }
}

LoginWidget::~LoginWidget()
{
    delete ui;
}


void LoginWidget::on_loginButton_clicked()
{
    QString username = ui->usernameBox->text();
    QString password = ui->passwordBox->text();

    if (username.length() <= 0 && password.length() <= 0)
    {
        QMessageBox::critical(this,"Error", "You have to enter both username and password.");
        return;
    }

    QSqlQuery q("SELECT Password, Id FROM Users WHERE Username = ?");
    q.addBindValue(username);
    q.exec();
    q.next();
    QString psw = q.value(0).toString();    // Password
    int id = q.value(1).toInt();            // Id

    if(password == psw)
    {
        //QMessageBox::information(this, "Login", "Username and password are correct.");
        this->hide();
        auto workplace = new WorkplaceWidget(nullptr, &db, id);
        workplace->show();
    }
    else
    {
        QMessageBox::warning(this,"Login", "Username or password is not correct.");
    }
}

void LoginWidget::on_registerButton_clicked()
{
    QString username = ui->usernameBox->text();
    QString password = ui->passwordBox->text();

    if (username.length() <= 0 && password.length() <= 0)
    {
        QMessageBox::critical(this,"Error", "You have to enter both username and password.");
        return;
    }

    QSqlQuery q("SELECT Username FROM Users");
    q.exec();
    while (q.next())
    {
        QString usr = q.value(0).toString();
        if (username == usr)
        {
            QMessageBox::critical(this,"Error", "This username is already taken.");
            return;
        }
    }

    QSqlQuery query;
    query.prepare("INSERT INTO Users (Username, Password, AccessLv)"
                  "VALUES (?, ?, 2)"); // AccessLv 2 is user;
    query.addBindValue(username);
    query.addBindValue(password);
    query.exec();
}
