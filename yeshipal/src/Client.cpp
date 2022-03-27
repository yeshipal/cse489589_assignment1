
#include <iostream>
#include <string>
#include <map>

#include "../include/Client.h"
#define DNS_ADDR "8.8.8.8"
#define DNS_PORT 53
#define MAXHOST 255
#define STDIN 0
using namespace std;
int clientport;
string clientip;
string clienthostname;
int clientfd;
enum clientStatus status;
struct sockaddr_in client_addr;
string GetmyHostname() {
    char hostname[MAXHOST];
    int hostreturn = gethostname(hostname, sizeof(hostname));
    if (hostreturn == -1) {
        perror("host name error");
    }
    return string(hostname);
};
string GetmyIP() {
    char *ip;
    int fd;
    struct sockaddr_in servaddr;
    struct sockaddr_in localaddr;
    socklen_t len;
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("socket failed");
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
void client(int portNumber) {
    clientport = portNumber;
    clientfd = 0;
    status = OFFLINE;
    //get hostname and ip address
    clienthostname = GetmyHostname();
    cout << clienthostname << endl;

    clientip = GetmyIP();
    bzero(&client_addr, sizeof(client_addr));
}

void RunClient() {
    int head_socket, selret, sock_index = 0;
    fd_set master_list, watch_list;
    FD_ZERO(&master_list);
    FD_ZERO(&watch_list);
    FD_SET(STDIN, &master_list);

    head_socket = 0;
    char *ubit = "yeshipal"
    while (1) {
        memcpy(&watch_list, &master_list, sizeof(master_list));

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
                            exit(1);
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
                                        char const *cmd = "AUTHOR";
                                        cse4589_print_and_log("[%s:SUCCESS]\n", cmd);
                                        cse4589_print_and_log("I, %s, have read and understood the course academic integrity policy.\n", ubit);
                                        cse4589_print_and_log("[%s:END]\n", cmd);
                                    } else if (strcmp(cmd, "IP") == 0) {
                                        char const *cmd = "IP";
                                        cse4589_print_and_log("[%s:SUCCESS]\n", cmd);
                                        cse4589_print_and_log("IP:%s\n", clientip.c_str());
                                        cse4589_print_and_log("[%s:END]\n", cmd);
                                    } else if (strcmp(cmd, "PORT") == 0) {
                                        char const *cmd = "PORT";
                                        cse4589_print_and_log("[%s:SUCCESS]\n", cmd);
                                        cse4589_print_and_log("PORT:%d\n", clientport);
                                        cse4589_print_and_log("[%s:END]\n", cmd);
                                    } else if (strcmp(cmd, "EXIT") == 0) {
                                        if (status == LOGIN) {
                                            close(clientfd);
                                        }
                                        char const *cmd = "EXIT";
                                        cse4589_print_and_log("[%s:SUCCESS]\n", cmd);
                                        cse4589_print_and_log("[%s:END]\n", cmd);
                                        exit(0);
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