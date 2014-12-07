#include "client.hpp"
#include <sstream>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

using namespace json11;

Client::Client(zmq::context_t& context, const std::string &host, int port, int timeout)
    :context_(context), host_(host), port_(port), timeout_(timeout*1000) {

    socket_ = new zmq::socket_t(context_, ZMQ_DEALER);
    int linger = 0;
    std::stringstream ss;
    boost::uuids::uuid uuid = boost::uuids::random_generator()();

    ss<<uuid;
    m_cid=ss.str();
    socket_->setsockopt(ZMQ_IDENTITY, m_cid.data(), m_cid.size());
    socket_->setsockopt(ZMQ_LINGER, &linger, sizeof(linger));
    char addr[128];
    snprintf(addr, sizeof(addr), "tcp://%s:%d", host.c_str(), port);
    socket_->connect(addr);
}

Client::~Client(){
    delete socket_;
    socket_ = NULL;
}

void Client::disconnect(){};

void Client::list(){
    Json msg = Json::object {
        {"id", this->m_cid},
        {"command", "list"},
    };
    Json response = call(msg);
    std::cout << response.dump() << std::endl;
}

void Client::list(const std::string &watcher){
    Json msg = Json::object {
        {"id", this->m_cid},
        {"command", "list"},
        {"properties", Json::object {{"name", watcher}}},
    };
    Json response = call(msg);
    std::cout << response.dump() << std::endl;
}


Json Client::parse_message(zmq::message_t &msg){
    std::string rpl = std::string(static_cast<char*>(msg.data()), msg.size());
    std::string err;
    return Json::parse(rpl, err);
}

Json Client::call(Json &cmd){
    std::string cmd_s = cmd.dump();
    int cmd_size = cmd_s.size();
    zmq::message_t msg(cmd_size);
    memcpy((void*) msg.data(), cmd_s.c_str(), cmd_size);
    socket_->send(msg);

    zmq::message_t response;
    socket_->recv(&response);
    return parse_message(response);
}
