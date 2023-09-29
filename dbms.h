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
    DBMS_ENTER_COMMAND_MANUALLY,
    DBMS_CLEAR_CONSOLE,
    DBMS_CLOSE_DATA_BASE
} SelectCommand;

typedef enum
{
    DBMS_MANUAL_BAD_COMMAND = 0,
    DBMS_MANUAL_GET_BY_PR_KEY,
    DBMS_MANUAL_ADD_NEW,
    DBMS_MANUAL_REMOVE_BY_PR_KEY,
} ManualCommand;

std::string commandToStr(SelectCommand command);
ManualCommand strToManual(const std::string& str_command);

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

        void commandContextMenu();
        void manualCommandInput(const std::string& command);

        std::string getByKey(const unsigned int key);
        void manuallyAddNewLine(const std::string& new_line);
        void manuallyRemoveByKey(const unsigned int key);

        void addNewLine();
        void removeLineByKey(const unsigned int pr_key);
        void displayTable();
};