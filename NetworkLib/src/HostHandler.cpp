/*
** EPITECH PROJECT, 2023
** R-Type
** File description:
** HostHandler
*/

#include "HostHandler.hpp"

#include <cstring>

#include "Error.hpp"

#ifdef WIN32
#include <winsock2.h>
#include <ws2tcpip.h>

#elif __linux__
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

#endif

Address::Ip HostHandler::getIp(std::string_view hostname)
{
    struct addrinfo     hints;
    struct addrinfo*    res;
    struct sockaddr_in* addr;
    Address::Ip         ip = 0;

    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    int status = getaddrinfo(hostname.data(), nullptr, &hints, &res);
    if (status != 0) {
        std::string error = "Impossible to init the client : Error in getaddrinfo " + std::string(gai_strerror(status));
        throw InitError(error);
    }
    for (auto it = res; it != NULL; it = it->ai_next) {
        addr = reinterpret_cast<struct sockaddr_in*>(it->ai_addr);
        ip   = ntohl(addr->sin_addr.s_addr);
        if (ip != 0) {
            freeaddrinfo(res);
            return (ip);
        }
    }
    return (ip);
}

Address HostHandler::getHostAddress(int ac, const char* const av[])
{
    Address serverInfos;

    if (ac != 4) throw Error("Usage: ./r-type_client [server port] [server ip] [client port]");

    serverInfos.port = std::atoi(av[1]);
    if (serverInfos.port <= 0) throw Error("Port must be a positive number");

    serverInfos.ip = getIp(av[2]);
    if (serverInfos.ip <= 0) throw Error("No server found with this IP address");

    return (serverInfos);
}

Address::Port HostHandler::getClientPort(int ac, const char* const av[])
{
    Address::Port clientPort = std::atoi(av[3]);
    if (clientPort <= 0) throw Error("Port must be a positive number");

    return (clientPort);
}