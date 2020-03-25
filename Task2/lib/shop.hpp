#include <set>
#include <map>
#include "employee.hpp"

const int DEFAULT_STAFF_LIMIT = 10;

struct NoPlaceInStaff : public exception {
   const char * what () const throw () {
      return "There is no empty slots in those shop staff";
   }
};

class Chain
{
    static int count_chains;
    int el_counts;
    Chain* elements;
protected:
    string name, addr, phone_number;
    int staff_size;
    int staff_limit;
    Employee* staff;
public:
    Chain(
        const string _name, const string _addr = "", const string _phone = "", 
        const int _staff_limit = DEFAULT_STAFF_LIMIT
    ): 
    name(_name), addr(_addr), phone_number(_phone), staff_limit(_staff_limit) {
        ++count_chains;
        this->staff = new Employee[this->staff_limit];
        this->staff_size = 0;
    }
    Chain(const Chain& source): 
        staff_limit(source.get_staff_limit()), staff_size(source.get_staff_size()) {
        ++count_chains;
        this->staff = new Employee[this->staff_limit];
        for (int i = 0; i < this->staff_size; ++i) {
            this->staff[i] = source.staff[i];
        }
    }

    string get_name() const { return this->name; }
    int get_staff_size() const { return this->staff_size; }
    int get_staff_limit() const { return this->staff_limit; }
    Employee* get_staff() const { return this->staff; }
    string get_addr() const { return this->addr; }
    string get_phone_number() const { return this->phone_number; }

    

    virtual Manager get_top_manager() const = 0;
    virtual void hire_employee(const Employee* el) = 0;    
    virtual void fire_employee(const Employee* el) = 0;
    
    static int get_count_chains() { return count_chains; }

    ~Chain() {
        --count_chains;
        delete[] this->staff;
    }
};

int Chain::count_chains = 0;
ostream& operator<< (std::ostream &out, const Chain &chain) {
    out << "Chain: " << chain.get_name() << endl;
    out << "Address: " << chain.get_addr() << endl;
    out << "Phone_number: " << chain.get_phone_number() << endl;
    out << "Staff: " << endl;
    out << "******************************" << endl;
    for (int i = 0; i < chain.get_staff_size(); ++i) {
        out << "------------------------------" << endl;
        out << (chain.get_staff())[i] << endl;
        out << "------------------------------" << endl;
    }
    out << "******************************" << endl;
    out << "Top manager: " << endl;
    out << "------------------------------" << endl;
    out << chain.get_top_manager();
    out << "------------------------------" << endl;
    return out;
}


class Shop: public Chain
{
    map<string, int> goods_and_price;
public:
    Shop(
        const string _name, const string _addr, const string _phone_number,
        const int _staff_limit = DEFAULT_STAFF_LIMIT
    ): Chain(_name, _addr, _phone_number, _staff_limit) {}
    Shop(const Shop& source): 
        Shop(source.get_name(), source.get_addr(), source.get_phone_number(), 
        source.get_staff_limit()) {
        this->goods_and_price = source.get_list_of_goods();
    }

    virtual Manager get_top_manager() const {
        int record_access = 0;
        int it = 0;
        for (int i = 0; i < this->staff_size; ++i)
            if ((this->staff)[i].get_position() == POSITION_MANAGER && 
                ((this->staff)[i]).get_access_level() > record_access) {
                record_access = ((this->staff)[i]).get_access_level();
                it = i;
            }
        return ((this->staff)[it]);
    }
    virtual void hire_employee(const Employee* el) {
        try {
            if (this->staff_size + 1 < this->staff_limit) {
                (this->staff)[this->staff_size] = *el;
                ++(this->staff_size);
            } else
                throw (NoPlaceInStaff());
        } catch (NoPlaceInStaff& e) {
            cout << e.what() << endl;
        }
    }
    virtual void fire_employee(const Employee* el) {
        for (int i = 0; i < this->staff_size; ++i) {
            if ((this->staff)[i].get_name() == (*el).get_name() && 
                ((this->staff)[i]).get_age() == (*el).get_age()) {
                Employee tmp = (this->staff)[i];
                (this->staff)[i] = (this->staff)[this->staff_size - 1];
                (this->staff)[this->staff_size - 1] = tmp;
                --(this->staff_size);
            }
        }
    }
    
    map<string, int> get_list_of_goods() const { return this->goods_and_price; }
    
    friend ostream& operator<< (ostream &out, const Chain &chain);
};


class Warehouse: public Chain
{
    set<string> orders;
    Manager top_manager;
public:
    Warehouse(
        const string _name, const string _addr, const string _phone_number, const Manager _manager
    ): Chain(_name, _addr, _phone_number, 1), top_manager(_manager) {
        (this->staff)[0] = top_manager;
        (this->staff_size) = 1;
    }
    Warehouse(const Warehouse& source): 
        Warehouse(
            source.get_name(), source.get_addr(), source.get_phone_number(), 
            source.get_top_manager()
        ) {
        this->top_manager = source.get_top_manager();
        this->orders = source.get_orders();
        (this->staff)[0] = top_manager;
        (this->staff_size) = 1;
    }
    
    virtual Manager get_top_manager() const {
        return this->top_manager;
    }
    virtual void hire_employee(const Employee* el) {
        this->top_manager = *el;
    }
    virtual void fire_employee(const Employee* el) {
        this->staff_size = 0;
        this->top_manager = Manager();
    }
   
    set<string> get_orders() const { return this->orders; }
};
