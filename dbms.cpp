// Реализация СУБД

#include "dbms.h"

void error(const std::string& message)
{
    std::cout << "ERROR: " << message << std::endl; 
}

std::string commandToStr(SelectCommand command)
{
    switch(command)
    {
        case DBMS_BAD_COMMAND:
            return "BAD COMMAND";
        case DBMS_CLOSE_DATA_BASE:
            return "CLOSE DATA BASE";
        case DBMS_PRINT_TABLE:
            return "PRINT TABLE";
        case DBMS_ADD_NEW:
            return "ADD NEW";
        case DBMS_REMOVE_BY_PR_KEY:
            return "REMOVE BY PRIMARY KEY";
        case DBMS_CLEAR_CONSOLE:
            return "CLEAR CONSOLE";
        default:
            return "INVALID COMMAND";
    }
}

DataBaseManagingSystem::DataBaseManagingSystem(const std::string& path)
{
    std::ifstream dataBaseChecker;
    dataBaseChecker.open(path);
    if (!dataBaseChecker.is_open())
    {
        error("Opening of database file failed");
        dataBaseChecker.close();
        std::cout << "Create new data base with name " << path << "?" << std::endl;
        std::cout << "[Y/N]: ";
        std::string choice;
        std::cin >> choice;
        if (choice == "Y" || choice == "YES" || choice == "yes")
        {
            std::ofstream dataBaseCreator(path);
            std::cout << "Type in new table's argument structure. " <<
                "Example: name age gender favourite_colour\n";
            std::cout << "NOTE: multi-word args shoul be in form this_is_arg, " 
                << "ALL INPUT MUST BE IN ONE LINE" << std::endl;
            std::string firstLine;
            std::string moreDump;
            getline(std::cin, moreDump);
            getline(std::cin, firstLine);
            dataBaseCreator << firstLine << std::endl; 
            dataBaseCreator.close();
            std::cout << "\nnew table created\n" << std::endl;
            dataBaseChecker.open(path);
        }
        else{
            this->~DataBaseManagingSystem();
            return;
        }
    }
    getline(dataBaseChecker, this->tableStructure);
    std::stringstream counter(this->tableStructure);
    std::string dump;
    while(getline(counter, dump, ' '))
    {
        this->numberOfArguments++;
    }
    //std::cout << "NUM OF ARGS: " << this->numberOfArguments << std::endl;
    dataBaseChecker.close();
    this->tablePath = path;
}

DataBaseManagingSystem::~DataBaseManagingSystem()
{

}

void DataBaseManagingSystem::displayTable()
{
    std::ifstream dataBase;
    dataBase.open(this->tablePath);
    std::string line;
    unsigned int lineNum = 0;
    while(getline(dataBase, line))
    {
        if (lineNum != 0)
        {
            std::cout << "\t" << lineNum;
        }
        else
        {
            std::cout << "\tpr_key";
        }
        std::string tmp;
        std::stringstream s(line);
        while(getline(s, tmp, ' '))
        {
            std::cout << "\t\t" << tmp;
        }
        std::cout << std::endl;
        //std::cout << line << std::endl;
        lineNum++;
        this->numberOfMembers++;
    }
    dataBase.close();
}

void DataBaseManagingSystem::addNewLine()
{
    std::ofstream dataBase;
    dataBase.open(this->tablePath, std::ios::app);
    std::cout << "Print arguments for the new table entry in following order:" << std::endl;
    std::cout << this->tableStructure << std::endl;
    std::cout << "--input can be one string--" << std::endl;
    std::string arg;

    if (dataBase.is_open())
    {
        for (int i = 0; i < this->numberOfArguments; ++i)
        {
            std::cin >> arg;
            dataBase << arg << " ";
        }
        dataBase << std::endl;
    }

    this->numberOfMembers++;
    dataBase.close();
}

void DataBaseManagingSystem::removeLineByKey(unsigned int pr_key)
{
    if (pr_key > this->numberOfMembers || pr_key == 0)
    {
        error("TRYING TO REMOVE LINE BY KEY THAT DOES NOT EXIST");
        return;
    }
    std::list<std::list<std::string>> data;
    std::ifstream dataBase;
    dataBase.open(this->tablePath);
    if (dataBase.is_open())
    {
        std::string line;
        unsigned int lineNum = 0;
        while(getline(dataBase, line))
        {
            if (lineNum != pr_key)
            {
                std::string tmp;
                std::stringstream s(line);
                std::list<std::string> tmpList;
                while(getline(s, tmp, ' '))
                {
                    tmpList.push_back(tmp);
                }
                data.push_back(tmpList);
                //std::cout << line << std::endl;
            }
            lineNum++;
        }
    }
    dataBase.close();
    std::ofstream dataWriter;
    dataWriter.open(this->tablePath);
    if (dataWriter.is_open())
    {
        if (data.size() != 0)
        {
            for (auto i = data.begin(); i != data.end(); ++i)
            {
                for (auto j = (*i).begin(); j != (*i).end(); ++j)
                {
                    dataWriter << (*j) << " ";
                }
                dataWriter << std::endl;
            }
        }
    }
    this->numberOfMembers--;
    dataWriter.close();
}

void DataBaseManagingSystem::awaitCommand()
{
    std::cout << "COMMAND MENU" << std::endl;
    for (int i = 1; i <= DBMS_CLOSE_DATA_BASE; ++i)
    {
        std::cout << i << ") " << commandToStr((SelectCommand)i) << std::endl;
    }
    int commandNum;
    std::cin >> commandNum;
    SelectCommand command = (SelectCommand)commandNum;
    switch(command)
    {
        case DBMS_BAD_COMMAND:
            std::cout << commandToStr(command) << std::endl;
            break;
        case DBMS_PRINT_TABLE:
            std::cout << "\n";
            this->displayTable();
            std::cout << "\n";
            break;
        case DBMS_ADD_NEW:
            this->addNewLine();
            break;
        case DBMS_REMOVE_BY_PR_KEY:
            std::cout << "ENTER THE POSITION YOU WANT TO REMOVE (pr_key):   ";
            unsigned int pr_key_input;
            std::cin >> pr_key_input;
            this->removeLineByKey(pr_key_input);
            break;
        case DBMS_CLEAR_CONSOLE:
            system("cls");
            break;
        case DBMS_CLOSE_DATA_BASE:
            std::cout << "Closing database..." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            exit(0);
        default:
            break;
    }
}