#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string.h>

#pragma comment(lib, "ws2_32.lib")  // Linkar a biblioteca de sockets do Windows

int port = 88;
const char *site = "10.0.2.15";
const char *caractere = "pinto";

int sendChar(const char *caractere) {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server;

    // Inicializar Winsock
    std::cout << "Inicializando Winsock..." << std::endl;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cout << "Falha na inicialização do Winsock. Código: " << WSAGetLastError() << std::endl;
        return 1;
    }

    // Criar socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cout << "Falha ao criar socket. Código: " << WSAGetLastError() << std::endl;
        return 1;
    }
    std::cout << "Socket criado com sucesso." << std::endl;

    // Configurar o endereço do servidor
    server.sin_addr.s_addr = inet_addr(site);  // IP do servidor
    server.sin_family = AF_INET;
    server.sin_port = htons(port);  // Porta do servidor

    // Conectar ao servidor
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        std::cout << "Falha ao conectar ao servidor." << std::endl;
        return 1;
    }

    std::cout << "Conectado ao servidor." << std::endl;

    // Enviar uma solicitação simples ao servidor
    char message[256];
    sprintf(message, "POST / HTTP/1.1\r\nHost: %s\r\nContent-Type: text/plain\r\nContent-Length: %lu\r\n\r\n%s",
            site, strlen(caractere), caractere);

    if (send(sock, message, strlen(message), 0) < 0) {
        std::cout << "Falha ao enviar mensagem." << std::endl;
        return 1;
    }
    std::cout << "Solicitação enviada." << std::endl;

    // Receber resposta
    char buffer[2000];
    int response_len = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (response_len < 0) {
        std::cout << "Falha ao receber resposta." << std::endl;
    } else {
        buffer[response_len] = '\0';  // Terminar a string corretamente
        std::cout << "Resposta recebida: " << std::endl << buffer << std::endl;
    }

    // Fechar o socket
    closesocket(sock);
    WSACleanup();

    return 0;
}
