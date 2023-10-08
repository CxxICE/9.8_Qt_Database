#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //Исходное состояние виджетов
    ui->setupUi(this);
    ui->lb_statusConnect->setStyleSheet("color:red");
    ui->pb_request->setEnabled(false);

    /*
     * Выделим память под необходимые объекты. Все они наследники
     * QObject, поэтому воспользуемся иерархией.
    */

	dialogSQL = new LoginDialogSql(this);
    dataBase = new DataBase(this);
    msg = new QMessageBox(this);

    //Установим размер вектора данных для подключения к БД
	dataForConnection.resize(NUM_DATA_FOR_CONNECT_TO_DB);

    /*
     * Добавим БД используя стандартный драйвер PSQL и зададим имя.
    */
    dataBase->AddDataBase(POSTGRE_DRIVER, DB_NAME);

    /*
     * Устанавливаем данные для подключениея к БД.
     * Поскольку метод небольшой используем лямбда-функцию.
     */
	connect(dialogSQL, &LoginDialogSql::sig_sendData, this, [&](QVector<QString> receivData){
		dataForConnection = receivData;
    });

    /*
     * Соединяем сигнал, который передает ответ от БД с методом, который отображает ответ в ПИ
     */
     connect(dataBase, &DataBase::sig_SendDataFromDB, this, &MainWindow::ScreenDataFromDB);

    /*
     *  Сигнал для подключения к БД
     */
    connect(dataBase, &DataBase::sig_SendStatusConnection, this, &MainWindow::ReceiveStatusConnectionToDB);

	/*
	 *  Сигнал об ошибке запроса
	 */
	connect(dataBase, &DataBase::sig_SendError, this, &MainWindow::ReceiveError);

}

MainWindow::~MainWindow()
{
    delete ui;
}

/*!
 * @brief Слот отображает форму для ввода данных подключения к БД
 */
void MainWindow::on_act_addData_triggered()
{
    //Отобразим диалоговое окно. Какой метод нужно использовать?
	dialogSQL->show();
}

/*!
 * @brief Слот выполняет подключение к БД. И отображает ошибки.
 */

void MainWindow::on_act_connect_triggered()
{
    /*
     * Обработчик кнопки у нас должен подключаться и отключаться от БД.
     * Можно привязаться к надписи лейбла статуса. Если он равен
     * "Отключено" мы осуществляем подключение, если "Подключено" то
     * отключаемся
    */

    if(ui->lb_statusConnect->text() == "Отключено"){

       ui->lb_statusConnect->setText("Подключение");
       ui->lb_statusConnect->setStyleSheet("color : black");


	   auto conn = [&]{dataBase->ConnectToDataBase(dataForConnection);};
	   auto f = QtConcurrent::run(conn);

    }
    else{
        dataBase->DisconnectFromDataBase(DB_NAME);
        ui->lb_statusConnect->setText("Отключено");
        ui->act_connect->setText("Подключиться");
        ui->lb_statusConnect->setStyleSheet("color:red");
        ui->pb_request->setEnabled(false);
    }

}

/*!
 * \brief Обработчик кнопки "Получить"
 */
void MainWindow::on_pb_request_clicked()
{
	QString queryMode = ui->cb_category->currentText();
	if (queryMode == "Все")
	{
		auto task = [&](){dataBase->RequestToDB(RequestType::kRequestAllFilms);};
		auto f = QtConcurrent::run(task);
	}
	else if (queryMode == "Комедии")
	{
		auto task = [&](){dataBase->RequestToDB(RequestType::kRequestComedy);};
		auto f = QtConcurrent::run(task);
	}
	else if (queryMode == "Ужасы")
	{
		auto task = [&](){dataBase->RequestToDB(RequestType::kRequestHorrors);;};
		auto f = QtConcurrent::run(task);
	}

}

/*!
 * \brief Слот отображает значение в QTableWidget
 * \param widget
 * \param typeRequest
 */
void MainWindow::ScreenDataFromDB(QSqlQueryModel *model)
{
	//int numColumns = model->columnCount();
	//qDebug() << numColumns;
	ui->tb_result->setModel(model);
	model->setHeaderData(0,Qt::Horizontal, tr("Название фильма"));
	model->setHeaderData(1,Qt::Horizontal, tr("Описание фильма"));

	auto oldTableSize = ui->tb_result->width();
	ui->tb_result->resizeColumnToContents(0);
	ui->tb_result->resizeColumnToContents(1);
	ui->tb_result->resize(ui->tb_result->columnWidth(0) + ui->tb_result->columnWidth(1) + ui->tb_result->verticalHeader()->width() + 20, ui->tb_result->height());

	auto newTableSize = ui->tb_result->width();
	ui->centralwidget->resize(ui->centralwidget->width() + newTableSize - oldTableSize, ui->centralwidget->height());
	this->resize(this->width() + newTableSize - oldTableSize, this->height());

	auto pos = this->pos();
	auto newPosX = pos.x() - (newTableSize - oldTableSize) / 2.0;
	this->move(newPosX,pos.y());

	ui->tb_result->show();
}
/*!
 * \brief Метод изменяет стотояние формы в зависимости от статуса подключения к БД
 * \param status
 */
void MainWindow::ReceiveStatusConnectionToDB(bool status)
{
    if(status){
        ui->act_connect->setText("Отключиться");
        ui->lb_statusConnect->setText("Подключено к БД");
        ui->lb_statusConnect->setStyleSheet("color:green");
        ui->pb_request->setEnabled(true);
    }
    else{
        dataBase->DisconnectFromDataBase(DB_NAME);
        msg->setIcon(QMessageBox::Critical);
        msg->setText(dataBase->GetLastError().text());
        ui->lb_statusConnect->setText("Отключено");
        ui->lb_statusConnect->setStyleSheet("color:red");
        msg->exec();
    }

}

void MainWindow::ReceiveError(QString error)
{
	msg->setIcon(QMessageBox::Critical);
	msg->setText(error);
	msg->exec();
}




void MainWindow::on_pb_clear_clicked()
{
	ui->tb_result->setModel(nullptr);
	//ui->tb_result->reset();

}

