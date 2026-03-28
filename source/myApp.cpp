#include "myApp.h"
#include "mainFrame.h"
#include <wx/wx.h>
#include "database.h"

//defining onInit method, it is the entry point of my application
using namespace std;
bool myApp::OnInit(){
    system("clear");
    myDB = new database(); //create a new database object
    // 1. Launch the UI Login Prompt
    wxTextEntryDialog loginDlg(nullptr, "Enter username:", "wxPomodoro Login");

    // 2. Wait for user input
    if (loginDlg.ShowModal() == wxID_OK) {
        user = loginDlg.GetValue().ToStdString();

        if (user.empty()) return false; // Quit if they entered nothing

        if(!myDB->userExists(user)) {
            myDB->addUser(user);
        }
    } else {
        // 3. User clicked Cancel or the X button
        return false;
    }

    mainFrame* MainFrame = new mainFrame("wxPomodoro");//New instance of mainframe with title argument
    MainFrame->Show(true);//calling show method of the object as it is hidden by default
    MainFrame->SetClientSize(395,350);
    return true; //returning true tells wxwidgets that the program can resume
}
//have to tell wxwidgets which class represents my application, so I call this following macro/method
wxIMPLEMENT_APP(myApp);

