#include <iostream>
#include <memory>
#include <pqxx/pqxx>
class User
{
public:
    User(int id, std::string name, std::string registration_date): id(id), name(name), registration_date(registration_date) {}
    User(const pqxx::result &result, int index=0);
    User(int id):id(id)
    {
        std::cin>>name>>registration_date;
    }
    void save(pqxx::connection &conn, std::ostream &outp);

private:
    std::string name,registration_date;
    int id;
};
