#include "models/server.cpp"

using namespace std;

int main()
{
    Server server=Server("log.txt");
    server.run();
    return 0;
}
