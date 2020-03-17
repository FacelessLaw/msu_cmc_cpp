
using namespace std;

class Complex
{
    double x, y;
public:
    Complex(int _x = 0, int _y = 0): x(_x), y(_y) {}

    double get_x() { return this->x; }
    double get_y() { return this->y; }

    Complex& operator=(const Complex &src) {
        this->x = src.x;
        this->y = src.y;
        return *this;
    }
    Complex& operator=(const int &src) {
        this->x = src;
        this->y = 0;
        return *this;
    }
    friend Complex operator+(const Complex &a, const Complex &b);
    friend Complex operator+(const Complex &a, const int &b);
    friend Complex operator-(const Complex &a, const Complex &b);
    friend Complex operator-(const Complex &a, const int &b);
    friend Complex operator*(const Complex &a, const Complex &b);
    friend Complex operator*(const Complex &a, const int &b);
    ~Complex() {}
};

Complex operator+(const Complex &a, const Complex &b) {
    return Complex(a.x + b.x, a.y + b.y);
}
Complex operator+(const Complex &a, const int &b) {
    return Complex(a.x + b, a.y);
}
Complex operator-(const Complex &a, const Complex &b) {
    return Complex(a.x - b.x, a.y - b.y);
}
Complex operator-(const Complex &a, const int &b) {
    return Complex(a.x - b, a.y);
}
Complex operator*(const Complex &a, const Complex &b) {
    return Complex(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x);
}
Complex operator*(const Complex &a, const int &b) {
    return Complex(a.x * b, a.y * b);
}
