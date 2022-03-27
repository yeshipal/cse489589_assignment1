
#include <iostream>
#include <string>
#include <map>
#include "../include/Server.h"

#define DNS_ADDR "8.8.8.8"
#define DNS_PORT 53
#define MAXHOST 255

using namespace std;
string GetMyIP() {
    char *ip;
    int fd;
    struct sockaddr_in servaddr;
    struct sockaddr_in localaddr;
    socklen_t len;
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("socket error");
    }
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(DNS_PORT);
    char *servIP = (char *)DNS_ADDR;
    inet_pton(AF_INET, servIP, &servaddr.sin_addr);
    if (connect(fd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        perror("connect error");
    }
    socklen_t l = INET_ADDRSTRLEN;

    getsockname(fd, (struct sockaddr *) &localaddr, &l);
    ip = inet_ntoa(localaddr.sin_addr);
    close(fd);
    return string(ip);
};

string GetMyHostname() {
    char hostname[MAXHOST];
    int hostreturn = gethostname(hostname, sizeof(hostname));
    if (hostreturn == -1) {
        perror("host name error");
    }
    return string(hostname);
};
string ip;
string hostname;
int port;
int server_socket;
int head_socket;
int selret;
int sock_index;
socklen_t caddr_len;
struct sockaddr_in server_addr;
struct sockaddr_in client_addr;
fd_set master_list;
fd_set watch_list;


void server(int portNumber) {
    port = portNumber;
    hostname = GetMyHostname();
    ip = GetMyIP();
    caddr_len = sizeof(client_addr);
}

void RunServer() {
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
        perror("socket failed");
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr((char *) ip.data());
    server_addr.sin_port = htons(port);
    if (bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        printf("Bind failed!");
        exit(0);
    }
    if (listen(server_socket, 10) < 0) {
        printf("Listen socket error");
    }
    FD_ZERO(&master_list);
    FD_ZERO(&watch_list);
    FD_SET(server_socket, &master_list);
    FD_SET(STDIN, &master_list);
    head_socket = server_socket;
    struct info newClient;
    while (1) {
        memcpy(&watch_list, &master_list, sizeof(master_list));
        bzero(&client_addr, sizeof(client_addr));
        selret = select(head_socket + 1, &watch_list, NULL, NULL, NULL);
        if (selret < 0)
            perror("Select error");
        if (selret > 0) {
            for (sock_index = 0; sock_index <= head_socket; sock_index++) {

                if (FD_ISSET(sock_index, &watch_list)) {
                    if (sock_index == 0) {
                        string cmdLine;
                        getline(cin, cmdLine);
                        if (cmdLine.size() == 0) {
                            exit(-1);
                        } else {
                            const char *sep = " ";
                            char *p;
                            p = strtok((char *) cmdLine.data(), sep);
                            char *cmd = p;
                            vector<char *> params;
                            //read other params
                            while (p) {
                                params.push_back(p);
                                p = strtok(NULL, sep);
                            }
                            if (strcmp(cmd, "AUTHOR") == 0) {
                                //Author();
                                char const *cmd = "AUTHOR";
                                cse4589_print_and_log("[%s:SUCCESS]\n", cmd);
                                cse4589_print_and_log("I, %s, have read and understood the course academic integrity policy.\n", "yeshipal");
                                cse4589_print_and_log("[%s:END]\n", cmd);
                            } else if (strcmp(cmd, "IP") == 0) {
                                //Ip();
                                char const *cmd = "IP";
                                cse4589_print_and_log("[%s:SUCCESS]\n", cmd);
                                cse4589_print_and_log("IP:%s\n", ip.c_str());
                                cse4589_print_and_log("[%s:END]\n", cmd);
                            } else if (strcmp(cmd, "PORT") == 0) {
                                //Port();
                                char const *cmd = "PORT";
                                cse4589_print_and_log("[%s:SUCCESS]\n", cmd);
                                cse4589_print_and_log("PORT:%d\n", port);
                                cse4589_print_and_log("[%s:END]\n", cmd);
                            } else {
                                perror("Unexpected command");
                            }
                        }
                    }
                    
            }
        }
    }
}
}