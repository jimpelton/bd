


#include "RtAudioFeeder.h"

//#include <QApplication>

#include <iostream>
#include <iomanip>
#include <thread>
#include <atomic>
#include <chrono>

#include <unistd.h>
#include <termios.h>
#include <stdio.h>



using namespace libsch;
using namespace std;


atomic<int> keypress(-1);
double totalTime;

void printStatus(const RtAudioFeeder *rifraf)
{
    chrono::milliseconds snooze(100); 
    cout << setw(20) << endl;
    while (keypress.load() != 1) {
        double time = rifraf->StreamTime();
        if (time >= totalTime) 
            return;
        double perc = (time / totalTime)*100.0;
        cout << '\r';
        cout << fixed;
        cout << setprecision(2) << time; 
        cout <<   " (" << setprecision(0) << perc << "%)" << flush;
        this_thread::sleep_for(snooze);
    }
}


char getch() 
{
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror ("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror ("tcsetattr ~ICANON");
    return (buf);
}

void usage() 
{
    std::cout << "Usage: rtaudiotest <file-name>\n" << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc < 1) {
        usage();
        exit(0);
    }

    //QApplication app(argc, argv);

    string fname(argv[1]);
    RtAudioFeeder raf(fname);
    //SCH_RESULT err = raf.Init();
//    if (err != SCH_OK) {
//        cout << "Could not init raf or something." << endl;
//        exit(1);
//    }

    SCH_RESULT err = raf.OpenStream();
    if (err != SCH_OK) exit(1);

    totalTime = raf.StreamLengthSeconds();
    cout << "Total seconds: " << totalTime << endl;;
    

    raf.StartStream();
    
    cout << "Supposed to be playing something..." << endl;
    chrono::milliseconds snoozeTime(1000);
    this_thread::sleep_for(snoozeTime);
    thread statusThread(printStatus, &raf);
    
    char poop='p'; 
    while (poop != 'q') {
        poop = getch();
    }

    keypress.store(1);
    cout << "\nWaiting for join..." << endl;
    statusThread.join();


    raf.StopStream();
    raf.CloseAndReleaseResources();


    return 0;
}

