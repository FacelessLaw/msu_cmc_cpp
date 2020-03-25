#include <string>
#include <exception>
#include <iostream>

using namespace std;

const string POSITION_INTERN = "INTERN";
const string POSITION_CASHIER = "CASHIER";
const string POSITION_MERCHANDISER = "MERCHANDISER";
const string POSITION_STEVEDORE = "STEVEDORE";
const string POSITION_MANAGER = "MANAGER";

struct PositionNotFound : public exception {
   const char * what () const throw () {
      return "Wrong type of Employee position";
   }
};

class Employee
{
    string position;
    int age;
protected:
    string name;
    unsigned int access_level;
public:
    Employee() { this->name = "Nobody"; }
    Employee(const string _name, const int _age, const string _pos = POSITION_INTERN): 
        name(_name), age(_age), position(_pos), access_level(0) {}
    Employee(const Employee& source): 
        Employee(source.get_name(), source.get_age(), source.get_position()) {}

    string get_name() const { return this->name; }
    int get_age() const { return this->age; }
    int get_access_level() const { return this->access_level; }

    virtual string get_position() const { return this->position; }
    virtual void set_position(string new_position) {
        try {
            if (new_position == POSITION_CASHIER) {
                this->position = POSITION_CASHIER;
            } else if (new_position == POSITION_MERCHANDISER) {
                this->position = POSITION_MERCHANDISER;
            } else if (new_position == POSITION_STEVEDORE) {
                this->position = POSITION_STEVEDORE;
            } else if (new_position == POSITION_MANAGER) {
                this->position = POSITION_MANAGER;
            } else if (new_position == POSITION_INTERN) {
                this->position = POSITION_INTERN;
            } else {
                throw (PositionNotFound());
            }
        } catch(PositionNotFound& e) {
            cout << e.what() << endl;
        }
    }
};

ostream& operator<< (std::ostream &out, const Employee &emp) {
    out << "Employee: " << emp.get_name() << endl;
    out << "Age: " << emp.get_age() << endl;
    out << "Position: " << emp.get_position() << endl;
    if (emp.get_position() == POSITION_MANAGER)
        out << "Access_level: " << emp.get_access_level() << endl;
    return out;
}


class Manager: public Employee
{
public:
    Manager() { this->name = "Nobody"; }
    Manager(const string _name, const int _age, const int _access_level = 0): 
        Employee(_name, _age) {
        (*((Employee*) this)).set_position(POSITION_MANAGER);
        this->access_level = _access_level;
    }
    Manager(const Manager& source): 
        Manager(source.get_name(), source.get_age(), source.get_access_level()) {}
    Manager(const Employee& source):
        Manager(source.get_name(), source.get_age(), source.get_access_level()) {}
    
    int get_access_level() const { return this->access_level; }

    virtual string get_position() const { return POSITION_MANAGER; }
    virtual void set_position(const int _access_level) { this->access_level = _access_level; }
};