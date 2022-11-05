
#include<iostream>
#include<thread>
#include<semaphore.h>
#include <stdlib.h>
using namespace std;
int in;
int total = 10;
int buffersize = 5;
int _buffer[5];
sem_t _empty, _full;


void prt(char c, int t) {
    string s ;
    if(c == 's') {
        s = "consumer";
    } else if (c == 'p') {
        s= "producer";
    }
    cout << s << ' ' << t << endl;
    total = total - 1;
    if(total == 0) {
        exit;
    }
}

int take() {
    int t;
    t = _buffer[in];
    in= (in-1) % buffersize;
    return t;
}

void append(int t) {
    _buffer[in] = t;
    in = in + 1 % buffersize;
}

int produce() {
    int t;
    t = rand()%10+1;
    prt('p', t);
    return t;
}
void consum(int t) {
    prt('s', t);
}

void consumer() {
    int t,m;
    sem_wait(&_empty);
    while(1) {
        sem_wait( &_full);
        t = take();
        m=in;
        sem_post(&_full);
        consum(t);
        if(m==0){
            sem_wait(&_empty);
        }
    }
}
int producer() {
    int t;
    while(1) {
        sem_wait(&_full);
        t = produce();
        _buffer[in] = t;
        in = (in + 1) % buffersize;
        if(in==1) {
            sem_post(&_empty);
        } else{
            sem_wait(&_full);
        }
    }
}

int main() {
    in = 0;
    std::thread t1 {producer};
    std::thread t2{consumer};
    t1.join();
    t2.join();



}

