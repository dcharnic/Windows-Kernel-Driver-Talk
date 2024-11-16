#include <iostream>
#include <windows.h>
using namespace std;

int main()
{
    int var = 0xdeadbeef;
    DWORD pid = GetCurrentProcessId();
    cout << "PID: " << pid << endl;
    cout << "Address of var: " << &var << endl;

    while (true) {
        if (var == 0xdeadbeef) {
            cout << "All good from userland!" << endl;
            cout << "PID: " << pid << endl;
            cout << "Address of var: " << &var << endl;
        }
        else {
            cout << "We've been hacked!" << endl;
        }
        Sleep(1000);
    }
}
