#include "dbms.h"

int main()
{

    std::cout << "Hi, enter the path to your database (csv file):" << std::endl;
    std::string inputPath;
    std::cin >> inputPath;
    DataBaseManagingSystem base(inputPath);

    std::string command = "";
    while(true)
    {
        base.awaitCommand();
    }

    // base.displayTable();
    // int x;
    // std::cin >> x;
    // return 0;
}