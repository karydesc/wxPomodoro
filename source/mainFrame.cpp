#include "mainFrame.h"
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include "pomodoro.h"
#include <wx/string.h>
#include "myApp.h"
#include <wx/sound.h>

DECLARE_APP(myApp)
using namespace std;

enum IDs{
    startButtonID=2,
    pauseButtonID=3,
    cancelButtonID=5,

};
wxBEGIN_EVENT_TABLE(mainFrame,wxFrame)
                EVT_BUTTON(startButtonID,mainFrame::onStartButtonClick)
                EVT_BUTTON(pauseButtonID,mainFrame::onPauseButtonClick)
                EVT_BUTTON(cancelButtonID,mainFrame::onCancelButtonClick)

wxEND_EVENT_TABLE();


//constructor implementation
mainFrame::mainFrame(const wxString& title) : wxFrame(nullptr,wxID_ANY,title) { //calling the constructor of the base class

    panel = new wxPanel(this); //panel is needed to contain controls


    startButton = new wxButton(panel, startButtonID, "Start", wxPoint(35, 255), wxSize(100, 35));

    pauseButton = new wxButton(panel, pauseButtonID, "Pause/Resume", wxPoint(150, 270), wxSize(100, 35));

    gauge = new wxGauge(panel, wxID_ANY, 100, wxPoint(210, 10), wxSize(150, -1));

    timeselect = new wxSpinCtrl(panel, wxID_ANY, "", wxPoint(265, 262),wxSize(100, 20)); //this will be used to select times for pomodoro and break

    breakselect = new wxSpinCtrl(panel, wxID_ANY, "", wxPoint(265, 290), wxSize(100, 20));

    timeselect->SetValue(25); //Setting the default values as per Project instructions.

    breakselect->SetValue(5);

    header = new wxStaticText(panel, wxID_ANY, "Pomodoro", wxPoint(50, 10),wxSize(200, 30)); //creating simple header

    timer = new wxStaticText(panel, wxID_ANY, "Stopped", wxPoint(105, 140), wxSize(200,30)); //Creating a text field were the running time will be displayed
    const wxFont font = timer->GetFont();
    timer->SetFont(font.Scaled(3));

    cancelButton = new wxButton(panel,cancelButtonID,"Cancel", wxPoint(35, 285), wxSize(100, 35));
    this->Bind(wxEVT_CLOSE_WINDOW, &mainFrame::OnClose, this);

    session = new pomodoro;

    std::string currentUser = wxGetApp().getUser();

    UserStats currentStats = wxGetApp().GetDatabase()->getStats(currentUser);

    statsLabel = new wxStaticText(panel, wxID_ANY, wxString::Format("Sessions: %d | Mins: %d",
    currentStats.sessionsCompleted, currentStats.workMins), wxPoint(105, 200), wxSize(200, 30), wxALIGN_CENTER_HORIZONTAL);


}

void mainFrame::onStartButtonClick(wxCommandEvent &evt) {
    if (breakselect->GetValue()==0 || timeselect->GetValue()==0) return; //if either spincontrol is 0 the timer wont start
    if (!session->processing) { //only run if previous thread has finished work
        wxSound::Play("../resources/sessionstart.mp3");
        if (session->backgroundThread.joinable())//join thread with main thread
       {
            session->backgroundThread.join();
        }

        const auto f = [this]() {
            session->startSession(timeselect->GetValue(), breakselect->GetValue(), timer, gauge, statsLabel);
        };
        session->backgroundThread = std::thread{f};
    }
}


void mainFrame::onPauseButtonClick(wxCommandEvent &evt) {
    session->pauseSession();
}


void mainFrame::onCancelButtonClick(wxCommandEvent &evt) {
    wxSound::Play("../resources/Cancel.wav"); //play sound
    if (session->processing) {
        session->cancelFlag = true; //set flags
    }
    session->resetScreen(this->timer, this->gauge);
}

void mainFrame::OnClose(wxCloseEvent& e){
    session->quitRequested = true; // Tell the loop to stop

    // Wait for the thread to safely exit its loop
    if (session->backgroundThread.joinable()) {
        session->backgroundThread.join();
    }

    // Now it's safe to delete things
    delete wxGetApp().GetDatabase();
    delete session;
    this->Destroy();
}



