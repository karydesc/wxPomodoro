#pragma once
#include <wx/wx.h>
#include <thread>
struct UserStats {
    int workMins = 0;
    int sessionsCompleted = 0;
};
class pomodoro {
private:
    bool pauseflag=false;
    int lastCurrentSessionMinutes=0;
    int lastCurrentSessionCount=0;
    int WorkSeconds;
    int sessionsCompleted=0;
public:
    bool processing=false;
    bool quitRequested=false;
    std::thread backgroundThread;
    bool cancelFlag=false;
    void startSession(int workmins,int breakmins,wxStaticText*,wxGauge*, wxStaticText *);
    void pauseSession();
    static void resetScreen(wxStaticText *text, wxGauge *gauge);

    friend class database;
};