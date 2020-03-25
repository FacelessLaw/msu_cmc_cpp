#include "lib/shop.hpp"

using namespace std;

int main()
{
    cout << "Check employee's functionality" << endl;
    cout << "*********************************" << endl;
    Employee nerd = Employee("JOHN", 34);
    cout << nerd.get_name() << ' ' << nerd.get_age() << ' ' << nerd.get_position() << endl;
    nerd.set_position(POSITION_CASHIER); 
    cout << nerd.get_position() << endl;
    nerd.set_position("POSITION_CASHIER");

    Employee second_nerd = Employee(nerd);
    cout << second_nerd.get_name() << ' ' << second_nerd.get_age() << ' ' <<
        second_nerd.get_position() << endl;

    Manager master_nerd = Manager("JACK", 35);
    cout << master_nerd.get_name() << ' ';
    cout << master_nerd.get_age() << ' ';
    cout << master_nerd.get_position() << endl;

    Manager master_nerd2 = Manager("Joseph", 50, 10);
    cout << "*********************************\n" << endl;


    cout << "Check chain shops functionality" << endl;
    cout << "*********************************" << endl;
    Shop shop1 = Shop("Pyaterochka", "Moscow str.", "88005553535");
    Warehouse wh1 = Warehouse("PyaterochkaWH", "Moscow", "8844", master_nerd);

    cout << "check copy of shop" << endl;
    Shop shop_copy = Shop(shop1);
    cout << shop_copy << endl;

    cout << "check copy of warehouse" << endl;
    Warehouse wh_copy = Warehouse(wh1);
    cout << wh_copy << endl;

    cout << "check warehouse staff logic" << endl;
    wh1.fire_employee(&master_nerd);
    cout << wh1 << endl << endl;
    wh1.hire_employee(&master_nerd2);
    cout << wh1 << endl << endl;

    cout << "check shop staff logic" << endl;
    shop1.hire_employee(&master_nerd2);
    cout << shop1 << endl << endl;
    shop1.hire_employee(&master_nerd);
    cout << shop1 << endl << endl;
    shop1.hire_employee(&nerd);
    cout << shop1 << endl << endl;

    shop1.fire_employee(&master_nerd);
    cout << shop1 << endl;
    cout << "**********************************" << endl;

    cout << "Total count of chains during tests: " << Chain::get_count_chains() << endl;
    return 0;
}