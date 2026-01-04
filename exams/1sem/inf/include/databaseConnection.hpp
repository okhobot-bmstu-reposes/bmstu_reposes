#include <pqxx/pqxx>
#include <vector>
#include <string>
#include <stdexcept>

class DatabaseConnection
{
private:
    std::unique_ptr<pqxx::connection> conn;
    std::unique_ptr<pqxx::work> transaction;

public:
    // Конструктор с параметрами подключения
    DatabaseConnection(const std::string &connectionString)

    // Выполнение запроса с возвратом результата
    pqxx::result executeQuery(const std::string &sql);

    // Выполнение запроса без возврата данных
    void executeNonQuery(const std::string &sql);

    // Начало транзакции
    void beginTransaction();

    // Завершение транзакции
    void commitTransaction();

    // Откат транзакции
    void rollbackTransaction();

    // Создание функции
    void createFunction(const std::string &functionName, const std::string &functionBody);

    // Создание триггера
    void createTrigger(const std::string &triggerName,
                       const std::string &tableName,
                       const std::string &timing, // BEFORE/AFTER
                       const std::string &events, // INSERT/UPDATE/DELETE
                       const std::string &functionName);
    // Проверка состояния транзакции
    std::string getTransactionStatus() const;

    // Деструктор
    ~DatabaseConnection();
};
