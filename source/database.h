#pragma once
#include <string>
#include <sqlite3.h>
#include "pomodoro.h"
using namespace std;
class database {
public:
    bool addUser(const string &);

    bool userExists(const string &);

    database();
    void close() const;
    void storeStats(const string&,pomodoro*);
    UserStats getStats(const std::string& user);
private:
    sqlite3* db;
    int rc;
    char* error;
    sqlite3_stmt* stmt;
};
