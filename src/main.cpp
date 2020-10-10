#include "Controller.h"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <thread>
#include <unistd.h>
#include <vector>

using namespace std;

typedef enum { AUTO = 1, MANUAL = 2 } APP_MODE;

static const std::string playlist = "../Playlists.txt";
static int ShowAppUsage() {
    std::cout
        << "============================================================= \n"
        << "            A console based Mediaplayer App \n"
        << "**************************************************************\n"
        << "This application works in 2 modes: \n"
        << "-----> 1. AUTO mode \n"
        << "-----> 2. MANUAL mode \n"
        << "In AUTO mode, app automatically plays a number of streams "
           "available in playlist \n"
        << "In MANUAL mode, user can control playback like start/stop/pause "
           "and exit. \n"
        << "This player app supports the following options in manual mode: \n"
        << " - User can enter the playback URL in console \n"
        << " - Type the following commands in console to controlle playback, \n"
        << " ---------> play   (Start the playback) \n"
        << " ---------> pause  (Pause the playback) \n"
        << " ---------> stop   (Stop  the playback) \n"
        << " ---------> quit   (Exit from app) \n"
        << " ---------> quit   (Exit from app) \n"
        << "============================================================= \n\n";
    int app_mode = 0;
    std::cout << "Please enter app mode (1 For AUTO, 2 For Manual) :-> \t";
    std::cin >> app_mode;
    return (app_mode);
}

int main(int argc, char **argv) {

    int appMode = ShowAppUsage();
    std::cout << "User selected mode = " << appMode << "\n";

    // Controll intstance.
    std::shared_ptr<Controller> controller = std::make_shared<Controller>();

    if (appMode == AUTO) {
        std::vector<std::string> myPlaylist;
        // Read media from text file.
        std::string line;
        std::ifstream mediaFile(playlist);
        if (mediaFile.is_open()) {
            while (std::getline(mediaFile, line)) {
                std::cout << line << "\n";
                myPlaylist.push_back(line);
            }
        }

        /** Read one by one. Play first one, sleep for some time, play next.
         *  App exit once all files played **/
        if (!myPlaylist.empty()) {
            vector<std::string>::iterator it;
            for (it = myPlaylist.begin(); it != myPlaylist.end(); ++it) {
                cout << "Channel URL = " << *it << "\n";
                controller->SetPlaybackUrl(*it);
                controller->ControllSwapPlayback(PlayOps::PLAY);
                this_thread::sleep_for(chrono::milliseconds(10000));
                controller->ControllSwapPlayback(PlayOps::STOP);
            }
        }
    } else if (appMode == MANUAL) {
        /*
         * Launch a thread and use promise-future mecanism to read the
         * playback url from the user. Wait for some time, exit the app
         * if user is not entered any playback url.
         */
        std::string purl;
        std::promise<std::string> prms;
        std::future<std::string> ftrURL = prms.get_future();
        std::thread getURLThread(&Controller::GetPlaybackURLFromUser,
                                 controller, std::move(prms));
        auto status = ftrURL.wait_for(std::chrono::milliseconds(10000));
        if (status == std::future_status::ready) // result is ready
        {
            purl = ftrURL.get();
            std::cout << "Playback URL received = " << purl << std::endl;

        } else {
            std::cout
                << " Failed to get playback URL!!! Exiting application \n";
            exit(EXIT_SUCCESS);
        }

        /*
         * Spawn a thread to controll playback.
         * ControllPlayback() will wait for the user triggered input. Based on
         * user's
         * request, method will do the necesseray jobs.
         */
        std::thread PlaybackCntrThread(&Controller::ControllPlayback,
                                       controller);
        PlaybackCntrThread.detach();

        /*
         * Spawn a thread to clean up.
         * Cleanup() method will wait for a condiional varaible. When user
         * entered `quit` on console, the
         * input callback handler will notify the conditional variable, then it
         * do the necessary cleanup.
         */
        std::thread CleanupThread(&Controller::Cleanup, controller);
        CleanupThread.detach();

        /* Run main glib event loop. */
        if (controller->RunGMainLoop() == false) {
            std::cout << "Failed to start main event loop \n";
        }

        /* Join thread. */
        getURLThread.join();
    } else {
        std::cout << "Invalid mode selected, Exiting app \n";
        exit(EXIT_SUCCESS);
    }

    return 0;
}