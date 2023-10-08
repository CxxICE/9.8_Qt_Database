#ifndef DATABASE_H
#define DATABASE_H

#include <QTableWidget>
#include <QSqlError>
#include <QSqlDatabase>
#include <QObject>
#include <QSqlTableModel>
#include <QSqlQueryModel>


#define POSTGRE_DRIVER "QPSQL"
#define DB_NAME "MyDB"

//Количество полей данных необходимых для подключения к БД
#define NUM_DATA_FOR_CONNECT_TO_DB 5

//Перечисление полей данных
enum FieldsForConnect
{
	kHost = 0,
	kDBName = 1,
	kUsername = 2,
	kPassword = 3,
	kPort = 4
};

//Типы запросов
enum RequestType
{
	kRequestAllFilms = 1,
	kRequestComedy   = 2,
	kRequestHorrors  = 3
};



class DataBase : public QObject
{
    Q_OBJECT

public:
    explicit DataBase(QObject *parent = nullptr);
    ~DataBase();

    void AddDataBase(QString driver, QString nameDB = "");
    void DisconnectFromDataBase(QString nameDb = "");
	void RequestToDB(RequestType request);
    QSqlError GetLastError(void);
    void ConnectToDataBase(QVector<QString> dataForConnect);


signals:

   void sig_SendDataFromDB(QSqlQueryModel *model);
   void sig_SendStatusConnection(bool);
   void sig_SendError(QString error);



private:

    QSqlDatabase* dataBase;
	QSqlTableModel* tModel;
	QSqlQueryModel* qModel;


};

#endif // DATABASE_H
