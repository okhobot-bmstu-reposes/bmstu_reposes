#include <iostream>
#include <memory>
#include <pqxx/pqxx>
class Author
{
public:
    Author(const pqxx::result &result, int index = 0);
    Author(int id, std::string name, int birth_year) : id(id), name(name), birth_year(birth_year) {}
    Author(int id):id(id)
    {
        std::cin>>name>>birth_year;
    }

    void save(pqxx::connection &conn, std::ostream &outp);

private:
    std::string name;
    int id, birth_year;
};
