/**
 * File : HTTPPlayer.h
 * Description : This class is inherited from Player class and implement the
 * virtual function.
 * Objetcive of this class is used to play the media. This Player class mainly
 * used to playback
 * the http:// source files.
 */

#ifndef _HTTPPLAYER_H
#define _HTTPPLAYER_H

#include "MessageQueue.h"
#include "Player.h"
#include <gst/gst.h>
#include <iostream>
#include <memory>

class HTTPPlayer : public Player {
  public:
    virtual RETCODE init(int, char **);
    virtual RETCODE load(const string &);
    virtual RETCODE play();
    virtual RETCODE pause();
    virtual RETCODE stop();
    static HTTPPlayer *getPlayerInstance();
    void SetErrorMsg(std::string msg);
    void SetErrorFlag(bool flag);

  private:
    HTTPPlayer();
    ~HTTPPlayer();
    GstElement *m_pPlaybin;
    bool m_isIntialised;
    string m_sUrl;
    GstBus *_bus;
    static gboolean handleGstMsg(GstBus *bus, GstMessage *msg);
};
#endif /* _HTTPPLAYER_H */
