#ifndef DUMMY_SERVER_H
#define DUMMY_SERVER_H

#include <zmq.hpp>
#include <zmq_addon.hpp>
#include <socket_config.hpp>

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

namespace dummy_server {
    //need caching
  class Notification {

    public:
      Notification() { }
      Notification(const std::vector<std::string> &user_list);
      Notification(const std::map<std::string, std::string> &user_dict);
      bool AddUserMessage(const std::string &user_name, const std::string &message);
      bool ChangeUserMessage(const std::string &user_name, const std::string &message);
      std::vector<std::string> const& GetMsgList() const;
      std::map<std::string, int> const& GetUserDict() const;
      bool GetMessage(const std::string &user_name, std::string *result) const;

    private:
      std::vector<std::string> msg_data_;
      std::map<std::string, int> msg_dict_;
  };

  struct PthreadData {
    Notification *notification;
    zmq::context_t *ctx;
    bioturing::utils::SocketConfig *config;
  };

  namespace pthread_helper {
    struct PthreadData *InitPthreadData();
    void DestroyPthreadData(struct dummy_server::PthreadData **);
  };

  void StartServerAsync(const bioturing::utils::SocketConfig &config,
                        dummy_server::Notification &notification,
                        zmq::context_t &ctx);
  void StartServerSync(const bioturing::utils::SocketConfig &config);
  bool InitServerPub(const std::string &bind_addr, bioturing::utils::SocketConfig *res = NULL);
}
#endif
