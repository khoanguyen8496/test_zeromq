#ifndef SOCKET_CONFIG_H_
#define SOCKET_CONFIG_H_
#include <zmq.hpp>
#include <zmq_addon.hpp>

#include <string>
#include <iostream>
#include <vector>
namespace bioturing { namespace utils {
  class SocketConfig {
    private:
      std::string bind_addr_;
      int zmq_type_;
    public:
      SocketConfig() {};
      SocketConfig(const std::string &bind_addr) : bind_addr_(bind_addr) {};

      void SetBindAddr(const std::string &bind_addr) {
        bind_addr_ = bind_addr;
      }
      const std::string& GetBindAddr() const {
        return bind_addr_;
      }

      void SetZmqType(const int zmq_type) {
        zmq_type_ = zmq_type;
      }
      const int &GetZmqType() const {
        return zmq_type_;
      }


      virtual ~SocketConfig() {};
  };
}
}
#endif
