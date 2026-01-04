#include <pqxx/pqxx>
#include <vector>
#include <string>
#include <stdexcept>

// Конструктор с параметрами подключения
DatabaseConnection::DatabaseConnection(const std::string &connectionString)
{
    try
    {
        conn = std::make_unique<pqxx::connection>(connectionString);
        if (!conn->is_open())
        {
            throw std::runtime_error("Не удалось подключиться к базе данных");
        }
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error(std::string("Ошибка подключения: ") + e.what());
    }
}

// Выполнение запроса с возвратом результата
pqxx::result DatabaseConnection::executeQuery(const std::string &sql)
{
    try
    {
        // Начинаем транзакцию
        pqxx::work txn(conn);

        // Выполняем запрос
        pqxx::result result = txn.exec(sql);

        // Подтверждаем транзакцию
        txn.commit();
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error(std::string("Ошибка запроса: ") + e.what());
    }

    return result;
}

// Выполнение запроса без возврата данных
void DatabaseConnection::executeNonQuery(const std::string &sql)
{
    try
    {
        pqxx::work work(*conn);
        work.exec(sql);
        work.commit();
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error(std::string("Ошибка выполнения: ") + e.what());
    }
}

// Начало транзакции
void DatabaseConnection::beginTransaction()
{
    if (transaction)
    {
        throw std::runtime_error("Транзакция уже начата");
    }
    transaction = std::make_unique<pqxx::work>(*conn);
}

// Завершение транзакции
void DatabaseConnection::commitTransaction()
{
    if (!transaction)
    {
        throw std::runtime_error("Нет активной транзакции");
    }
    transaction->commit();
    transaction.reset();
}

// Откат транзакции
void DatabaseConnection::rollbackTransaction()
{
    if (!transaction)
    {
        throw std::runtime_error("Нет активной транзакции");
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
~DatabaseConnection::DatabaseConnection()
{
    try
    {
        if (transaction)
        {
            transaction->abort();
        }
        if (conn && conn->is_open())
        {
            conn->disconnect();
        }
    }
    catch (...)
    {
        // Игнорируем ошибки в деструкторе
    }
}
