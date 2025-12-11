#include <iostream>
#include <vector>
#include <cstring>
#include <cmath>
#include <fstream>
#include <memory>
#define fname "data.txt"
using namespace std;
class Animal
{
public:
    virtual void display() const {}

protected:
    string name;
    vector<string> notes;
    Animal(){}
    Animal(string name) : name(name) {};
    void parse_notes(string data)
    {
        string tmp = "";
        for (int i = 0; i < data.size(); i++)
            if (data[i] == ';')
            {
                notes.push_back(tmp);
                tmp = "";
            }
            else
                tmp += data[i];
        // for (int i = 0; i < notes.size(); i++)std::cout<<notes[i]<<std::endl;
    }
void save_vars(int var_count, string prefix)
    {
        ofstream f(fname);
        f<<prefix<<";\n";
        for (int i = 0; i < var_count; i++)
            f << get_var(i) << ";";
        f<<"|";
        for (int i = 0; i < notes.size(); i++)
            f<<notes[i]<<";";
        f<<"|"<< std::endl;
        f.close();
    }
public:
    virtual string get_var(unsigned int index)
    {
        switch (index)
        {
        case 0:
            return name;
            break;

        default:
            std::cout << "error: no such index" << std::endl;
            return "";
            break;
        }
    }
    virtual void set_var(unsigned int index, string val)
    {
        switch (index)
        {
        case 0:
            name = val;
            break;

        default:
            std::cout << "error: no such index" << std::endl;
            break;
        }
    }
    void add_note(string note)
    {
        notes.push_back(note);
    }
    void remove_note(unsigned int index)
    {
        notes.erase(notes.begin()+index);
    }

    
    void parse(string data)
    {
        string tmp = "";
        unsigned int var_index = 0;
        for (int i = 0; i < data.size(); i++)
            if (data[i] == ';')
            {
                set_var(var_index, tmp);
                tmp = "";
                var_index++;
            }
            else if (data[i] == '|')
                parse_notes(data.substr(i + 1, data.find("|", i + 1)));
            else tmp+=data[i];
    }
};
class Bird : public Animal
{
    string wingspan;

public:
    Bird(){}
    Bird(string name, int wingspan) : wingspan(to_string(wingspan)), Animal(name) {}
    void display() const override
    {
        cout << "Bird;\nname: " << name << "; wingspan: " << wingspan << ";\nnotes:\n";
        for (int i = 0; i < notes.size(); i++)
            cout << notes[i] << "; ";
        cout << std::endl;
    }
    virtual string get_var(unsigned int index)
    {
        switch (index)
        {
        case 0:
            return name;
            break;
        case 1:
            return wingspan;
            break;

        default:
            std::cout << "error: no such index" << std::endl;
            return "";
            break;
        }
    }
    virtual void set_var(unsigned int index, string val)
    {
        switch (index)
        {
        case 0:
            name = val;
            break;
        case 1:
            wingspan = val;
            break;

        default:
            std::cout << "error: no such index" << std::endl;
            break;
        }
    }
    void save()
    {
        save_vars(2,"Bird");
    }
};

class Mammal : public Animal
{
    string msize;

public:
    Mammal(){}
    Mammal(string name, int msize) : msize(to_string(msize)), Animal(name) {}
    void display() const override
    {
        cout << "Mammal;\nname: " << name << "; size: " << msize << ";\nnotes:\n";
        for (int i = 0; i < notes.size(); i++)
            cout << notes[i] << "; ";
        cout << std::endl;
    }
    virtual string get_var(unsigned int index)
    {
        switch (index)
        {
        case 0:
            return name;
            break;
        case 1:
            return msize;
            break;

        default:
            std::cout << "error: no such index" << std::endl;
            return "";
            break;
        }
    }
    virtual void set_var(unsigned int index, string val)
    {
        switch (index)
        {
        case 0:
            name = val;
            break;
        case 1:
            msize = val;
            break;

        default:
            std::cout << "error: no such index" << std::endl;
            break;
        }
    }
    void save()
    {
        save_vars(2,"Mammal");
    }
};

int main()
{
    vector<shared_ptr<Animal>> animals;
    string com="";
    do
    {
        cin>>com;
        if(com=="load")
        {
            ifstream f(fname);
            while(std::getline(f, com))
            {
                int prefix_index=com.find(";");
                string prefix=com.substr(0,prefix_index);
                if(prefix=="Bird")
                animals.push_back(make_shared<Bird>());
                animals[animals.size()-1]->parse(com.substr(prefix_index));
            }
            f.close();
        }
    } while (com!="exit");
    
    return 0;
}
