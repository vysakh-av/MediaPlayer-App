/**
 * File : FilePlayer.h
 * Description : This class is inherited from Player class and implement the
 * virtual function.
 * Objetcive of this class is used to play the media. This Player class mainly
 * used to playback
 * the file:// source files.
 */

#ifndef _FILEPLAYER_H
#define _FILEPLAYER_H

#include "Player.h"
#include <gst/gst.h>
#include <iostream>
#include <memory>

class FilePlayer : public Player
{
public:
  virtual RETCODE init(int, char**);
  virtual RETCODE load(const string&);
  virtual RETCODE play();
  virtual RETCODE pause();
  virtual RETCODE stop();
  static FilePlayer* getPlayerInstance();

private:
  FilePlayer();
  ~FilePlayer();
  GstElement* m_pPlaybin;
  bool m_isIntialised;
  string m_sUrl;
  static gboolean handleGstMsg(GstBus* bus, GstMessage* msg);
};
#endif /* _FILEPLAYER_H */
