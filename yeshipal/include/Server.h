#include <string>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <strings.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <vector>
#include <algorithm>
#include <functional>

#include "../include/logger.h"
#include "../include/global.h"

#define STDIN 0
#define TRUE 1


using namespace std;
void server(int portNumber);
void RunServer();
