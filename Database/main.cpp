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

void insertQuery(string sqlQuery, sqlite3* db)
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
    searchInfo = searchInfo + "AppName = \"" + cinBuffer + "\"";

    // DEVELOPER
    cinBuffer.clear(); //reset string
    cout << "Developer: "; 
    getline(cin, cinBuffer);

    if(cinBuffer.empty() == 1)
    {
        insertInfo = insertInfo + ",\"-\"";
        searchInfo = searchInfo + " AND Developer = \"-\"";
    }
    else
    {
        insertInfo = insertInfo + ",\""+ cinBuffer + "\"";
        searchInfo = searchInfo + " AND Developer = \"" + cinBuffer + "\"";
    }

    // EXECUTABLE NAME
    cinBuffer.clear(); //reset string
    cout << "Executable: "; 
    getline(cin, cinBuffer);

    if(cinBuffer.empty() == 1)
    {
        insertInfo = insertInfo + ",\"-\"";
        searchInfo = searchInfo + " AND Executable_Name = \"-\"";
    }
    else
    {
        insertInfo = insertInfo + ",\""+ cinBuffer + "\"";
        searchInfo = searchInfo + " AND Executable_Name = \"" + cinBuffer + "\"";
    }

    // LICENSE
    cinBuffer.clear(); //reset string
    cout << "License: "; 
    getline(cin, cinBuffer);

    if(cinBuffer.empty() == 1)
    {
        insertInfo = insertInfo + ",\"-\"";
        searchInfo = searchInfo + " AND License = \"-\"";
    }
    else
    {
        insertInfo = insertInfo + ",\""+ cinBuffer + "\"";
        searchInfo = searchInfo + " AND License = \"" + cinBuffer + "\"";
    }

    // CATEGORY
    cinBuffer.clear(); //reset string
    cout << "App category: "; 
    getline(cin, cinBuffer);

    if(cinBuffer.empty() == 1)
    {
        insertInfo = insertInfo + ",\"-\"";
        searchInfo = searchInfo + " AND Category = \"-\"";
    }
    else
    {
        insertInfo = insertInfo + ",\""+ cinBuffer + "\"";
        searchInfo = searchInfo + " AND Category = \"" + cinBuffer + "\"";
    }

    // INTERNET CONNECTION (YES/NO)
    cinBuffer.clear(); //reset string
    cout << "Internet Connection (YES/NO): "; 
    getline(cin, cinBuffer);

    if(cinBuffer.empty() == 1)
    {
        insertInfo = insertInfo + ",\"-\"";
        searchInfo = searchInfo + " AND InternetConnection = \"-\"";
    }
    else
    {
        insertInfo = insertInfo + ",\""+ cinBuffer + "\"";
        searchInfo = searchInfo + " AND InternetConnection = \"" + cinBuffer + "\"";
    }

    // DESCRIPTION
    cinBuffer.clear(); //reset string
    cout << "Description: "; 
    getline(cin, cinBuffer);

    if(cinBuffer.empty() == 1)
    {
        insertInfo = insertInfo + ",\"-\"";
        searchInfo = searchInfo + " AND AppInfo= \"-\"";
    }
    else
    {
        insertInfo = insertInfo + ",\""+ cinBuffer + "\"";
        searchInfo = searchInfo + " AND AppInfo = \"" + cinBuffer + "\"";
    }

    cout << "\ninsertInfo...\n"<< insertInfo << endl << "searchInfo...\n" << searchInfo << endl;

    return insertInfo;
}

void selectQuery(string select_data, sqlite3* db)
{
    char* SQL_errorMessage;
    char data[maxi]; // callback argument
    string result;

    data[0] = 0;
    result.clear();

    int rc = sqlite3_exec(db, select_data.c_str(), callback, data, &SQL_errorMessage);
    if (rc != SQLITE_OK)
    {
        result = SQL_errorMessage;
        cout << result;
        sqlite3_free (SQL_errorMessage);
    }
    else
    {
        data[strlen(data)-1]=0;
        result = data;
        cout << endl << result;
    }
}

int return_id_app( string conditie_select , sqlite3* db) 
{
    char* SQL_errorMessage; // mesaju de eroare
    char data[maxi]; // callback argument
    string result;

    data[0] = 0;
    result.clear();

    int rc = sqlite3_exec(db, conditie_select.c_str(), callback, data, &SQL_errorMessage);
    if (rc != SQLITE_OK)
    {
        result = SQL_errorMessage;
        cout << result;
        sqlite3_free (SQL_errorMessage);
        return 0;
    }
    else
    {
        data[strlen(data)-1]='\0';
        result = data;
    }

    int poz = result.find('=');
    string id_app = result.substr(poz+1,result.length()-poz);
    return stoi(id_app);
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
    int count_arg = 0;

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
        cout << sqlQuery << endl;

        insertQuery(sqlQuery, db);

        sqlQuery.clear();


        // to do - cere inf despre celelalte 2 tabele si adauga 2 insert
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
