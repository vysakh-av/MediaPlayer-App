/**
 * MediaPlayer.cpp
 *
 * Implementation of Mediaplayer Application.
 */

#include "MediaPlayer.h"
#include "FilePlayer.h"
#include "HTTPPlayer.h"

extern MessageQueue<PlayOps> g_MsgQ;

// Constants.
const string http_url = "http://";
const string https_url = "https://";
const string file_url = "file://";

// Constructor.
MediaPlayer::MediaPlayer()
    : pURI(""), player(NULL), ePlayerType(INVALID_TYPE) {}

// Destructor.
MediaPlayer::~MediaPlayer() {}

RETCODE
MediaPlayer::load(const string &uri) {
    RETCODE rc = SUCCESS;

    if (uri.empty()) {
        cout << "ERROR:Playback URL is not set \n";
        return (LOAD_FAILED);
    }

    pURI = uri; // Set the URL.

    rc = createPlayer(uri);
    if (rc != SUCCESS) {
        cout << "ERROR:Player object is not created \n";
        return (LOAD_FAILED);
    }

    return (SUCCESS);
}

RETCODE
MediaPlayer::createPlayer(const string &uri) {
    RETCODE rc = SUCCESS;

    /* Clear the object. */
    if ((uri.find(http_url) != string::npos) ||
        (uri.find(https_url) != string::npos)) {
        player = HTTPPlayer::getPlayerInstance();
        if (player == NULL) {
            cout << "ERROR:HTTPPlayer instance failed to created \n";
            return (INTERNAL_ERROR);
        }
        ePlayerType = HTTP_PLAYBACK;
    } else if ((uri.find(file_url) != string::npos)) {
        player = FilePlayer::getPlayerInstance();
        if (player == NULL) {
            cout << "ERROR:FilePlayer instance failed to created \n";
            return (INTERNAL_ERROR);
        }
        ePlayerType = FILE_PLAYBACK;
    } else {
        cout << "ERROR:Invalid player \n";
        ePlayerType = INVALID_TYPE;
        return (INTERNAL_ERROR);
    }

    // Player intialisation.
    rc = player->init(0, NULL);
    if (rc != SUCCESS) {
        cout << "ERROR:Player initialization failed \n";
        return (INIT_FAILED);
    }

    rc = player->load(uri);
    if (rc != SUCCESS) {
        cout << "ERROR:Player load failed \n";
        return (LOAD_FAILED);
    }
    return SUCCESS;
}

RETCODE
MediaPlayer::play() {
    RETCODE rc = SUCCESS;

    if (player) {
        rc = player->play();
        if (rc != SUCCESS) {
            cout << "ERROR:Player is not able to playback the stream \n";
            return (PLAY_FAILED);
        }
    } else {
        cout << "ERROR: Player instance not found. \n";
        return (PLAY_FAILED);
    }
    std::cout << "Playbacking the stream " << pURI << " started successfully"
              << std::endl;
    return (rc);
}

RETCODE
MediaPlayer::pause() {
    RETCODE rc = SUCCESS;
    if (player) {
        rc = player->pause();
        if (rc != SUCCESS) {
            cout << "ERROR:Player is not able to pause the stream \n";
            rc = PAUSE_FAILED;
        }
    } else {
        cout << "ERROR:Player instance not found. \n";
        rc = PAUSE_FAILED;
    }
    return rc;
}
RETCODE
MediaPlayer::stop() {
    RETCODE rc = SUCCESS;
    if (player) {
        rc = player->stop();
        if (rc != SUCCESS) {
            cout << "ERROR:Player stop call failed \n";
            return (STOP_FAILED);
        }
    } else {
        cout << "ERROR:Player instance not available \n\t";
        return (STOP_FAILED);
    }

    return (SUCCESS);
}