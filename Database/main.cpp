#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;
#define maxi 1024

static int callback(void* data, int argc, char** argv, char** azColName) //calback function used in sqlite
{
    // information from sql query to be displayed as:
    // atribute1 = value1
    // atribute2 = value2
    // ...

    char* information = (char*) data;
    for (int i = 0; i < argc; i++) 
    {
        strcat(information, azColName[i]);
        strcat(information, "=");
        if(argv[i])
        {
            strcat(information, argv[i]);
        }
        else
        {
            strcat(information,"-");
        }
        strcat(information,"\n");
    }
    strcat(information, "\n");
    return 0;
}

void insertQuery(sqlite3* db, string sqlQuery)
{
    char* SQL_errorMessage; // error message from sql query
    char data[maxi]; // callback argument
    string sqlQueryResult;
    sqlQueryResult.clear();
    data[0] = 0;
    
    int sqlExec = sqlite3_exec(db, sqlQuery.c_str(), callback, data, &SQL_errorMessage);
    if (sqlExec != SQLITE_OK)
    {
        sqlQueryResult = SQL_errorMessage;
        cout << sqlQueryResult;
        sqlite3_free (SQL_errorMessage);
    }
    else
    {
        data[strlen(data) - 1]=0;
        sqlQueryResult = data;
        cout << sqlQueryResult;
    }
}

string insertValues_Application()
{
    string cinBuffer; // reading from stdin
    string searchInfo; // holds information about sql query for searching db with a criteria
    string insertInfo; // holds information about sql query for inserting in db


    cout << "Complete the following information about your application.\n";
    searchInfo.clear();

    cout << "Project Name: "; 
    getline(cin, cinBuffer);
    
    while(cinBuffer.empty() == 1) 
    {
        cout << "Project Name field must be completed!" << endl;
        cout << "Project Name: "; 
        getline(cin, cinBuffer);
    }

    insertInfo = insertInfo + "\""+ cinBuffer + "\"";

    // DEVELOPER
    cinBuffer.clear(); //reset string
    cout << "Developer: "; 
    getline(cin, cinBuffer);

    if(cinBuffer.empty() == 1)
    {
        insertInfo = insertInfo + ",\"-\"";
    }
    else
    {
        insertInfo = insertInfo + ",\""+ cinBuffer + "\"";
    }

    // EXECUTABLE NAME
    cinBuffer.clear(); //reset string
    cout << "Executable: "; 
    getline(cin, cinBuffer);

    if(cinBuffer.empty() == 1)
    {
        insertInfo = insertInfo + ",\"-\"";
    }
    else
    {
        insertInfo = insertInfo + ",\""+ cinBuffer + "\"";
    }

    // LICENSE
    cinBuffer.clear(); //reset string
    cout << "License: "; 
    getline(cin, cinBuffer);

    if(cinBuffer.empty() == 1)
    {
        insertInfo = insertInfo + ",\"-\"";
    }
    else
    {
        insertInfo = insertInfo + ",\""+ cinBuffer + "\"";
    }

    // CATEGORY
    cinBuffer.clear(); //reset string
    cout << "App category: "; 
    getline(cin, cinBuffer);

    if(cinBuffer.empty() == 1)
    {
        insertInfo = insertInfo + ",\"-\"";
    }
    else
    {
        insertInfo = insertInfo + ",\""+ cinBuffer + "\"";
    }

    // INTERNET CONNECTION (YES/NO)
    cinBuffer.clear(); //reset string
    cout << "Internet Connection (YES/NO): "; 
    getline(cin, cinBuffer);

    if(cinBuffer.empty() == 1)
    {
        insertInfo = insertInfo + ",\"-\"";
    }
    else
    {
        insertInfo = insertInfo + ",\""+ cinBuffer + "\"";
    }

    // DESCRIPTION
    cinBuffer.clear(); //reset string
    cout << "Description: "; 
    getline(cin, cinBuffer);

    if(cinBuffer.empty() == 1)
    {
        insertInfo = insertInfo + ",\"-\"";
    }
    else
    {
        insertInfo = insertInfo + ",\""+ cinBuffer + "\"";
    }

    return insertInfo;
}

string insertValues_Minimum_Req()
{
    string cinBuffer; // reading from stdin
    string searchInfo; // holds information about sql query for searching db with a criteria
    string insertInfo; // holds information about sql query for inserting in db

    int ok = 0;

    cout << "Minimum requirements for the application:\n";
    searchInfo.clear();

    cout << "CPU (GHz): "; 
    getline(cin, cinBuffer);

    if(cinBuffer.empty() == 1)
    {
        insertInfo = insertInfo + "\"-\"";
    }
    else
    {
        insertInfo = insertInfo + "\""+ cinBuffer + "\"";
        ok = 1;
    }

    cinBuffer.clear();
    cout << "GPU: "; 
    getline(cin, cinBuffer);

    if(ok == 1)
    {
        if(cinBuffer.empty() == 1)
        {
            insertInfo = insertInfo + ",\"-\"";
        }
        else
        {
            insertInfo = insertInfo + ",\""+ cinBuffer + "\"";
        }
    }
    else
    {
        if(cinBuffer.empty() == 1)
        {
            insertInfo = insertInfo + "\"-\"";
        }
        else
        {
            insertInfo = insertInfo + "\""+ cinBuffer + "\"";
            ok = 1;
        }
    }


    cinBuffer.clear();
    cout << "RAM (GB): "; 
    getline(cin, cinBuffer);

    if(ok == 1)
    {
        if(cinBuffer.empty() == 1)
        {
            insertInfo = insertInfo + ",\"-\"";
        }
        else
        {
            insertInfo = insertInfo + ",\""+ cinBuffer + "\"";
        }
    }
    else
    {
        if(cinBuffer.empty() == 1)
        {
            insertInfo = insertInfo + "\"-\"";
        }
        else
        {
            insertInfo = insertInfo + "\""+ cinBuffer + "\"";
            ok = 1;
        }
    }

    cinBuffer.clear();
    cout << "Hard Disk Storage (GB): "; 
    getline(cin, cinBuffer);

    if(ok == 1)
    {
        if(cinBuffer.empty() == 1)
        {
            insertInfo = insertInfo + ",\"-\"";
        }
        else
        {
            insertInfo = insertInfo + ",\""+ cinBuffer + "\"";
        }
    }
    else
    {
        if(cinBuffer.empty() == 1)
        {
            insertInfo = insertInfo + "\"-\"";
        }
        else
        {
            insertInfo = insertInfo + "\""+ cinBuffer + "\"";
        }
    }

    return insertInfo;
}


void selectQuery(sqlite3* db, string select_data)
{
    char* SQL_errorMessage;
    char data[maxi]; // callback argument
    string sqlQueryResult;
    sqlQueryResult.clear();
    data[0] = 0;
    
    int sqlExec = sqlite3_exec(db, select_data.c_str(), callback, data, &SQL_errorMessage);
    if (sqlExec != SQLITE_OK)
    {
        sqlQueryResult = SQL_errorMessage;
        cout << sqlQueryResult;
        sqlite3_free (SQL_errorMessage);
    }
    else
    {
        data[strlen(data) - 1] = 0;
        sqlQueryResult = data;
        cout << endl << sqlQueryResult;
    }
}

string getAppID(sqlite3* db, string appName) 
{
    char* SQL_errorMessage; // error message from sql query
    char data[maxi]; // callback argument
    string sqlQueryResult;
    sqlQueryResult.clear();
    data[0] = 0;

    string sqlQuery = "SELECT AppID FROM Application WHERE AppName = \"" + appName + "\";";

    int sqlExec = sqlite3_exec(db, sqlQuery.c_str(), callback, data, &SQL_errorMessage);
    if (sqlExec != SQLITE_OK)
    {
        sqlQueryResult = SQL_errorMessage;
        cout << sqlQueryResult;
        sqlite3_free (SQL_errorMessage);
        return 0;
    }
    else
    {
        data[strlen(data) - 1] = '\0';
        sqlQueryResult = data;

        int position = sqlQueryResult.find("=");
        string appID =sqlQueryResult.substr(position + 1, sqlQueryResult.length() - position);

        return appID;
    }
} 


int main(int argc, char* argv[])
{
    sqlite3* db;
    int database = sqlite3_open("SoftwareApp.db", &db);

    if (database)
    {
        cout << "Error: " << sqlite3_errmsg (db) << endl;
        return 0;
    }
    else
    {
        cout << "Database connected succesfully...\n";
    }
        
    string cinBuffer; // reading from stdin
    string searchInfo; // sql query for searching db with a criteria
    string insertInfo; // sql query for inserting in db
    string sqlQuery; // sql query to be executed

    cout << "[1] To insert your Application, write \"Insert\". \n";
    cout << "[2] To search an Application, write \"Search\". \n";

    string inputCommand;
    cin >> inputCommand;
    cin.ignore();     // flush the \n character out of the buffer
    
    if(inputCommand == "Insert") // Insert app
    {
        insertInfo = insertValues_Application();
        sqlQuery.clear();

        sqlQuery = "INSERT INTO Application(AppName, Developer, Executable_name, License, Category, InternetConnection, AppInfo) VALUES(" + insertInfo + ");";
        insertQuery(db, sqlQuery);
        sqlQuery.clear();

        int secondposition = insertInfo.find("\"", 1); 
        string appName = insertInfo.substr(1,secondposition-1);
        string appID = getAppID(db, appName);
        insertInfo.clear();

        // inserare in OS table
        cout << "Operating System details:\n";
        cinBuffer.clear();

        cout << "OS distribution: "; 
        getline(cin, cinBuffer);

        if(cinBuffer.empty() == 1)
        {
            insertInfo = insertInfo + "\"-\"";
        }
        else
        {
            insertInfo = insertInfo + "\""+ cinBuffer + "\"";
        }

        sqlQuery = "INSERT INTO OS(AppID, OS_Name) VALUES("+ appID + ", " + insertInfo + ");";
        insertQuery(db, sqlQuery);
        sqlQuery.clear();
        insertInfo.clear();


        cout << "If you want to add more distributions, write ADD. If you don't want to, write STOP.\n";
        cinBuffer.clear();
        getline(cin, cinBuffer);
        while(cinBuffer != "STOP")
        {
            cout << "OS distribution: "; 
            getline(cin, cinBuffer);

            if(cinBuffer.empty() == 1)
            {
                insertInfo = insertInfo + "\"-\"";
            }
            else
            {
                insertInfo = insertInfo + "\""+ cinBuffer + "\"";
            }

            sqlQuery = "INSERT INTO OS(AppID, OS_Name) VALUES("+ appID + ", " + insertInfo + ");";
            insertQuery(db, sqlQuery);

            cinBuffer.clear();
            insertInfo.clear();
            cout << "If you want to add more distributions, write ADD. If you don't want to, write STOP.\n";
            getline(cin, cinBuffer);
        } 

        cinBuffer.clear();
        sqlQuery.clear();
        insertInfo.clear();

        // Inserare in Minimum_req Table
        insertInfo = insertValues_Minimum_Req();
        sqlQuery = "INSERT INTO Minimum_Req(AppID,GHzCPU, GPU, GB_RAM, GB_HDStorage) VALUES(" + appID + "," +insertInfo + ");";

        cout << sqlQuery << endl;
        insertQuery(db, sqlQuery);

    }
    else
    if(inputCommand == "Search")          // cautare in db
    {
        /*
        count_arg = 0;
        cout << "Introduceti date pentru cautare:\n";
        cout << "nume: "; getline(cin,cinBuffer);
        if(cinBuffer.empty() == 0)
        {
            searchInfo = searchInfo + "name = \"" + cinBuffer + "\"";
            count_arg++;
        }

        cinBuffer.clear();
        cout << "manufacturer: ";
        getline(cin,cinBuffer);
        if(cinBuffer.empty() == 0)
        {
            if(count_arg != 0)
            {
                searchInfo.append(" AND ");
            }
            searchInfo = searchInfo + "manufacturer = \"" + cinBuffer + "\"";
            count_arg++;
        }

        // to do : sa adaug fiecare Cerere de inf

        cinBuffer.clear();

        sqlQuery.clear();
        if(searchInfo.empty() == 0 )
            sqlQuery = "SELECT * FROM AppRepository WHERE " + searchInfo + " ;";
        else
            sqlQuery = "SELECT * FROM AppRepository;";

        selectQuery( sqlQuery , db);
        */
    }


    sqlite3_close(db);
    return 0;
}
