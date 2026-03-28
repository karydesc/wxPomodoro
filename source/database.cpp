#include "database.h"
#include <iostream>
#include <sqlite3.h>
#include <string>
#include <sstream>
#include "myApp.h"
#include <fstream>
using namespace std;


wxDECLARE_APP(myApp);

database::database() { //constructor that is called in the onInit method
    system("mkdir ./data");
    sqlite3_open("./data/file.db", &db);
    string query = "CREATE TABLE IF NOT EXISTS database(user TEXT, workmins INT, sessionsCompleted INT);"; //create a table if it doesnt exist with 4 columns: user,pass,workmins,sessionscompleted
    sqlite3_exec(db, query.c_str(), NULL, NULL, &error); //execute the query
    cout<<"Connected to database successfully"<<endl;
}


bool database::addUser(const string& user) {
    stringstream query; //create a stringstream in which I will build the query
    query << "SELECT * FROM database WHERE user = '" << user << "';"; //grab the row in which the user is matched, to see if user already exists
    rc = sqlite3_prepare_v2(db, query.str().c_str(), -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        cout << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return false;
    }
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        cout << "User already exists!...";
        return false;

    }
    //query with placeholders for the parameters, i would change it to use stringstream
    const char* insertDataQuery = "INSERT INTO database(user, workmins, sessionsCompleted) VALUES (?, ?, ?);";

    rc = sqlite3_prepare_v2(db, insertDataQuery, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        cout << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return false;
    }

    sqlite3_bind_text(stmt, 1, user.c_str(), -1, SQLITE_STATIC); //bind values into the placeholders, I use stringstreams later

    rc = sqlite3_step(stmt); //execute the command

    if (rc != SQLITE_DONE) { //error handling
        cout << "Error executing statement: " << sqlite3_errmsg(db) << endl;
    }else cout<<"User inserted successfully"<<endl;

    sqlite3_finalize(stmt); //finalize stmt
    return true;
}

bool database::userExists(const string & user) {
    stringstream query;
    query << "SELECT * FROM database WHERE user = '" << user << "';";
    rc = sqlite3_prepare_v2(db, query.str().c_str(), -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        cout << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return false;
    }
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        return true;
    }
    return false;
}


void database::storeStats(const std::string& user,pomodoro* session) {
    stringstream query; //create a query variable
    query << "SELECT workmins, sessionsCompleted FROM database WHERE user= '" << user << "';"; //build the query
    rc = sqlite3_prepare_v2(db, query.str().c_str(), -1, &stmt, 0); //prepare statement

    if (rc != SQLITE_OK) { //handle errors
        cout << "Error preparing SELECT statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    rc = sqlite3_step(stmt);//execute statement
    int prevWM=0; //initialize variables here, so they don't go out of scope inside the if statement
    int prevSC=0;
    if (rc == SQLITE_ROW) {
         prevWM = sqlite3_column_int(stmt, 0); //extract previous values from database
         prevSC = sqlite3_column_int(stmt, 1);


        sqlite3_finalize(stmt); // Reset the statement for reuse
        query.str("");       // Clear the stringstream

        query << "UPDATE database SET workmins = ?, sessionsCompleted = ? WHERE user= '" << user << "';";
        rc = sqlite3_prepare_v2(db, query.str().c_str(), -1, &stmt, 0); //executing stmt

        if (rc != SQLITE_OK) { //error handling
            cout << "Error preparing UPDATE statement: " << sqlite3_errmsg(db) << endl;
            sqlite3_close(db);
            return;
        }

        sqlite3_bind_int(stmt, 1, prevWM + (session->WorkSeconds/60 - session-> lastCurrentSessionMinutes)); //bind for execution
        sqlite3_bind_int(stmt, 2, prevSC + (session->sessionsCompleted - session-> lastCurrentSessionCount));//I remove from the result the time and session count of the
        session-> lastCurrentSessionMinutes=session->WorkSeconds / 60 ;// already existing instance to prevent for example: pressing log multiple times
        session->lastCurrentSessionCount=session->sessionsCompleted;//and adding the same minutes worked over and over again
        rc = sqlite3_step(stmt); //exectute statement

        if (rc != SQLITE_DONE) { //handle error
            cout << "Error executing UPDATE statement: " << sqlite3_errmsg(db) << endl;
            sqlite3_close(db);
            return;
        }
    }

    sqlite3_reset(stmt); // Finalize the statement
}

UserStats database::getStats(const std::string& user){
    UserStats stats;
    stringstream query;
    query << "SELECT workmins, sessionsCompleted FROM database WHERE user = '" << user << "';";

    rc = sqlite3_prepare_v2(db, query.str().c_str(), -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        std::cout << "Error preparing SELECT statement: " << sqlite3_errmsg(db) << std::endl;
        return stats; // Returns {0, 0} on error
    }

    rc = sqlite3_step(stmt);

    if (rc == SQLITE_ROW) {
        // Populate the struct with the database values
        stats.workMins = sqlite3_column_int(stmt, 0);
        stats.sessionsCompleted = sqlite3_column_int(stmt, 1);
    }

    sqlite3_reset(stmt);
    return stats;
}

void database::close() const {
    sqlite3_finalize(stmt); //close db and finalize stmt
    sqlite3_close(db);
}


