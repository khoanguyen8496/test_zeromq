#include "dummy_server.h"
#include <unistd.h>

static void *ThreadRunner(void *data) {
  struct dummy_server::PthreadData *p_data = (struct dummy_server::PthreadData*) data;
  dummy_server::StartServerAsync(*(p_data->config),
                                 *(p_data->notification),
                                 *(p_data->ctx));
  return NULL;
}

dummy_server::Notification::Notification(const std::vector<std::string> &user_list) {
  this->msg_data_.resize(user_list.size());
  for (int i = 0; i < (int) user_list.size(); ++i) {
    this->msg_dict_.insert({user_list[i], i});
  }
}

dummy_server::Notification::Notification(const std::map<std::string, std::string> &user_dict) {
  this->msg_data_.resize(user_dict.size());
  int i = 0;
  for (auto &user_item : user_dict) {
    this->msg_dict_[user_item.first] = 1;
    this->msg_data_[i] = user_item.second;
  }
}

bool dummy_server::Notification::AddUserMessage(const std::string &user_name,
                                                const std::string &message) {
  if (msg_dict_.find(user_name) != msg_dict_.end()) {
    // found key in map
    int const& index = msg_dict_.at(user_name);
    msg_data_[index] = message;
  } else {
    //
    msg_dict_.insert({user_name, msg_data_.size()});
    msg_data_.push_back(message);
  }
  return true;
}

bool dummy_server::Notification::GetMessage(const std::string &user_name,
                                            std::string *result) const {
  if (msg_dict_.find(user_name) != msg_dict_.end()) {
    int const &id = msg_dict_.at(user_name);
    *result = msg_data_[id];
  } else {
#ifdef D_DEBUG
    std::cerr << "Key " << user_name << " does not exist\n";
#endif
    return false;
  }
  return true;
}

bool dummy_server::Notification::ChangeUserMessage(const std::string &user_name,
                                                   const std::string &message) {
  if (msg_dict_.find(user_name) != msg_dict_.end()) {
    // found key in map
    int const& index = msg_dict_.at(user_name);
    msg_data_[index] = message;
  } else {
    return false;
  }
  return true;
}

std::vector<std::string> const &dummy_server::Notification::GetMsgList() const {
  return this->msg_data_;
}

void TestSimple() {
  // std::string bind_addr("tcp://127.0.0.1:10000");
  // bioturing::utils::SocketConfig config;
  // bool init_server_success = dummy_server::InitServerPub(bind_addr, &config);
  // assert(init_server_success);
  // dummy_server::StartServerSync(config);
  struct dummy_server::PthreadData *data = dummy_server::pthread_helper::InitPthreadData();
  pthread_t thr;
  pthread_create(&thr, NULL, ThreadRunner, data);
  dummy_server::pthread_helper::DestroyPthreadData(&data);
}

bool dummy_server::InitServerPub(const std::string &bind_addr,
                                 bioturing::utils::SocketConfig *res) {

  if (res == NULL) {
    std::cerr << "result value is not constructed\n";
    return false;
  }

  res->SetBindAddr(bind_addr);
  res->SetZmqType(ZMQ_PUB);
  return true;
}

std::map<std::string, int> const& dummy_server::Notification::GetUserDict() const {
  return msg_dict_;
}

void dummy_server::StartServerSync(const bioturing::utils::SocketConfig &config) {
  zmq::context_t ctx;
  try {
    int zmq_type = config.GetZmqType();
    zmq::socket_t soc(ctx, zmq_type);
    std::string bind_addr = config.GetBindAddr();
    std::cerr << bind_addr << std::endl;
    std::vector<std::string> user_list({"user1", "user2"});
    dummy_server::Notification notification(user_list);
    bool f = notification.ChangeUserMessage("user1", "hello");
    assert(f);
    f = notification.ChangeUserMessage("user2", "hello");
    // assert(f);
    auto &msg_data = notification.GetMsgList();
    auto &user_dict = notification.GetUserDict();
    soc.bind(bind_addr);
    while (1) {
      for (auto &item : user_dict) {
        // zmq::multipart_t msg_send(item.first);
        // msg_send.pushstr(msg_data[item.second]);
        // std::cerr << msg_send.size() << std::endl;
        // msg_send.send(soc, 0);
        int rc = soc.send(item.first.data(), item.first.size(), ZMQ_SNDMORE);
        assert(rc == item.first.size());
        rc = soc.send(msg_data[item.second].data(), msg_data[item.second].size(), 0);
        assert(rc == msg_data[item.second].size());
        usleep(5000);
      }
      notification.ChangeUserMessage("user1", "bye");
      std::cerr << "sent messages\n";
    }
  } catch (zmq::error_t &err) {
    std::cerr << err.what() << std::endl;
  }
}

void dummy_server::StartServerAsync(const bioturing::utils::SocketConfig &config,
                                    dummy_server::Notification &notification,
                                    zmq::context_t &ctx) {
  try {
    int zmq_type = config.GetZmqType();
    zmq::socket_t soc(ctx, zmq_type);
    std::string bind_addr = config.GetBindAddr();
    std::cerr << bind_addr << std::endl;
    bool f = notification.AddUserMessage("user1", "hello");
    assert(f);
    f = notification.AddUserMessage("user2", "hello");
    // assert(f);
    auto &msg_data = notification.GetMsgList();
    auto &user_dict = notification.GetUserDict();
    soc.bind(bind_addr);
    while (1) {
      for (auto &item : user_dict) {
        int rc = soc.send(item.first.data(), item.first.size(), ZMQ_SNDMORE);
        assert(rc == item.first.size());
        rc = soc.send(msg_data[item.second].data(), msg_data[item.second].size(), 0);
        assert(rc == msg_data[item.second].size());
        usleep(5000);
      }
      notification.ChangeUserMessage("user1", "bye");
      std::cerr << "sent messages\n";
    }
  } catch (zmq::error_t &err) {
    std::cerr << err.what() << std::endl;
  }

}

struct dummy_server::PthreadData *dummy_server::pthread_helper::InitPthreadData() {
  struct dummy_server::PthreadData *return_value = new struct dummy_server::PthreadData;
  zmq::context_t *ctx = new zmq::context_t();
  dummy_server::Notification *notification = new dummy_server::Notification();
  bioturing::utils::SocketConfig *config = new bioturing::utils::SocketConfig();
  return_value->ctx = ctx;
  return_value->notification = notification;
  return_value->config = config;
  config->SetBindAddr("tcp://127.0.0.1:10000");
  config->SetZmqType(ZMQ_PUB);
  return return_value;
}

void DestroyPthreadData(struct dummy_server::PthreadData **pp_data) {
  struct dummy_server::PthreadData *p_data = *pp_data;
  delete []p_data->ctx;
  delete []p_data->notification;
  delete []p_data->config;
  delete []p_data;
}

int main() {
  TestSimple();
  return 0;
}
