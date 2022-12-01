#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <string>
#include <arpa/inet.h>

using namespace std;

int main()
{
    // Ceate a socket
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if(listening == -1){
        cerr << "Can't create a socket!";
        return -1;
    }

    // Bind the socket to a IP / port
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(8080);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    bind(listening, (sockaddr*)&hint, sizeof(hint));

    // Mark the socket for listening socket
    if(listen(listening, SOMAXCONN) == -1){
        cerr << "Can't listen!";
        return -2;
    }

    // Accept a call
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);



    int clientSocket = accept(listening,
                              (sockaddr*)&client,
                              &clientSize);
    char host[NI_MAXHOST];  //Client's remote name
    char service[NI_MAXSERV];   // Service the client is connected on

    memset(host, 0 , NI_MAXHOST);
    memset(service, 0, NI_MAXSERV);

    if(clientSocket == -1){
        cerr << "Problem with client connection!";
        return -3;
    }


    if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
    {
        cout << host << " connected on port " << service << endl;
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        cout << host << " connected on port " << ntohs(client.sin_port) << endl;
    }

    // Close listening socket
    close(listening);

    // While receiving display message, echo message
    char buf[4096];
    while(true){
        // Clear the buffer
        memset(buf,0,4096);
        // Wait for a message
        int bytesRecv = recv(clientSocket, buf, 4096, 0);
        if(bytesRecv == -1){
            cerr << "There was a connection issue" << endl;
            break;
        }

        if(bytesRecv == 0){
            cout << "The client disconnected" << endl;
            break;
        }

        // Display message
        cout << "Received: " << string(buf, 0, bytesRecv) << endl;

        // Resend message
        send(clientSocket, buf, bytesRecv+1, 0);
    }

    // Close socket
    close(clientSocket);

    return 0;
}
