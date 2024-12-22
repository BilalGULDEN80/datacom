#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string.h> // Mesaj kontrolü için gerekli

#pragma comment(lib, "ws2_32.lib") // Winsock kütüphanesini baðlar

#define PORT 1453
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int addrLen = sizeof(clientAddr);
    char buffer[BUFFER_SIZE];

    // Winsock baþlatma
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed. Error: %d\n", WSAGetLastError());
        return 1;
    }

    // Sunucu soketi oluþturma
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        printf("Socket creation failed. Error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Sunucu adresi yapýlandýrma
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Soketi baðlama (bind)
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Baglanti Basarisiz Oldu!: %d\n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Baðlantýlarý dinleme
    if (listen(serverSocket, 3) == SOCKET_ERROR) {
        printf("Dinleme Hatasi: %d\n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    printf("Sunucu Portu Dinleniyor %d...\n", PORT);

    // Baðlantýyý kabul etme
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen);
    if (clientSocket == INVALID_SOCKET) {
        printf("Kabul Hatasi: %d\n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    printf("Connection established with client.\n");

    // Ýstemciden mesaj alma ve cevap verme
    int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0'; // Gelen mesajý null karakter ile sonlandýr
        printf("Ýstemci Mesaji: %s\n", buffer);

        // Gelen mesaja göre yanýt oluþturma
        char response[BUFFER_SIZE];
        if (strcmp(buffer, "merhaba") == 0) {
            strcpy(response, "hosgeldin");
        } else if (strcmp(buffer, "nasilsin?") == 0) {
            strcpy(response, "iyiyim, tesekkurler! Siz nasilsiniz?");
        } else {
            strcpy(response, "Anlamadým. Lütfen tekrar deneyin.");
        }

        // Yanýtý istemciye gönderme
        send(clientSocket, response, strlen(response), 0);
        printf("Sunucu: %s\n", response);
    }

    // Soketleri kapatma ve Winsock'u temizleme
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}

