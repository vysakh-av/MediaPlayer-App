#include "Controller.h"
#include "MessageQueue.h"
#include <iostream>
using namespace std;

/* App's message queue. */
extern MessageQueue<PlayOps> g_MsgQ;

Controller::Controller() : _url("") {
    _player = std::make_shared<MediaPlayer>();
}

Controller::~Controller() {}

void Controller::ControllPlayback() {
    while (true) {
        /* Read from MsgQueue.*/
        PlayOps ops = g_MsgQ.receive();
        // std::cout << "User selected operation = " << ops << std::endl;
        if (ops == PLAY) {
            if (!_url.empty()) {
                _player->load(_url);
                if (_player->play() != SUCCESS) {
                    std::cout << "ERROR:Failed to start playback. \n";
                    _cond.notify_one(); // Notify Cleanup thread to stop it.
                }
            }
        } else if (ops == PAUSE) {
            if (!_url.empty()) {
                _player->pause();
            }
        } else if (ops == STOP) {
            if (!_url.empty()) {
                _player->stop();
            }
        } else if (ops == QUIT) {
            _cond.notify_one(); // Notify Cleanup thread to stop it.
            break;
        } else if (ops == GSTERROR) {
            std::cout
                << "GSTREAMER ERROR: Failed to playback the stream. Looks like "
                   "a missing plugin/bad URL \n";
            _cond.notify_one(); // Notify Cleanup thread to stop it.
            break;
        } else {
            std::cout << "Invalid operation \n";
        }
    }
}

void Controller::ControllSwapPlayback(const PlayOps &ops) {
    std::cout << "User selected operation = " << ops << std::endl;
    if (ops == PLAY) {
        if (!_url.empty()) {
            _player->load(_url);
            _player->play();
        }
    } else if (ops == PAUSE) {
        if (!_url.empty()) {
            _player->pause();
        }
    } else if (ops == STOP) {
        if (!_url.empty()) {
            _player->stop();
        }
    } else {
        std::cout << "Invalid operation \n";
    }
}

void Controller::GetPlaybackURLFromUser(std::promise<std::string> &&prms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::string url;
    std::cout << "Please enter the playback URL, (http:// or file://): \n";
    std::cin >> url;
    /* Update member _url. */
    _url = url;
    prms.set_value(url);
}

void Controller::Cleanup() {
    /** A thread will wait for notification and do the clean up when get
     * notification. */
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::unique_lock<std::mutex> uLock(_mutex);
    _cond.wait(uLock);

    if (_mainLoop) {
        g_print("QUITTING the pipeline\n");
        g_main_loop_quit(_mainLoop);
        _mainLoop = NULL;
    }

    if (_userInChannel) {
        g_print("QUITTING IOCHANNEL\n");
        g_io_channel_unref(_userInChannel);
        _userInChannel = NULL;
    }
}

gboolean Controller::UserInCallback(GIOChannel *source, GIOCondition condition,
                                    gpointer data) {
    gchar *str = NULL;
    gsize str_len = 0;
    gsize terminator_pos = 0;
    GError *error = NULL;

    GIOStatus rc =
        g_io_channel_read_line(source, &str, &str_len, &terminator_pos, &error);
    if (rc == G_IO_STATUS_EOF) {
        g_MsgQ.send(QUIT);
        return FALSE;
    }

    if (NULL != str) {
        if (strncmp(str, "play", 4) == 0) {
            g_MsgQ.send(PLAY);
        } else if (strncmp(str, "pause", 5) == 0) {
            g_MsgQ.send(PAUSE);
        } else if (strncmp(str, "quit", 4) == 0) {
            g_MsgQ.send(QUIT);
        } else if (strncmp(str, "stop", 4) == 0) {
            g_MsgQ.send(STOP);
        } else {
            std::cout
                << "Invalid user input!!! Use from (play/pause/stop/quit) \n";
        }
    }
    return true;
}

bool Controller::RunGMainLoop() {
    _mainLoop = g_main_loop_new(NULL, FALSE);
    if (NULL == _mainLoop) {
        std::cout << " g_main_loop is null\n";
        return false;
    }
    _userInChannel = g_io_channel_unix_new(fileno(stdin));
    if (NULL == _userInChannel) {
        std::cout << " g_io_channel_unix_new is null\n";
        return false;
    }
    if ((NULL != _userInChannel) && (NULL != _mainLoop)) {
        g_io_add_watch(_userInChannel, G_IO_IN, UserInCallback, NULL);
        g_main_loop_run(_mainLoop);
    } else {
        cout << "Since g_io_channel_unix_new failed, we are not running the "
                "main g_GlibMainLoop \n";
        return false;
    }
    return true;
}

void Controller::SetPlaybackUrl(std::string &url) { _url = url; }

std::string Controller::GetPlaybackUrl() { return _url; }