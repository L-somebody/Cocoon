#include <list>
#include <stdexcept>
#include <string>
#include <cstdio>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <deque>
#include <iostream>
using namespace std;
namespace jj03
{
void test_list(long& value)
{
    cout << "\ntest_list()...... \n";

list<string> c;
char buf[10];

clock_t timeStart = clock();
    for(long i = 0; i < value; i++) {
        try {
            snprintf(buf, 10, "%d", rand());
            c.push_back(string(buf));
        } catch(exception& p) {
            cout << "i=" << i << " " << p.what() << endl;
            abort();
        }
    }
    cout << "milli-seconds : " << (clock() - timeStart) << endl;
    cout << "list.size()=" << c.size() << endl;
    cout << "list.max_size()=" << c.max_size() << endl;
    cout << "list.front()=" << c.front() << endl;
    cout << "list.back()=" << c.back() << endl;
}
}
int main(int argc, char *argv[]) {
    deque<int> *de;
    de->cbegin();
    deque<int> dee;
    dee.cbegin();
}