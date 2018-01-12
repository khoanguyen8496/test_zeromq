#include <zmq.hpp>
#include <zmq_addon.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <assert.h>

void TestClient() {
  zmq::context_t ctx;
  zmq::socket_t soc(ctx, ZMQ_SUB);
  soc.setsockopt(ZMQ_SUBSCRIBE, "user1", 5);
  soc.connect("tcp://127.0.0.1:10000");
  if (soc.connected()) {
    std::cerr << "connected to publisher\n";
    zmq::message_t msg;
    bool f  = soc.recv(&msg);
    assert(f);
    if (msg.size()) {
      std::cerr << "recv msg\n";
    } else {
      std::cerr << "fail to receive any messages\n";
    }
  }
  // std::cerr << std::endl;
}

int main() {
  TestClient();
  return 0;
}
