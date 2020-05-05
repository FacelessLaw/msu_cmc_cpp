#include <iostream>
#include <string>

const std::string templ_A_loop = "bab";

class Parser {
public:
    virtual void gc() = 0;
};
//ababbabbababbabbabbabd@
class Analyzer : public Parser 
{
    char curr_symbol;
    bool A() {
        if (this->curr_symbol == 'a') {
            bool at_least_one = false;
            bool fl = true;
            int it;
            while (fl) {
                it = 0;
                for (; fl && it < templ_A_loop.size(); ++it) {
                    this->gc();
                    if (templ_A_loop[it] != this->curr_symbol)
                        fl = false;
                }
                if (fl)
                    at_least_one = true;
            }
            if (at_least_one && it == 1)
                return true;
        }
        return false;
    }
    bool B() {
        if (this->curr_symbol == 'b') {
            this->gc();
            return A();
        } else if (this->curr_symbol == 'd')
            return true;
        return false;
    }
    bool S() {
        bool res = A() && B();
        if (this->curr_symbol != 'd')
            return false;
        this->gc();
        if (this->curr_symbol != '@')
            return false;
        return true;
    }
    
public:
    virtual void gc() {
        std::cin >> curr_symbol;
    }

    std::string operator~() {
        if (!S())
            return "error";
        return "success";
    }
};

int main ()
{
    Parser *p;
    Analyzer a;
    p = &a;
    p -> gc();
    std::cout << "----------";
    std::cout << ~a;
    std::cout << "----------";
    return 0;
}