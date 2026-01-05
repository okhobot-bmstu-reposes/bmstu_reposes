#include <pqxx/pqxx>
#include <vector>
#include <string>
#include <iostream>
#include <databaseConnection.hpp>

// Конструктор с параметрами подключения
DatabaseConnection::DatabaseConnection(const std::string &connectionString)
{
    try
    {
        conn = std::make_unique<pqxx::connection>(connectionString);
        if (!conn->is_open())
        {
            std::cerr<<("Не удалось подключиться к базе данных");
        }
    }
    catch (const std::exception &e)
    {
        std::cerr<<(std::string("Ошибка подключения: ") + e.what());
    }
}

// Выполнение запроса с возвратом результата
pqxx::result DatabaseConnection::executeQuery(const std::string &sql)
{
    pqxx::result result;
    try
    {
        // Начинаем транзакцию
        beginTransaction();

        // Выполняем запрос
        result = transaction->exec(sql);

        // Подтверждаем транзакцию
        commitTransaction();
    }
    catch (const std::exception &e)
    {
        std::cerr<<(std::string("Ошибка запроса: ") + e.what())<<std::endl;
        rollbackTransaction();
    }

    return result;
}

// Выполнение запроса без возврата данных
void DatabaseConnection::executeNonQuery(const std::string &sql)
{
    try
    {
        beginTransaction();
        transaction->exec(sql);
        commitTransaction();
    }
    catch (const std::exception &e)
    {
        std::cerr<<(std::string("Ошибка выполнения: ") + e.what())<<std::endl;
        rollbackTransaction();
    }
}

// Начало транзакции
void DatabaseConnection::beginTransaction()
{
    if (transaction)
    {
        std::cerr<<("Транзакция уже начата");
    }
    transaction = std::make_unique<pqxx::work>(*conn);
}

// Завершение транзакции
void DatabaseConnection::commitTransaction()
{
    if (!transaction)
    {
        std::cerr<<("Нет активной транзакции");
    }
    transaction->commit();
    transaction.reset();
}

// Откат транзакции
void DatabaseConnection::rollbackTransaction()
{
    if (!transaction)
    {
        std::cerr<<("Нет активной транзакции");
    }
    transaction->abort();
    transaction.reset();
}

// Создание функции
void DatabaseConnection::createFunction(const std::string &functionName,
                                        const std::string &functionBody)
{
    std::string sql = "CREATE OR REPLACE FUNCTION " + functionName +
                      " RETURNS void AS $$ " + functionBody + " $$ LANGUAGE plpgsql;";
    executeNonQuery(sql);
}

// Создание триггера
void DatabaseConnection::createTrigger(const std::string &triggerName,
                                       const std::string &tableName,
                                       const std::string &timing, // BEFORE/AFTER
                                       const std::string &events, // INSERT/UPDATE/DELETE
                                       const std::string &functionName)
{
    std::string sql = "CREATE TRIGGER " + triggerName + " " + timing +
                      " " + events + " ON " + tableName +
                      " FOR EACH ROW EXECUTE FUNCTION " + functionName + ";";
    executeNonQuery(sql);
}

// Проверка состояния транзакции
std::string DatabaseConnection::getTransactionStatus() const
{
    if (!transaction)
    {
        return "Нет активной транзакции";
    }
    return "Транзакция активна";
}

// Деструктор
DatabaseConnection::~DatabaseConnection()
{

    if (transaction)
    {
        transaction->abort();
    }
    if (conn && conn->is_open())
    {
        conn->close();
    }
}
