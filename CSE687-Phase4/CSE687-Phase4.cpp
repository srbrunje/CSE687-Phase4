// CSE687_Phase4.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "Menu.h"


int main() {
    SocketSystem ss;
    TestServer testServer = TestServer();
    testServer.StartServer();

    while (true) {
        RunMenu();
        system("cls");
    }

    testServer.StopServer();

    return 0;
}
