#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__

#include "zmq.hpp"
#include "json11.hpp"
#include <map>

using namespace json11;

class Client {

    public:
        Client(zmq::context_t& context, const std::string &host, int port, int timeout);
        ~Client();
        void disconnect();
        Json call(Json &msg);
        void list(const std::string &watcher);
        void list();

    private:
        zmq::context_t      &context_;
        zmq::socket_t       *socket_;
        std::string         host_;
        int                 port_;
        int                 timeout_;
        std::string         m_cid;
        Json parse_message(zmq::message_t &msg);
};

#endif
