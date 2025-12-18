#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>
#include <memory>
#include <sstream>

class Animal
{
protected:
    std::vector<std::string> health_events;
    std::string name;
    std::string species;
    int age;

    void display_health_events(std::ostream &outp) const
    {
        if (!health_events.empty())
        {
            outp << ", events: ";
            for (int i = 0; i < health_events.size(); i++)
            {
                if (i > 0)
                    outp << ", ";
                outp << health_events[i];
            }
        }
    }
    void load_health_events(std::ifstream &f)
    {
        std::string line, tmp = "";
        std::getline(f, line);

        for (int i = 0; i < line.size(); i++)
        {
            if (line[i] == '|')
            {
                health_events.push_back(tmp);
                tmp = "";
            }

            else
                tmp += line[i];
        }
        health_events.push_back(tmp);
    }
    void save_health_events(std::ofstream &f)
    {
        for (int i = 0; i < health_events.size(); i++)
        {
            if (i > 0)
                f << "|";
            f << health_events[i];
        }
    }

public:
    Animal(std::string n, std::string s, int a)
        : name(n), species(s), age(a) {}
    Animal() {}

    virtual void display(std::ostream &outp) const
    {
        outp << name << " (" << species << "), " << age << " years";
        display_health_events(outp);
        outp << std::endl;
    }
    virtual void load_from_file(std::ifstream &f) {}
    virtual void save_to_file(std::ofstream &f) {}

    std::string get_name() const { return name; }
    std::string get_species() const { return species; }
    int get_age() const { return age; }
    const std::vector<std::string> &get_health_events() const { return health_events; }
    std::vector<std::string> &get_health_events() { return health_events; }

    void set_name(const std::string &new_name) { name = new_name; }
    void set_species(const std::string &new_species) { species = new_species; }
    void set_age(int new_age) { age = new_age; }
    void set_health_events(const std::vector<std::string> &new_events) { health_events = new_events; }

    void add_event(std::string event) { health_events.push_back(event); }
    const std::vector<std::string> &get_events() const { return health_events; }
};

class Bird : public Animal
{
private:
    double wingspan;

public:
    Bird(std::string n, std::string s, int a,
         double w)
        : Animal(n, s, a), wingspan(w) {}
    Bird() {}

    double get_wingspan() const { return wingspan; }
    void set_wingspan(double new_wingspan) { wingspan = new_wingspan; }

    void display(std::ostream &outp) const override
    {
        outp << "Bird: " << get_name() << " (" << get_species() << "), " << get_age()
             << " years, wingspan: " << wingspan;
        display_health_events(outp);
        outp << std::endl;
    }
    void load_from_file(std::ifstream &f) override
    {
        std::string tmp;
        std::string name;
        std::string species;

        std::getline(f, name, ';');
        set_name(name);
        std::getline(f, species, ';');
        set_species(species);
        std::getline(f, tmp, ';');
        set_age(std::stoi(tmp));
        std::getline(f, tmp, ';');
        wingspan = std::stod(tmp);

        load_health_events(f);
    }
    void save_to_file(std::ofstream &f) override
    {
        f << "Bird;" << get_name() << ";" << get_species() << ";" << get_age()
          << ";" << wingspan << ";";
        save_health_events(f);
    }
};

class Mammal : public Animal
{
private:
    std::string fur_type;

public:
    Mammal(std::string n, std::string s, int a,
           std::string fur)
        : Animal(n, s, a), fur_type(fur) {}
    Mammal() {}

    std::string get_fur_type() const { return fur_type; }
    void set_fur_type(const std::string &new_fur_type) { fur_type = new_fur_type; }

    void display(std::ostream &outp) const override
    {
        outp << "Mammal: " << get_name() << " (" << get_species() << "), " << get_age()
             << " years, fur: " << fur_type;
        display_health_events(outp);
        outp << std::endl;
    }
    void load_from_file(std::ifstream &f) override
    {
        std::string tmp;
        std::string name;
        std::string species;

        std::getline(f, name, ';');
        set_name(name);
        std::getline(f, species, ';');
        set_species(species);
        std::getline(f, tmp, ';');
        set_age(std::stoi(tmp));
        std::getline(f, fur_type, ';');

        load_health_events(f);
    }
    void save_to_file(std::ofstream &f) override
    {
        f << "Mammal;" << get_name() << ";" << get_species() << ";" << get_age()
          << ";" << fur_type << ";";
        save_health_events(f);
    }
};

std::vector<std::shared_ptr<Animal>> load_animals(std::string file_name)
{
    std::vector<std::shared_ptr<Animal>> animals;
    std::ifstream input_file(file_name);
    std::string type;

    while (std::getline(input_file, type, ';'))
    {
        std::shared_ptr<Animal> animal;
        if (type == "Bird")
        {
            animal = std::make_shared<Bird>();
            animal->load_from_file(input_file);
        }
        else if (type == "Mammal")
        {
            animal = std::make_shared<Mammal>();
            animal->load_from_file(input_file);
        }
        animals.push_back(animal);
    }

    input_file.close();
    return animals;
}

void modify_animal(Animal *animal, std::string new_species, int new_age, std::string event)
{
    animal->set_species(new_species);
    animal->set_age(new_age);
    animal->add_event(event);
}

int main()
{
    std::vector<std::shared_ptr<Animal>> animals = load_animals("animals.txt");

    // Вывод всех животных
    std::cout << "All animals:" << std::endl;
    for (auto it = animals.begin(); it != animals.end(); it++)
        (*it)->display(std::cout);

    std::cout << "\nModifying first animal..." << std::endl;
    modify_animal(animals[0].get(), "NewSpecies", 10, "checkup");
    animals[0]->display(std::cout);

    // Подсчет млекопитающих старше 5 лет
    auto old_mammals = std::count_if(animals.begin(), animals.end(),
                                     [](const auto &animal)
                                     {
                                         auto mammal = dynamic_cast<Mammal *>(animal.get());
                                         return mammal && mammal->get_age() > 5;
                                     });
    std::cout << "\nMammals older than 5: " << old_mammals << std::endl;

    // Сортировка по имени
    std::sort(animals.begin(), animals.end(),
              [](const auto &animal_a, const auto &animal_b)
              {
                  return animal_a->get_name() < animal_b->get_name();
              });

    std::cout << "\nSorted by name:" << std::endl;
    for (const auto &animal : animals)
        animal->display(std::cout);

    auto fastest_bird = *std::max_element(animals.begin(), animals.end(),
                                         [](const auto &animal_a, const auto &animal_b)
                                         {
                                             auto bird_a = dynamic_cast<Bird *>(animal_a.get());
                                             auto bird_b = dynamic_cast<Bird *>(animal_b.get());
                                             double speed_a = bird_a ? bird_a->get_wingspan() : 0;
                                             double speed_b = bird_b ? bird_b->get_wingspan() : 0;
                                             return speed_a < speed_b;
                                         });
    std::cout << "\nFastest bird(by wingspan): " << std::endl;
    fastest_bird->display(std::cout);

    // Сортировка по возрасту и вывод старше 5
    std::sort(animals.begin(), animals.end(),
              [](const auto &animal_a, const auto &animal_b)
              {
                  return animal_a->get_age() < animal_b->get_age();
              });

    std::cout << "\nAnimals older than 5:" << std::endl;
    for (const auto &animal : animals)
        if (animal->get_age() > 5)
            animal->display(std::cout);

    // Подсчет по виду
    std::map<std::string, int> species_count;
    for (const auto &animal : animals)
        species_count[animal->get_species()]++;

    std::cout << "\nSpecies count:" << std::endl;
    for (const auto &[species, count] : species_count)
        std::cout << species << ": " << count << std::endl;

    // Поиск вакцинированных
    std::vector<std::shared_ptr<Animal>> vaccinated_animals;
    std::copy_if(animals.begin(), animals.end(),
                 std::back_inserter(vaccinated_animals),
                 [](const auto &animal)
                 {
                     const auto &events = animal->get_events();
                     return std::find(events.begin(), events.end(), "vaccinated") != events.end();
                 });

    std::cout << "\nVaccinated animals:" << std::endl;
    for (const auto &animal : vaccinated_animals)
        animal->display(std::cout);

    return 0;
}