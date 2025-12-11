#include <iostream>
#include <memory>
#include <pqxx/pqxx>
class User
{
public:
    User(int id, std::string name, std::string registration_date): id(id), name(name), registration_date(registration_date) {}
    User(const pqxx::result &result, int index=0);
    void save(pqxx::connection &conn);

private:
    std::string name,registration_date;
    int id;
};
