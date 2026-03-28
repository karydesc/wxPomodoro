#pragma once
#include <wx/wx.h>
#include "database.h"
#include <string>
//myApp class needed to represent the application itself
class myApp : public wxApp{ //inheriting from wxApp class
    public:
    bool OnInit() override;
        database* GetDatabase() const { return myDB; }
        string getUser() const {return user;}

    private:
        string user;
        database* myDB;
};
