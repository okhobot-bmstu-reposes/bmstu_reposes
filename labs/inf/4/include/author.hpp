#include <iostream>
#include <memory>
#include <pqxx/pqxx>
class Author
{
public:
    Author(int id, std::string name, int birth_year) : id(id), name(name), birth_year(birth_year) {}
    Author(const pqxx::result &result, int index = 0);
    void save(pqxx::connection &conn);

private:
    std::string name;
    int id, birth_year;
};
