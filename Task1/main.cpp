#include <iostream>
#include <string>
#include "lib/complex.hpp"

using namespace std;

const string TEST = "1";
const string FREE = "2";
const string EXIT = "3";

void show_menu() {
    cout << "\n\n Welcome! It's a pretty simple interface of complex numbers library \n\n";
    cout << " Which mode do you fish to on? \n";
    cout << "1. Just looking at command line [1]\n";
    cout << "2. Free mode [2]\n";
    cout << "3. Exit? [3]\n\n";
}
void show_inpt_menu() {
    cout << "\n\n Format of complex numbers input: (x,y)\n";
    cout << " Choose format of statement: 1. (x,y)&(m,n)) [1]\n";
    cout << "                             2. (x,y)&a [2]\n";
    cout << "                             3. a&(x,y)) [3]\n";
    cout << " where & is one of: +, -, * \n";
    cout << " To see this window again print 0\n";
    cout << " Write Q to exit to main menu \n" << "Enjoy!\n\n";
}
Complex get_complex(double &a, double &b) {
    char junk;
    cin >> junk >> a >> junk >> b >> junk;
    return Complex(a, b);
}
void print_complex(Complex a) {
    cout << "\n" << a.get_x() << ' ' << a.get_y() << '\n';
}


int main()
{
    show_menu();
    while (true) {
        string inpt;
        cin >> inpt;

        if (inpt == TEST) {
            cout << "\n\n Just remember to type QUIT to open main menu someday...\n";
            while (true) {
                cin >> inpt;
                if (inpt == "QUIT") {
                    show_menu();
                    break;
                }
            }
        } else if (inpt == FREE) {
            show_inpt_menu();
            
            while (true) {
                double a_x, a_y;
                double b_x, b_y;
                char sign;
                char curr;
                cin >> curr;
                if (curr == '0')
                    show_inpt_menu();
                else if (curr == '1') {
                    cout << "INPUT (x,y)&(m,n): ";
                    Complex first_c = get_complex(a_x, a_y);
                    cin >> sign;
                    Complex second_c = get_complex(b_x, b_y);

                    if (sign == '+')
                        print_complex(first_c + second_c);
                    else if (sign == '-')
                        print_complex(first_c - second_c);
                    else if (sign == '*')
                        print_complex(first_c * second_c);
                    else
                        continue;
                } else if (curr == '2') {
                    cout << "INPUT (x,y)&a: ";
                    Complex first_c = get_complex(a_x, a_y);
                    cin >> sign;
                    cin >> b_x;

                    if (sign == '+')
                        print_complex(first_c + b_x);
                    else if (sign == '-')
                        print_complex(first_c - b_x);
                    else if (sign == '*')
                        print_complex(first_c * b_x);
                    else
                        continue;
                } else if (curr == '3') {
                    cout << "INPUT a&(x,y): ";
                    cin >> b_x;
                    cin >> sign;
                    Complex second_c = get_complex(a_x, a_y);

                    if (sign == '+')
                        print_complex(b_x + second_c);
                    else if (sign == '-')
                        print_complex(b_x - second_c);
                    else if (sign == '*')
                        print_complex(b_x * second_c);
                    else
                        continue;
                } else if (curr == 'Q') {
                    show_menu();
                    break;
                }
                cout << " Another try \n";
            }
        } else if (inpt == EXIT) {
            break;
            cout << " See u around \n\n";
        }
    }

    return 0;
}
