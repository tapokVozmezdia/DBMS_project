// Объявление СУБД

#pragma once

//#include "db.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>
#include <list>
#include <vector>
#include <stdlib.h>

typedef enum
{
    DBMS_BAD_COMMAND = 0,
    DBMS_PRINT_TABLE,
    DBMS_ADD_NEW,
    DBMS_REMOVE_BY_PR_KEY,
    DBMS_CLEAR_CONSOLE,
    DBMS_CLOSE_DATA_BASE
} SelectCommand;

std::string commandToStr(SelectCommand command);

class DataBaseManagingSystem
{
    public:
        DataBaseManagingSystem(const std::string& path);
        ~DataBaseManagingSystem();

        void awaitCommand();

    private:
        std::string tablePath;
        std::string tableStructure;
        unsigned int numberOfArguments = 0;
        unsigned int numberOfMembers = 0;
        void addNewLine();
        void removeLineByKey(unsigned int pr_key);
        void displayTable();
};