#include <thread>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <mutex>


#define CANT_THREADS 10

using namespace std;

void printear(int i){
    mutex mtx;
    mtx.lock();
    for (int i = 0; i < 5; i++){
    cout << "Hola! Soy el thread: " << i << endl;
    this_thread::sleep_for(100ms);
    cout << "Chau! Saludos desde: " << i << endl;
    this_thread::sleep_for(200ms); 
    }
    mtx.unlock();
    return;
}

int main() {
    vector<thread> v;
    for (int i = 0; i < CANT_THREADS; i++){
        v.emplace_back(printear, i);
    }
    for (int i = 0; i < CANT_THREADS; i++)
    {
        v[i].join();
    }
    return 0;
}