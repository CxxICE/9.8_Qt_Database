#ifndef LOGINDIALOGSQL_H
#define LOGINDIALOGSQL_H

#include <QDialog>
//#include <QIntValidator>


namespace Ui {
class LoginDialogSql;
}

class LoginDialogSql : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialogSql(QWidget *parent = nullptr);
    ~LoginDialogSql();

signals:
	void sig_sendData(QVector<QString> dbData);

private slots:

    void on_pb_Cancel_clicked();
	void on_pb_Ok_clicked();

private:
    Ui::LoginDialogSql *ui;
	QVector<QString> dataForConnection;
	//QString username, dbname, password, host;
	//uint port;
//    QValidator *port_validator;
};

#endif // LOGINDIALOGSQL_H
