/**
 * File : Player.h
 * Description : Abstract class which defined the base functions for the Player.
 */
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "MessageQueue.h"
#include <string>

/* Enum define differnt type of user operations. */
typedef enum { PLAY = 0, PAUSE, STOP, QUIT, GSTERROR } PlayOps;

using namespace std;
/* Player return values. */
typedef enum {
  SUCCESS = 0,
  ERROR,
  INTERNAL_ERROR,
  INIT_FAILED,
  LOAD_FAILED,
  PLAY_FAILED,
  STOP_FAILED,
  PAUSE_FAILED,
  OPEN_FAILED,
  INVALID_URL
} RETCODE;

/**********************************************************
 * Player abstract class.
 **********************************************************/
class Player
{
public:
  virtual RETCODE init(int, char**) = 0;
  virtual RETCODE load(const string&) = 0;
  virtual RETCODE play() = 0;
  virtual RETCODE stop() = 0;
  virtual RETCODE pause() = 0;
  virtual ~Player() {} // virtual destructor.
};
#endif /* _PLAYER_H_ */