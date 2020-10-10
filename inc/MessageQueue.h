#ifndef _MSG_Q_
#define _MSG_Q_
#include <condition_variable>
#include <deque>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
/*
 * Message queue. The main purpose of this message queue is communicate between
 * the
 * playback controll thread and glib stdin callback handler method. Stdin
 * callback
 * handler will send message to queue, on the other hand, playback controller
 * method will do the neceesary actions based on user events.
 */
template <class T>
class MessageQueue
{
public:
  T receive()
  {
    std::unique_lock<std::mutex> uLock(_mutex);
    _cond.wait(uLock, [this] { return !_messages.empty(); });
    T msg = std::move(_messages.back());
    _messages.pop_back();
    return msg;
  }

  void send(T&& msg)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::lock_guard<std::mutex> uLock(_mutex);
    _messages.push_back(std::move(msg));
    _cond.notify_one();
  }

private:
  std::mutex _mutex;
  std::condition_variable _cond;
  std::deque<T> _messages;
};
#endif // _MSG_Q_