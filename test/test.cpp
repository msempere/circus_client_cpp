#include "client.hpp"
#include <iostream>

int main(){
    std::string host = "127.0.0.1";
    int timeout = 1;
    int threads = 1;

    zmq::context_t context(threads);
    Client c = Client(context, host, 5555, timeout);

    std::string watcher = "watcher_name";
    c.list(watcher);
    c.list();
}
