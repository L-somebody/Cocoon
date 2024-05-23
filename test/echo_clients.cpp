#include "unistd.h"
#include <iostream>
#include "pine.h"
using namespace std;

void OneClient(int msgs, int wait){
    unique_ptr<Socket> sock = make_unique<Socket>();
    sock->Create();
    sock->Connect("127.0.0.1", 1234);

    unique_ptr<Connection> conn = make_unique<Connection>(sock->fd(), nullptr);
    sleep(wait);

    int count = 0;
    while(count < msgs) {
        conn->set_send_buf("I'm client!");
        conn->Write();
        if (conn->state() == Connection::State::Closed) {
            conn->Close();
            break;
        }
        conn->Read();
        cout << "msg count" << count++ << ": " << conn->read_buf()->c_str() << endl;
    }
}

int main(int argc, char *argv[]) {
    int threads = 100;
    int msgs = 100;
    int wait = 0;
    int o = -1;
    const char *opt_string = "t:m:w:";
    while((o = getopt(argc, argv, opt_string)) != -1) {
        switch (o) {
            case 't':
                threads = std::stoi(optarg);
                break;
            case 'm':
                msgs = std::stoi(optarg);
                break;
            case 'w':
                wait = std::stoi(optarg);
                break;
            case '?':
                printf("error opt_opt: %c\n", optopt);
                printf("error opt_err: %d\n", opterr);
                break;
            default:
                break;
        }
    }

    unique_ptr<ThreadPool> poll = make_unique<ThreadPool>(threads);
    std::function<void()> func = [msgs, wait](){ OneClient(msgs, wait); };
    for(int i = 0; i < threads; i++) {
        poll->add(func);
    }

    return 0;
}