#include "pine.h"
#include "iostream"
#include "memory"

int main() {
    std::unique_ptr<Socket> sock = std::make_unique<Socket>();
    sock->Create();
    sock->Connect("127.0.0.1", 1234);

    std::unique_ptr<Connection> conn = std::make_unique<Connection>(sock->fd(), nullptr);

    while(true) {
        std::string input;
        std::getline(std::cin, input);
        conn->set_send_buf(input.c_str());
        conn->Write();
        if(conn->state() == Connection::State::Closed) {
            conn->Close();
            break;
        }
        conn->Read();
        std::cout << "Message from server: " << conn->read_buf()->c_str() << std::endl;
    }

    return 0;
}