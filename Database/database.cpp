#include "database.h"

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


        cout << "If you want to add more distributions, write ADD. If you don't want to, write STOP.\nYour command: ";
        cinBuffer.clear();
        getline(cin, cinBuffer);

        while(cinBuffer != "STOP" && cinBuffer != "ADD")
        {
            cout << "If you want to add more distributions, write ADD. If you don't want to, write STOP.\nYour command: ";
            cinBuffer.clear();
            getline(cin, cinBuffer);
        }
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

        insertInfo = insertValues_Minimum_Req();
        sqlQuery = "INSERT INTO Minimum_Req(AppID,GHzCPU, GPU, GB_RAM, GB_HDStorage) VALUES(" + appID + "," + insertInfo + ");";

        sqlResponse = insertQuery(db, sqlQuery);
        cout << sqlResponse << endl;
        sqlResponse.clear();

    }
    else
    if(inputCommand == "Search")
    {
        sqlQuery.clear();
        searchInfo.clear();
        cinBuffer.clear();

        // toate astea trebuie intr un singur mesaj si trimise catre client.
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

        searchInfo = searchApps();
        sqlQuery = "SELECT COUNT(DISTINCT AppName) FROM Application LEFT JOIN OS USING(AppID) LEFT JOIN Minimum_Req USING(AppID) WHERE " + searchInfo + ";";

        string appsFound = numberOfAppsFound(db, sqlQuery);
        cout << "Found "<< appsFound << " programs for the criteria. \n\n";

        sqlQuery.clear();

        sqlQuery = "SELECT * FROM Application LEFT JOIN OS USING(AppID) LEFT JOIN Minimum_Req USING(AppID) WHERE " + searchInfo + ";";

        sqlResponse = selectQuery_SEARCH(db, sqlQuery);
        cout << sqlResponse << endl;


        sqlQuery.clear();
        sqlResponse.clear();
    }
    sqlite3_close(db);
    return 0;
}
