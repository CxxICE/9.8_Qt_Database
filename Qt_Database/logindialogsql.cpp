#include "logindialogsql.h"
#include "ui_logindialogsql.h"
#include "database.h"


LoginDialogSql::LoginDialogSql(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialogSql)
{
    ui->setupUi(this);
    ui->le_password->setEchoMode(QLineEdit::EchoMode::Password);//
    //port_validator = new QIntValidator(0, 65535, this);
    //ui->le_port->setValidator(port_validator);
    QPixmap pm = QPixmap(1,1);
    pm.fill(QColor(0,0,0,0));
    setWindowIcon(QIcon(pm));

	dataForConnection.resize(NUM_DATA_FOR_CONNECT_TO_DB);

	//значения по умолчанию
	ui->le_host->setText("981757-ca08998.tmweb.ru");
	ui->sb_port->setValue(5432);
	ui->le_dbname->setText("netology_cpp");
	ui->le_username->setText("netology_usr_cpp");
	ui->le_password->setText("CppNeto3");
}

LoginDialogSql::~LoginDialogSql()
{
    delete ui;    
}

void LoginDialogSql::on_pb_Ok_clicked()
{
	dataForConnection[FieldsForConnect::kHost] = ui->le_host->text();
	dataForConnection[FieldsForConnect::kPort] = ui->sb_port->text();
	dataForConnection[FieldsForConnect::kDBName] = ui->le_dbname->text();
	dataForConnection[FieldsForConnect::kUsername] = ui->le_username->text();
	dataForConnection[FieldsForConnect::kPassword] = ui->le_password->text();
	emit sig_sendData(dataForConnection);
	this->close();
    return;
}


void LoginDialogSql::on_pb_Cancel_clicked()
{
    this->close();
    return;
}

