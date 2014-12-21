#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__

#include "zmq.hpp"
#include "json11.hpp"
#include <map>
#include <vector>

using namespace json11;

namespace cccpp{

    class Client {

        public:
            Client(zmq::context_t& context, const std::string &host, int port, int timeout);
            ~Client();

            // list processes in a watcher
            std::vector<std::string> list(const std::string &watcher);

            // list watchers
            std::vector<std::string>  list();

            // add a new watcher with a process
            bool add(const std::string &name, const std::string &command, const std::vector<std::string> &args, bool autostart);

            // get status from a watcher
            std::string status(const std::string &watcher);

        private:
            zmq::context_t      &context_;
            zmq::socket_t       *socket_;
            std::string         host_;
            int                 port_;
            int                 timeout_;
            std::string         m_cid;
            Json parse_message(zmq::message_t &msg);
            Json call(Json &msg);
            void disconnect();
    };
}

#endif
