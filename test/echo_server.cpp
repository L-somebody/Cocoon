#include <iostream>
#include "pine.h"

int main() {
    std::unique_ptr<TcpServer> server = std::make_unique<TcpServer>();

    Signal::signal(SIGINT, [&] {
        server.release();
        std::cout << "\nServer exit!" << std::endl;
        exit(0);
    });

    server->onConnect([](Connection *conn) { std::cout << "New connection fd: " << conn->socket()->fd() << std::endl; });

    server->onRecv([](Connection *conn) {
        std::cout << "Message from client " << conn->read_buf()->c_str() << std::endl;
        conn->Send(conn->read_buf()->c_str());
    });

    server->Start();

    return 0;
}