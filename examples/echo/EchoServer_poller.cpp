#include <vector>
#include <map>

#include <string.h>     // memset

#include <tools/log/log.h>
#include <tools/base/Exception.h>
#include <tools/socket/ConnSocket.h>
#include <tools/socket/SocketsOps.h>
#include <tools/socket/InetAddress.h>
#include <tools/socket/ServerSocket.h>
#include <tools/poller/Channel.h>
#include <tools/poller/Poller.h>

#define LISTEN_PORT 9000
#define BUFFER_SZ 1024

ServerSocket ss(sockets::create_nonblocking_socket(sockets::IPv4));
Poller *poller = Poller::new_default_Poller();
std::map<Channel*, ConnSocket> connPool;

void on_message(Channel *channelPtr){
  char buf[BUFFER_SZ] = {0};
  auto connSocketIter = connPool.find(channelPtr);
  ssize_t len = connSocketIter->second.read(buf, BUFFER_SZ);
  if(len < 0){
    LOG_WARN("read error");
  }
  else if(len == 0){
    LOG_INFO("client disconnects");

    poller->remove_channel(connSocketIter->first);
    delete connSocketIter->first;
    connPool.erase(connSocketIter);
  }
  else{
    connSocketIter->second.write(buf, len);
  }
}

void on_connection(){
  try{
    ConnSocket connSocket = ss.accept_nonblocking();
    Channel *connChannel = new Channel(connSocket.get_sockfd());
    connChannel->set_read_callback(std::bind(on_message, connChannel));
    connChannel->enable_reading();
    poller->update_channel(connChannel);
    connPool.insert({connChannel, connSocket});
  }
  catch(const Exception &e){
    LOG_WARN("accept error");
  }
}

int main(){
  InetAddress addr(LISTEN_PORT);
  ss.set_reuse_address(true);
  ss.bind(addr);
  ss.listen();
  LOG_INFO("server is listening...");

  Channel listenChannel(ss.get_sockfd());
  listenChannel.set_read_callback(on_connection);
  listenChannel.enable_reading();
  poller->update_channel(&listenChannel);

  for(;;){
    Poller::ChannelList activeChannels = poller->poll(-1);
    for(auto &channelPtr : activeChannels){
      channelPtr->handle_event();
    }
  }

  return 0;
}