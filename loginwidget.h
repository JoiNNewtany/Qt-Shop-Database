#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QSqlDatabase>

QT_BEGIN_NAMESPACE
namespace Ui { class LoginWidget; }
QT_END_NAMESPACE

class LoginWidget : public QWidget
{
    Q_OBJECT

public:

    LoginWidget(QWidget *parent = nullptr);
    ~LoginWidget();

private slots:

    void on_loginButton_clicked();
    void on_registerButton_clicked();

private:

    Ui::LoginWidget *ui;
    QSqlDatabase db; // Shop database
};
#endif // LOGINWIDGET_H
