#pragma once

#include <sqlite3.h>
#include <iostream>
#include <string>
#include <string.h>
#include <algorithm>
using namespace std;

#define maxi 100000 // maximum length for data array in callback function
#define MAXIMUM 1024


static int callback(void* data, int argc, char** argv, char** azColName);
static int callback_SEARCH(void* data, int argc, char** argv, char** azColName);
string selectQuery(sqlite3* db, string sqlQuery);
string selectQuery_SEARCH(sqlite3* db, string sqlQuery);
string insertQuery(sqlite3* db, string sqlQuery);
bool verifyingExistingName(sqlite3* db, string appName);
string insertValues_Application(sqlite3* db);
string insertValues_Minimum_Req();
string getAppID(sqlite3* db, string appName);
string searchApps();
string numberOfAppsFound(sqlite3* db, string sqlQuery);

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
        strcat(information, " = ");
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

static int callback_SEARCH(void* data, int argc, char** argv, char** azColName) //calback function used in sqlite
{
    // information from sql query to be displayed as:
    // atribute1 = value1
    // atribute2 = value2
    // ...

    char* information = (char*) data;
    for (int i = 0; i < argc; i++) 
    {
        if(strcmp(azColName[i], "AppID") != 0)
        {
            strcat(information, azColName[i]);
            strcat(information, " = ");
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
    }
    strcat(information, "\n");
    return 0;
}

string selectQuery(sqlite3* db, string sqlQuery)
{
    char* SQL_errorMessage;
    char data[maxi]; // callback argument
    string sqlQueryResult;
    sqlQueryResult.clear();
    data[0] = 0;

    int sqlExec = sqlite3_exec(db, sqlQuery.c_str(), callback, data, &SQL_errorMessage);
    if (sqlExec != SQLITE_OK)
    {
        sqlQueryResult = SQL_errorMessage;
        return sqlQueryResult;
        sqlite3_free (SQL_errorMessage);
    }
    else
    {
        data[strlen(data) - 1] = 0;
        sqlQueryResult = data;
        return sqlQueryResult;
    }
}

string selectQuery_SEARCH(sqlite3* db, string sqlQuery)
{
    char* SQL_errorMessage;
    char data[maxi]; // callback argument
    string sqlQueryResult;
    sqlQueryResult.clear();
    data[0] = 0;

    int sqlExec = sqlite3_exec(db, sqlQuery.c_str(), callback_SEARCH, data, &SQL_errorMessage);
    if (sqlExec != SQLITE_OK)
    {
        sqlQueryResult = SQL_errorMessage;
        return sqlQueryResult;
        sqlite3_free (SQL_errorMessage);
    }
    else
    {
        data[strlen(data) - 1] = 0;
        sqlQueryResult = data;
        return sqlQueryResult;
    }
}

string insertQuery(sqlite3* db, string sqlQuery)
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
        sqlite3_free (SQL_errorMessage);
        return sqlQueryResult;
    }
    else
    {
        return "\n[Inserting Query succeeded]\n";
    }
}

bool verifyingExistingName(sqlite3* db, string appName) // searching existing AppName in db (returns true for already existing)
{
    string sqlQuery = "SELECT AppName FROM Application WHERE AppName=\"" + appName + "\";";
    string sqlResponse; // sql query response for search existing appName in database
    sqlResponse = selectQuery(db, sqlQuery);

    if(sqlResponse.empty())
        return false;
    
    string name = sqlResponse.substr(10,string::npos);
    name = name.substr(0, name.length()-1);

    if(name == appName)
        return true;
    return false;
}

string insertValues_Application(sqlite3* db)
{
    string cinBuffer; // reading from stdin
    string insertInfo; // holds information about sql query for inserting in db

    
    cout << "Complete the following information about your application.\n";
    cout << "Project Name: "; 
    getline(cin, cinBuffer);
    
    while(cinBuffer.empty() == 1) 
    {
        cout << "Project Name field must be completed!" << endl;
        cout << "Project Name: "; 
        getline(cin, cinBuffer);
    }

    string name = cinBuffer;
    while(verifyingExistingName(db, name) == true)
    {
        name.clear();
        cout << "Project name already exists. Choose another name." << endl;
        cout << "Project Name: ";
        getline(cin, name);
    }

    insertInfo = insertInfo + "\""+ name + "\"";
    name.clear();

    // DEVELOPER
    cinBuffer.clear();
    cout << "Developer: "; 
    getline(cin, cinBuffer);

    while(cinBuffer.empty() == 1)
    {
        cout << "Developer's Name field must be completed!" << endl;
        cout << "Developer: ";
        getline(cin, cinBuffer);
    }
    insertInfo = insertInfo + ",\""+ cinBuffer + "\"";

    // EXECUTABLE NAME
    cinBuffer.clear(); //reset string
    cout << "Executable: "; 
    getline(cin, cinBuffer);

    while(cinBuffer.empty() == 1)
    {
        cout << "Executable's Name field must be completed!" << endl;
        cout << "Executable: ";
        getline(cin, cinBuffer);
    }
    insertInfo = insertInfo + ",\""+ cinBuffer + "\"";

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
    string insertInfo; // holds information about sql query for inserting in db

    int ok = 0;

    cout << "Minimum requirements for the application:\n";
    cout << "CPU (GHz): "; 
    getline(cin, cinBuffer);

    if(cinBuffer.empty() == 1)
    {
        insertInfo = insertInfo + "-1";
    }
    else
    {
        insertInfo = insertInfo + cinBuffer;
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
            insertInfo = insertInfo + "-1";
        }
        else
        {
            insertInfo = insertInfo + cinBuffer;
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
            insertInfo = insertInfo + ",-1";
        }
        else
        {
            insertInfo = insertInfo + ","+ cinBuffer;
        }
    }
    else
    {
        if(cinBuffer.empty() == 1)
        {
            insertInfo = insertInfo + "-1";
        }
        else
        {
            insertInfo = insertInfo + cinBuffer;
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
            insertInfo = insertInfo + ",-1";
        }
        else
        {
            insertInfo = insertInfo + ","+ cinBuffer;
        }
    }
    else
    {
        if(cinBuffer.empty() == 1)
        {
            insertInfo = insertInfo + "-1";
        }
        else
        {
            insertInfo = insertInfo+ cinBuffer;
        }
    }

    return insertInfo;
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

string searchApps()
{
    string cinBuffer;
    string searchInfo; // holds information about sql query for searching db with criteria
    cinBuffer.clear();
    searchInfo.clear();
    int ok = 0;

    cout << "App's name: ";
    getline(cin, cinBuffer);

    if(cinBuffer.empty() == 0)
    {
        searchInfo = searchInfo + "AppName=\""+ cinBuffer + "\" ";
        ok = 1;
    }
    cinBuffer.clear();


    cout << "Developer's name: ";
    getline(cin, cinBuffer);

    if(cinBuffer.empty() == 0)
    {
        if(ok == 1)
        {
            searchInfo = searchInfo + "AND Developer=\""+ cinBuffer + "\" ";
        }
        else
        {
            searchInfo = searchInfo + "Developer=\""+ cinBuffer + "\" ";
            ok = 1;
        }
    }
    cinBuffer.clear();

    cout << "License: ";
    getline(cin, cinBuffer);

    if(cinBuffer.empty() == 0)
    {
        if(ok == 1)
        {
            searchInfo = searchInfo + "AND (License=\""+ cinBuffer + "\" ";
        }
        else
        {
            searchInfo = searchInfo + "(License=\""+ cinBuffer + "\" ";
            ok = 1;
        }
        searchInfo = searchInfo + "OR License=\"-\") ";
    }
    cinBuffer.clear();

    cout << "Category: ";
    getline(cin, cinBuffer);

    if(cinBuffer.empty() == 0)
    {
        if(ok == 1)
        {
            searchInfo = searchInfo + "AND (Category=\""+ cinBuffer + "\" ";
        }
        else
        {
            searchInfo = searchInfo + "(Category=\""+ cinBuffer + "\" ";
            ok = 1;
        }
        searchInfo = searchInfo + "OR Category=\"-\") ";
    }
    cinBuffer.clear();

    cout << "Internet Connection: ";
    getline(cin, cinBuffer);

    if(cinBuffer.empty() == 0)
    {
        if(ok == 1)
        {
            searchInfo = searchInfo + "AND (InternetConnection=\""+ cinBuffer + "\" ";
        }
        else
        {
            searchInfo = searchInfo + "(InternetConnection=\""+ cinBuffer + "\" ";
            ok = 1;
        }
        searchInfo = searchInfo + "OR InternetConnection=\"-\") ";
    }
    cinBuffer.clear();

    cout << "Operating System: ";
    getline(cin, cinBuffer);

    if(cinBuffer.empty() == 0)
    {
        if(ok == 1)
        {
            searchInfo = searchInfo + "AND (OS_Name=\""+ cinBuffer + "\" ";
        }
        else
        {
            searchInfo = searchInfo + "(OS_Name=\""+ cinBuffer + "\" ";
            ok = 1;
        }
        searchInfo = searchInfo + "OR OS_Name=\"-\") ";
    }
    cinBuffer.clear();

    cout << "Minimum CPU: ";
    getline(cin, cinBuffer);

    if(cinBuffer.empty() == 0)
    {
        if(ok == 1)
        {
            searchInfo = searchInfo + "AND (GHzCPU>="+ cinBuffer;
        }
        else
        {
            searchInfo = searchInfo + "(GHzCPU>="+ cinBuffer;
            ok = 1;
        }
        searchInfo = searchInfo + " OR GHzCPU=\"-1\") ";
    }

    cout << "Minimum RAM: ";
    getline(cin, cinBuffer);

    if(cinBuffer.empty() == 0)
    {
        if(ok == 1)
        {
            searchInfo = searchInfo + "AND (GB_RAM>="+ cinBuffer;
        }
        else
        {
            searchInfo = searchInfo + "(GB_RAM>="+ cinBuffer;
            ok = 1;
        }
        searchInfo = searchInfo + " OR GB_RAM=\"-1\") ";
    }
    cinBuffer.clear();

    cout << "Minimum Hard Disk Storage: ";
    getline(cin, cinBuffer);

    if(cinBuffer.empty() == 0)
    {
        if(ok == 1)
        {
            searchInfo = searchInfo + "AND (GB_HDStorage>="+ cinBuffer;
        }
        else
        {
            searchInfo = searchInfo + "(GB_HDStorage>="+ cinBuffer;
            ok = 1;
        }
        searchInfo = searchInfo + " OR GB_HDStorage=\"-1\")";
    }
    cinBuffer.clear();

    cout << endl;

    return searchInfo;

}

string numberOfAppsFound(sqlite3* db, string sqlQuery)
{
    string sqlResponse; // sql query response
    sqlResponse = selectQuery_SEARCH(db, sqlQuery);

    string nr = sqlResponse.substr(26,string::npos);
    nr = nr.substr(0,nr.length() - 1);
    return nr;
}

