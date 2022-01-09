#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

#include <sqlite3.h>
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

#define maxi 100000 // maximum length for data array in callback function
#define MAXIMUM 1024


static int callback(void* data, int argc, char** argv, char** azColName);
static int callback_SEARCH(void* data, int argc, char** argv, char** azColName);
string selectQuery(sqlite3* db, string sqlQuery);
string selectQuery_SEARCH(sqlite3* db, string sqlQuery);
string insertQuery(sqlite3* db, string sqlQuery);
string insertValues_Application();
string insertValues_Minimum_Req();
string getAppID(sqlite3* db, string appName);
string searchApps(sqlite3* db);
int numberOfAppsFound(sqlite3* db, string sqlQuery);