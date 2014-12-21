#include "client.hpp"
#include "circus_exception.hpp"
#include <utility>
#include <sstream>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

using namespace json11;
using namespace cccpp;

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

std::vector<std::string> Client::list(){
    Json msg = Json::object {
        {"id", this->m_cid},
        {"command", "list"},
    };
    Json response = call(msg);
    std::vector<std::string> watchers;

    if(response["status"].string_value() == "ok"){
        for (auto k : response["watchers"].array_items()){
            watchers.push_back(k.string_value());
        }
    }
    else{
        throw CircusException(response["reason"].string_value(), response["errno"].int_value());
    }
    return watchers;
}

//TODO if watcher exists, add the proc
//TODO proc as object?
bool Client::add(const std::string &name, const std::string &command, const std::vector<std::string> &args, bool autostart){

    std::string arguments = "";
    for(auto arg: args)
        arguments += arg + " ";

    Json msg = Json::object {
        {"id", this->m_cid},
        {"command","add"},
        {"properties",
            Json::object {
                {"cmd", command},
                {"name", name},
                {"numprocesses", std::to_string(1)},
                {"start", autostart},
                {"args", arguments},
                {"options",
                    Json::object {
                    {"singleton", true},
                    {"shell", true}
                    },
                },
            },
        },
    };
    Json response = call(msg);
    if(response["status"].string_value() == "ok"){
        return true;
    }
    else{
        throw CircusException(response["reason"].string_value(), response["errno"].int_value());
    }
    return false;
}


std::string Client::status(const std::string &watcher){

    Json msg = Json::object {
        {"id", this->m_cid},
        {"command", "status"},
        {"properties", { Json::object {{"name", watcher}}}},
    };
    Json response = call(msg);

    if(response["status"].string_value() != "error"){
        return response["status"].string_value();
    }
    else {
        return "not running";
    }
}

//TODO change errors to exceptions
std::vector<std::string> Client::list(const std::string &watcher){
    Json msg = Json::object {
        {"id", this->m_cid},
        {"command", "list"},
        {"properties", Json::object {{"name", watcher}}},
    };
    Json response = call(msg);
    std::vector<std::string> watchers;

    if(response["status"].string_value() == "ok"){
        for (auto k : response["watchers"].array_items()){
            watchers.push_back(k.string_value());
        }
    }
    else{
        throw CircusException(response["reason"].string_value(), response["errno"].int_value());
    }
    return watchers;
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
