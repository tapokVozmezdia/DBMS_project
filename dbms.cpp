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
        case DBMS_ADD_NEW:
            return "ADD NEW LINE";
        case DBMS_REMOVE_BY_LINE_NUM:
            return "REMOVE LINE BY LINE NUMBER";
        case DBMS_REMOVE_BY_PR_KEY:
            return "REMOVE LINE BY PRIMARY KEY";
        case DBMS_ENTER_COMMAND_MANUALLY:
            return "ENTER COMMAND MANUALLY";
        case DBMS_CLEAR_CONSOLE:
            return "CLEAR CONSOLE";
        default:
            return "INVALID COMMAND";
    }
}

ManualCommand strToManual(const std::string& str_command)
{
    if (str_command == "PRINT_TABLE")
    {
        return DBMS_MANUAL_PRINT_TABLE;
    }
    if (str_command == "GET")
    {
        return DBMS_MANUAL_GET_BY_PR_KEY;
    }
    if (str_command == "PUT")
    {
        return DBMS_MANUAL_ADD_NEW;
    }
    if (str_command == "REMOVE")
    {
        return DBMS_MANUAL_REMOVE_BY_PR_KEY;
    }
    if (str_command == "SET")
    {
        return DBMS_MANUAL_SET_VALUE;
    }
    if (str_command == "END")
    {
        return DBMS_MANUAL_END_CYCLE;
    }
    return DBMS_MANUAL_BAD_COMMAND;
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
            std::cout << "NOTE: multi-word args should be in form this_is_arg, " 
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
            this->shut_off_flag = true;
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

bool DataBaseManagingSystem::is_active()
{
    if (this->shut_off_flag)
    {
        std::cout << "Closing database..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(800));
    }
    return !(this->shut_off_flag);
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
            std::cout << "       " << lineNum;
        }
        else
        {
            std::cout << "line_num";
        }
        std::string tmp;
        std::stringstream s(line);
        while(getline(s, tmp, ' '))
        {
            std::cout << "\t" << tmp << "\t";
        }
        std::cout << std::endl;
        //std::cout << line << std::endl;
        lineNum++;
        this->numberOfMembers++;
    }
    dataBase.close();
}

std::string DataBaseManagingSystem::getByKey(const unsigned int key)
{
    std::string line;
    std::ifstream dataBase;
    dataBase.open(this->tablePath);
    std::stringstream t_structure(this->tableStructure);
    std::string r_answer = "";
    std::string tmp2;
    getline(t_structure, tmp2, ' ');

    if (dataBase.is_open())
    {
        std::string dump;
        getline(dataBase, dump);
        while(getline(dataBase, line))
        {
            std::string tmp;
            std::stringstream s(line);
            getline(s, tmp, ' ');
            if (stoi(tmp) == key)
            {
                r_answer = tmp2 + ": " + tmp;
                while(getline(s, tmp, ' '))
                {
                    getline(t_structure, tmp2, ' ');
                    r_answer += ("\n" + tmp2 + ": " + tmp);
                }
                dataBase.close();
                return r_answer;
            }
        }
        std::cout << "exception by identifier " << key << std::endl;
        error("NO ELEMENT BY THAT IDENTIFIER EXISTS");
    }
    dataBase.close();
    return "";
}

void DataBaseManagingSystem::changeByKey(const unsigned int key, std::string& param, std::string to_what)
{
    std::string line;
    std::ifstream dataBase;
    dataBase.open(this->tablePath);
    bool flag = false;
    std::list<std::list<std::string>> data;
    if (dataBase.is_open())
    {
        std::string dump;
        std::string word_d;
        getline(dataBase, dump);
        std::stringstream dump_handler(dump);
        std::stringstream param_scale(this->tableStructure);
        std::list<std::string> dump_list;
        while(getline(dump_handler, word_d, ' '))
        {
            dump_list.push_back(word_d);
        }
        data.push_back(dump_list);

        while(getline(dataBase, line))
        {
            std::string tmp;
            std::stringstream s(line);
            getline(s, tmp, ' ');
            if (stoi(tmp) != key)
            {
                std::list<std::string> tmp_list;
                tmp_list.push_back(tmp);
                while(getline(s, tmp, ' '))
                {
                    tmp_list.push_back(tmp);
                }
                data.push_back(tmp_list);
            }
            else
            {
                std::list<std::string> tmp_list;
                tmp_list.push_back(tmp);
                std::string param_in_scale;
                getline(param_scale, param_in_scale, ' ');
                while(getline(s, tmp, ' '))
                {
                    getline(param_scale, param_in_scale, ' ');
                    if (tmp == param || param_in_scale == param)
                    {
                        tmp_list.push_back(to_what);
                    }
                    else
                    {
                        tmp_list.push_back(tmp);
                    }
                }
                data.push_back(tmp_list);
                flag = true;
            }
        }
        if (!flag)
        {
            std::cout << "exception by identifier " << key << std::endl;
            error("NO ELEMENT BY THAT IDENTIFIER EXISTS");
            dataBase.close();
            return;
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
    dataWriter.close();
}

void DataBaseManagingSystem::manuallyAddNewLine(const std::string& new_line)
{
    std::ofstream dataBase;
    dataBase.open(this->tablePath, std::ios::app);
    if (dataBase.is_open())
    {
        dataBase << new_line << std::endl;
        this->numberOfMembers++;
    }
    dataBase.close();
}

void DataBaseManagingSystem::manuallyRemoveByKey(const unsigned int key)
{
    std::string line;
    std::ifstream dataBase;
    dataBase.open(this->tablePath);
    bool flag = false;
    std::list<std::list<std::string>> data;
    if (dataBase.is_open())
    {
        std::string dump;
        std::string word_d;
        getline(dataBase, dump);
        std::stringstream dump_handler(dump);
        std::list<std::string> dump_list;
        while(getline(dump_handler, word_d, ' '))
        {
            dump_list.push_back(word_d);
        }
        data.push_back(dump_list);

        while(getline(dataBase, line))
        {
            std::string tmp;
            std::stringstream s(line);
            getline(s, tmp, ' ');
            if (stoi(tmp) != key)
            {
                std::list<std::string> tmp_list;
                tmp_list.push_back(tmp);
                while(getline(s, tmp, ' '))
                {
                    tmp_list.push_back(tmp);
                }
                data.push_back(tmp_list);
            }
            else
            {
                flag = true;
            }
        }
        if (!flag)
        {
            std::cout << "exception by identifier " << key << std::endl;
            error("NO ELEMENT BY THAT IDENTIFIER EXISTS");
            dataBase.close();
            return;
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
        this->numberOfMembers++;
    }
    dataBase.close();
}

void DataBaseManagingSystem::removeLineByNum(const unsigned int pr_key)
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
    this->commandContextMenu();
}

void DataBaseManagingSystem::commandContextMenu()
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
        {
            std::cout << commandToStr(command) << std::endl;
            break;
        }
        case DBMS_ADD_NEW:
        {
            this->addNewLine();
            break;
        }
        case DBMS_REMOVE_BY_LINE_NUM:
        {
            std::cout << "ENTER THE POSITION YOU WANT TO REMOVE (line number):   ";
            unsigned int pr_key_input;
            std::cin >> pr_key_input;
            this->removeLineByNum(pr_key_input);
            break;
        }
        case DBMS_REMOVE_BY_PR_KEY:
        {
            std::cout << "ENTER THE PRIMARY KEY (ID) OF AN ELEMENT THAT YOU WANT TO REMOVE:   ";
            unsigned int pr_key_input;
            std::cin >> pr_key_input;
            this->manuallyRemoveByKey(pr_key_input);
            break;
        }
        case DBMS_ENTER_COMMAND_MANUALLY:
        {
            this->in_manual = true;
            while(this->in_manual)
            {
                std::string newCommand;
                getline(std::cin, newCommand);
                if (newCommand.length() == 0)
                {
                    getline(std::cin, newCommand);
                }
                this->manualCommandInput(newCommand);
            }
            break;
        }
        case DBMS_CLEAR_CONSOLE:
        {
            system("cls");
            break;
        }
        case DBMS_CLOSE_DATA_BASE:
        {
            std::cout << "Closing database..." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            exit(0);
        }
        default:
            break;
    }
}

void DataBaseManagingSystem::manualCommandInput(const std::string& command)
{
    std::stringstream s(command);
    std::string keyWord;
    std::string id_str;
    std::string suppline;
    unsigned int id;
    std::string param1;
    std::string param2;

    getline(s, keyWord, ' ');
    ManualCommand en_command = strToManual(keyWord);
    //std::cout << "KEY WORD: " << keyWord << std::endl;
    switch(en_command)
    {
        case DBMS_MANUAL_BAD_COMMAND:
        {
            error("BAD COMMAND GIVEN MANUALLY");
            break;
        }
        case DBMS_MANUAL_PRINT_TABLE:
        {
            std::cout << "WARNING: THE DATA MAY NOT BE DISPLAYED PROPERLY, ARE YOU SURE?" << std::endl;
            std::cout << "[Y/N]: ";
            std::string choice;
            std::cin >> choice;
            if (choice == "Y" || choice == "YES" || choice == "yes")
            {
                std::cout << "\n";
                this->displayTable();
                std::cout << "\n";
            }
            break;
        }
        case DBMS_MANUAL_GET_BY_PR_KEY:
        {
            getline(s, id_str, ' ');
            if (id_str.length() != 0)
            {
                //std::cout << id_str << std::endl;
                id = stoi(id_str);
                std::cout << this->getByKey(id) << std::endl;
            }
            break;
        }
        case DBMS_MANUAL_ADD_NEW:
        {
            getline(s, suppline);
            this->manuallyAddNewLine(suppline);
            break;
        }
        case DBMS_MANUAL_REMOVE_BY_PR_KEY:
        {
            getline(s, id_str, ' ');
            if (id_str.length() != 0)
            {
                //std::cout << id_str << std::endl;
                id = stoi(id_str);
                this->manuallyRemoveByKey(id);
            }
            break;
        }
        case DBMS_MANUAL_SET_VALUE: // SET id param to_what
        {
            getline(s, id_str, ' ');
            getline(s, param1, ' ');
            getline(s, param2);
            if (id_str.length() != 0 && param1.length() != 0 &&
                param2.length() != 0)
                {
                    id = stoi(id_str);
                    this->changeByKey(id, param1, param2);
                }
            break;
        }
        case DBMS_MANUAL_END_CYCLE:
        {
            this->in_manual = false;
            return;
        }
        default:
            error("BAD COMMAND GIVEN MANUALLY");
            break;
    }
}