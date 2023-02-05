/*
** EPITECH PROJECT, 2023
** R-Type
** File description:
** LinuxSocket
*/

#ifdef __linux__

#include "LinuxSocket.hpp"

#include "Error.hpp"

LinuxSocket::LinuxSocket(Address::Port port)
{
    socketFd_ = socket(AF_INET, SOCK_DGRAM, 0);

    if (socketFd_ < 1) { throw InitError("Socket initialization failed."); }

    // make the socket listen to a port passed in parameter
    address_.sin_port        = htons(port);
    address_.sin_addr.s_addr = htonl(INADDR_ANY);
    address_.sin_family      = AF_INET;

    if (bind(socketFd_, reinterpret_cast<sockaddr*>(&address_), sizeof(address_)) != 0)
        throw InitError("Socket binding to port failed.");

    receivedBuffer_.reserve(MAX_RECEIVED_BUFFER_SIZE);
}

LinuxSocket::~LinuxSocket() noexcept
{
    close(socketFd_);
}

ISocket::Fd LinuxSocket::getSocketFd() const noexcept
{
    return (socketFd_);
}

Address LinuxSocket::linuxAddressToAddress(sockaddr_in address) noexcept
{
    Address converted{ .port = ntohs(address.sin_port), .ip = ntohl(address.sin_addr.s_addr) };
    return converted;
}

sockaddr_in LinuxSocket::addressToLinuxAddress(Address address) noexcept
{
    sockaddr_in converted;
    converted.sin_family      = AF_INET;
    converted.sin_port        = htons(address.port);
    converted.sin_addr.s_addr = htonl(address.ip);
    return converted;
}

Address LinuxSocket::getAddress() const noexcept
{
    Address address = linuxAddressToAddress(address_);
    return address;
}

void LinuxSocket::send(const void* data, int data_size, Address destAddr) const
{
    sockaddr_in linuxDestAddr = addressToLinuxAddress(destAddr);

    int sent_bytes =
        sendto(socketFd_, data, data_size, 0, reinterpret_cast<sockaddr*>(&linuxDestAddr), sizeof(linuxDestAddr));

    if (sent_bytes < 0) { throw NetworkExecError("Error in sending data from the server to the client"); }
}

ReceivedInfos LinuxSocket::receive()
{
    sockaddr_in   address = { 0 };
    socklen_t     addrLen = sizeof(address);
    ReceivedInfos infos;

    int bytesReceived = recvfrom(socketFd_,
        reinterpret_cast<void*>(receivedBuffer_.data()),
        receivedBuffer_.capacity(),
        0,
        reinterpret_cast<sockaddr*>(&address),
        &addrLen);

    if (bytesReceived < 0) { throw NetworkExecError("Error receiving data from the client"); }

    infos.address = linuxAddressToAddress(address);
    infos.data.insert(infos.data.begin(), receivedBuffer_.data(), receivedBuffer_.data() + bytesReceived);
    return infos;
}

#endif
