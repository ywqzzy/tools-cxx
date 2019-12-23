#ifndef POLLPOLLER_H_
#define POLLPOLLER_H_

#include <vector>

#include <poll.h> // pollfd

#include "tools/poller/Poller.h"

class Channel;

class PollPoller : public Poller{
public:
  PollPoller() = default;

  virtual ~PollPoller() = default;

  virtual void poll(int timeoutMs, ChannelList *activeChannels) override;
  virtual void update_channel(Channel *channel) override;
  virtual void remove_channel(Channel *channel) override;

private:
  void fill_active_channels(int numEvents, ChannelList *activeChannels) const; 

private:
  std::vector<struct pollfd> m_pollfdList;
};

#endif // POLLPOLLER_H_