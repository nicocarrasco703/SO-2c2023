#include <thread>
#include <iostream>
#include <semaphore.h>
#include <vector>


#define MSG_COUNT 5
using namespace std;

sem_t s1, s2;

void f1_a() {

    for (int i = 0; i < MSG_COUNT; ++i) {
    cout << "Ejecutando F1 (A)\n";
    this_thread::sleep_for(100ms);
    }
}

void f1_b() {
    for (int i = 0; i < MSG_COUNT; ++i) {
    cout << "Ejecutando F1 (B)\n";
    this_thread::sleep_for(200ms);
    }
}

void f2_a() {
    for (int i = 0; i < MSG_COUNT; ++i) {
    cout << "Ejecutando F2 (A)\n";
    this_thread::sleep_for(500ms);
    }
}

void f2_b() {
    for (int i = 0; i < MSG_COUNT; ++i) {
    cout << "Ejecutando F2 (B)\n";
    this_thread::sleep_for(10ms);
    }
}

void f1() {
    f1_a();
    sem_post(&s1);
    sem_wait(&s2);
    f1_b();

}
void f2() {
    f2_a();
    sem_post(&s2);
    sem_wait(&s1);
    f2_b();
}
int main() {
    sem_init(&s1, 0, 0);
    sem_init(&s2, 0, 0);
    vector<thread> v;
    v.emplace_back(f1);
    v.emplace_back(f2);
    v[0].join();
    v[1].join();
    return 0;
}