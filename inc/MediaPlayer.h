/**
 * @brief  MediaPlayer application
 * @note  Based on the SRC url, choose appropriate player and playback the
 * stream.
 */

#ifndef _MEDIAPLAYER_H_
#define _MEDIAPLAYER_H_

#include "Player.h"
#include <memory>

// Player type
typedef enum {
  HTTP_PLAYBACK,
  FILE_PLAYBACK,
  INVALID_TYPE,
} PLAYBACK_TYPE;
using namespace std;

class MediaPlayer
{
public:
  MediaPlayer();
  ~MediaPlayer();
  MediaPlayer(const MediaPlayer& src) = default;
  MediaPlayer& operator=(const MediaPlayer& src) = default;
  MediaPlayer(MediaPlayer&& src) = default;
  MediaPlayer& operator=(MediaPlayer&& src) = default;
  RETCODE load(const std::string& uri);
  RETCODE play();
  RETCODE pause();
  RETCODE stop();

private:
  std::string pURI;
  Player* player;
  PLAYBACK_TYPE ePlayerType;
  RETCODE createPlayer(const string& uri);
};
#endif //_MEDIAPLAYER_H_.
