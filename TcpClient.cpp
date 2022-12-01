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
    // Create a socket
    int sock = socket(AF_INET, SOCK_STREAM,0);
    if(sock == -1){
        return 1;
    }
    // Create a hint structure for the server connecting with
    int port = 8080;

    string ipAddress = "127.0.0.1";

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

    // Connet to the server on the socket
    int connectRes = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if(connectRes == -1){
        return 1;
    }

    char buf[4096];
    string userInput;

    while(true){
        // Enter lines of text
        cout << " > ";
        getline(cin, userInput);

        // Periodically send a message  to the server for 10 seconds
        if(!strcmp(userInput.c_str(), "test")){
            int messages_count = 1;
            string sendmessage;
            while(true){
                sendmessage = "This is message " +  to_string(messages_count);
                send(sock, sendmessage.c_str(), sendmessage.size()+1, 0);
                memset(buf, 0, 4096);
                cout << "SERVER>" << string(buf, recv(sock, buf, 4096, 0)) << "\r\n";
                messages_count++;
                if (messages_count > 10){
                    cout<< "Test over \r\n";
                    break;
                }
                sleep(1);
            }
            continue;
        }

        // Send to server
        int sendRes = send(sock, userInput.c_str(), userInput.size()+1, 0);
        if(sendRes == -1){
            cout << "Could not send to server! \r\n";
            continue;
        }

        // Wait for response;
        memset(buf, 0, 4096);
        int bytesReceived = recv(sock, buf, 4096, 0);
        if(bytesReceived == -1){
            cout<< "There was an error getting response from server\r\n";
        }
        // Display response
        cout << "SERVER>" << string(buf, bytesReceived) << "\r\n";
    }

    // Close the socket
    return 0;
}
