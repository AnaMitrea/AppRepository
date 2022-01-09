#include "database.h"

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
        return "OK at inserting query";
    }
}

string insertValues_Application()
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
    insertInfo = insertInfo + "\""+ cinBuffer + "\"";

    // DEVELOPER
    cinBuffer.clear(); //reset string
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

string searchApps(sqlite3* db)
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

int numberOfAppsFound(sqlite3* db, string sqlQuery)
{
    string sqlResponse; // sql query response
    sqlResponse = selectQuery_SEARCH(db, sqlQuery);

    string nr = sqlResponse.substr(25,string::npos);
    int num = stoi(nr);

    return num;
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
    string searchInfo; // holds information about sql query for searching db with criteria
    string insertInfo; // holds information about sql query for inserting in db
    string sqlQuery; // sql query to be executed
    string sqlResponse; // sql query response

    cout << "[1] To insert your Application, write \"Insert\". \n";
    cout << "[2] To search an Application, write \"Search\". \n";
    cout << "Your command: ";

    string inputCommand;
    getline(cin, inputCommand);
    
    if(inputCommand == "Insert") // Insert app
    {
        insertInfo = insertValues_Application();
        sqlQuery.clear();

        sqlQuery = "INSERT INTO Application(AppName, Developer, Executable_name, License, Category, InternetConnection, AppInfo) VALUES(" + insertInfo + ");";
        sqlResponse = insertQuery(db, sqlQuery);
        cout << sqlResponse << endl;
        sqlResponse.clear();
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
        sqlResponse = insertQuery(db, sqlQuery);
        cout << sqlResponse << endl;
        sqlResponse.clear();
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
            sqlResponse = insertQuery(db, sqlQuery);
            cout << sqlResponse << endl;
            sqlResponse.clear();

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

        sqlResponse = insertQuery(db, sqlQuery);
        cout << sqlResponse << endl;
        sqlResponse.clear();

    }
    else
    if(inputCommand == "Search")          // cautare in db
    {
        sqlQuery.clear();
        searchInfo.clear();
        cinBuffer.clear();

        cout << "\nCriteria available:\n";
        cout << "- App's Name\n";
        cout << "- Developer\n";
        cout << "- License\n";
        cout << "- Category\n";
        cout << "- Internet Connection\n";
        cout << "- Operating System\n";
        cout << "- CPU\n";
        cout << "- RAM\n";
        cout << "- Hard Disk Storage\n\n";

        searchInfo = searchApps(db);
 
        sqlQuery = "SELECT COUNT(DISTINCT AppName) FROM Application LEFT JOIN OS USING(AppID) LEFT JOIN Minimum_Req USING(AppID) WHERE " + searchInfo + ";";

        int appsFound = numberOfAppsFound(db, sqlQuery);
        cout << "Found "<< appsFound << " programs for the criteria. \n\n";
        sqlQuery.clear();

        sqlQuery = "SELECT * FROM Application LEFT JOIN OS USING(AppID) LEFT JOIN Minimum_Req USING(AppID) WHERE " + searchInfo + ";";

        sqlResponse = selectQuery_SEARCH(db, sqlQuery);
        cout << sqlResponse << endl;
        sqlQuery.clear();

 
    }
    sqlite3_close(db);
    return 0;
}
