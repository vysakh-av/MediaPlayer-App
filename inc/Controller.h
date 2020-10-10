#ifndef _COTROLLER_H
#define _COTROLLER_H
#include "MediaPlayer.h"
#include <future>
#include <glib.h>
#include <iostream>
#include <memory.h>
#include <mutex>
#include <queue>
#include <thread>

/**
 * Controller class.
 * Objective is to controll playback,user inputs and start/stop main glib event
 * loop.
 */
class Controller
{
public:
  Controller();
  ~Controller();

  // Rule of Five
  Controller(const Controller& src) = delete;
  Controller& operator=(const Controller& src) = delete;
  Controller(Controller&& src) = delete;
  Controller& operator=(Controller&& src) = delete;

  bool RunGMainLoop();
  void GetPlaybackURLFromUser(std::promise<std::string>&& prms);
  void Cleanup();
  void SetPlaybackUrl(std::string& url);
  std::string GetPlaybackUrl();
  void ControllPlayback();
  void ControllSwapPlayback(const PlayOps& ops);
  void ErrorMonitor();

private:
  std::shared_ptr<MediaPlayer> _player;
  std::mutex _mutex;
  std::condition_variable _cond;
  GMainLoop* _mainLoop;
  GIOChannel* _userInChannel;
  std::string _url;
  static gboolean UserInCallback(GIOChannel* source, GIOCondition condition,
                                 gpointer data);
};

#endif //_COTROLLER_H