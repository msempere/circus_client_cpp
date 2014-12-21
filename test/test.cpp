#include "client.hpp"
#include <iostream>

using namespace cccpp;

int main(){
    zmq::context_t context(1);
    Client c = Client(context, "127.0.0.1", 5555, 1);

    c.add("a_proc", "ls", {"-la"}, true);

    std::cout<<c.status("a_proc")<<std::endl;

    for (auto watcher: c.list()){
        std::cout<< watcher <<std::endl;
    }
}
