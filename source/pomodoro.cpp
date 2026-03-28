#include "pomodoro.h"
#include <iostream>
#include <wx/wx.h>
#include <wx/string.h>
#include <chrono>
#include "myApp.h"
#include <wx/sound.h>

using namespace std::chrono_literals;
using namespace std;
wxDECLARE_APP(myApp);

void pomodoro::startSession(int workminutes,int breakminutes,wxStaticText* text,wxGauge* gauge, wxStaticText *statsLabel) {
    this->processing =true;
    pauseflag=false;
    cancelFlag=false;
    int userWorkedMinutes=0;
//restart pomodoro indefinitely
    while(true){
        int secs = workminutes * 60;
        wxGetApp().CallAfter([gauge,secs](){ //set the range of the gauge
            gauge->SetRange(secs);
        });
    for (int i = secs; i >= 0;) {
        if (this->cancelFlag||quitRequested){
            this->processing=false;
            return;
        }       //handling window close/end thread

        if (!this->pauseflag) {
            this->WorkSeconds++;
            wxGetApp().CallAfter([i, text, gauge]() {
                text->SetLabelText(wxString::Format("Focus: %d:%02d", i / 60, i % 60)); //updating the timer
                gauge->SetValue(i);
            });
            i--;
        }
        std::this_thread::sleep_for(1s); //timer tick
        if (((secs-i)%60)==0) { //because i is iterating backwards from 'secs'
            userWorkedMinutes++;
            wxGetApp().GetDatabase()->storeStats(wxGetApp().getUser(), this); //for every minute store user session stats
            UserStats currentStats = wxGetApp().GetDatabase()->getStats(wxGetApp().getUser());
            wxGetApp().CallAfter([statsLabel,currentStats]() {
                statsLabel->SetLabelText(wxString::Format("Sessions: %d | Mins: %d",
                currentStats.sessionsCompleted, currentStats.workMins));
            });
        }
    }
    wxSound::Play("../resources/breaktime.mp3");

    secs = breakminutes * 60; //repeat for break session
    wxGetApp().CallAfter([gauge,secs](){ //set the range of the gauge
            gauge->SetRange(secs);
    });
    for (int i = secs; i >= 0;) {
        if (this->cancelFlag||quitRequested){
            this->processing=false;
            return;
        }

        if(!this->pauseflag){
            wxGetApp().CallAfter([i, text, gauge]() {
                text->SetLabelText(wxString::Format("Break: %d:%02d", i / 60, i % 60));
                gauge->SetValue(i);
            });
            i--;

        }
        std::this_thread::sleep_for(1s);

        if (((secs-i)%60)==0) {
            userWorkedMinutes++;
            wxGetApp().GetDatabase()->storeStats(wxGetApp().getUser(), this); //for every minute store user session stats
            UserStats currentStats = wxGetApp().GetDatabase()->getStats(wxGetApp().getUser());
            wxGetApp().CallAfter([statsLabel,currentStats]() {
                statsLabel->SetLabelText(wxString::Format("Sessions: %d | Mins: %d",
                currentStats.sessionsCompleted, currentStats.workMins));
            });
        }
    }
    this->sessionsCompleted++;
    wxGetApp().GetDatabase()->storeStats(wxGetApp().getUser(), this); //for every minute store user session stats
    UserStats currentStats = wxGetApp().GetDatabase()->getStats(wxGetApp().getUser());
        wxGetApp().CallAfter([statsLabel,currentStats]() {
            statsLabel->SetLabelText(wxString::Format("Sessions: %d | Mins: %d",
            currentStats.sessionsCompleted, currentStats.workMins));
        });

    wxSound::Play("../resources/sessioncomplete.mp3");
    system("clear");
    cout<<"Starting over in 5s...";
    this_thread::sleep_for(5s);
    system("clear");
    wxSound::Play("../resources/sessionstart.mp3");

}
}

void pomodoro::pauseSession() {
    this->pauseflag= !pauseflag;
}
void pomodoro::resetScreen(wxStaticText* text, wxGauge* gauge){
    wxGetApp().CallAfter([text,gauge]() {
        text->SetLabelText("Stopped");
        gauge->SetValue(0);
    });
}

