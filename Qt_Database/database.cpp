#include "database.h"

DataBase::DataBase(QObject *parent)
    : QObject{parent}
{

    dataBase = new QSqlDatabase();
	qModel = new QSqlQueryModel(this);
	tModel = nullptr;
}

DataBase::~DataBase()
{
    delete dataBase;
	delete tModel;
}

/*!
 * \brief Метод добавляет БД к экземпляру класса QSqlDataBase
 * \param driver драйвер БД
 * \param nameDB имя БД (Если отсутствует Qt задает имя по умолчанию)
 */
void DataBase::AddDataBase(QString driver, QString nameDB)
{

    *dataBase = QSqlDatabase::addDatabase(driver, nameDB);

}

/*!
 * \brief Метод подключается к БД
 * \param для удобства передаем контейнер с данными необходимыми для подключения
 * \return возвращает тип ошибки
 */
void DataBase::ConnectToDataBase(QVector<QString> data)
{

	dataBase->setHostName(data[FieldsForConnect::kHost]);
	dataBase->setDatabaseName(data[FieldsForConnect::kDBName]);
	dataBase->setUserName(data[FieldsForConnect::kUsername]);
	dataBase->setPassword(data[FieldsForConnect::kPassword]);
	dataBase->setPort(data[FieldsForConnect::kPort].toInt());

    bool status;
    status = dataBase->open( );
	if (status)
	{
		if(tModel == nullptr)
		{
			tModel = new QSqlTableModel(nullptr, *dataBase);
		}
		else
		{
			delete tModel;
			tModel = new QSqlTableModel(nullptr, *dataBase);
		}
	}
    emit sig_SendStatusConnection(status);
}
/*!
 * \brief Метод производит отключение от БД
 * \param Имя БД
 */
void DataBase::DisconnectFromDataBase(QString nameDb)
{

    *dataBase = QSqlDatabase::database(nameDb);
    dataBase->close();
	delete tModel;
	tModel = nullptr;
}
/*!
 * \brief Метод формирует запрос к БД.
 * \param request - SQL запрос
 * \return
 */
void DataBase::RequestToDB(RequestType request)
{
	bool status;
	switch(request)
	{
	case RequestType::kRequestAllFilms:

		tModel->setTable("film");
		tModel->setEditStrategy(QSqlTableModel::OnFieldChange);
		status = tModel->select();
		tModel->removeColumns(3,11);
		tModel->removeColumns(0,1);
		if(status)
		{
			emit sig_SendDataFromDB(tModel);
		}
		else
		{
			emit sig_SendError(tModel->lastError().text());
		}
		break;
	case RequestType::kRequestComedy:
		qModel->setQuery("SELECT title, description "
						 "FROM film f "
						 "JOIN film_category fc on f.film_id = fc.film_id "
						 "JOIN category c on c.category_id = fc.category_id "
						 "WHERE c.name = 'Comedy'",
						 *dataBase);
		if(qModel->lastError().type() == 0)
		{
			emit sig_SendDataFromDB(qModel);
		}
		else
		{
			emit sig_SendError(qModel->lastError().text());
		}

		break;
	case RequestType::kRequestHorrors:
		qModel->setQuery("SELECT title, description "
						 "FROM film f "
						 "JOIN film_category fc on f.film_id = fc.film_id "
						 "JOIN category c on c.category_id = fc.category_id "
						 "WHERE c.name = 'Horror'",
						 *dataBase);
		if(qModel->lastError().type() == 0)
		{
			emit sig_SendDataFromDB(qModel);
		}
		else
		{
			emit sig_SendError(qModel->lastError().text());
		}
		break;
	}
}



/*!
 * @brief Метод возвращает последнюю ошибку БД
 */
QSqlError DataBase::GetLastError()
{
    return dataBase->lastError();
}
