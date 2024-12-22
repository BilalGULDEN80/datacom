#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string.h> // Mesaj kontrol� i�in gerekli

#pragma comment(lib, "ws2_32.lib") // Winsock k�t�phanesini ba�lar

#define PORT 1453
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int addrLen = sizeof(clientAddr);
    char buffer[BUFFER_SIZE];

    // Winsock ba�latma
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed. Error: %d\n", WSAGetLastError());
        return 1;
    }

    // Sunucu soketi olu�turma
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        printf("Socket creation failed. Error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Sunucu adresi yap�land�rma
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Soketi ba�lama (bind)
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Baglanti Basarisiz Oldu!: %d\n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Ba�lant�lar� dinleme
    if (listen(serverSocket, 3) == SOCKET_ERROR) {
        printf("Dinleme Hatasi: %d\n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    printf("Sunucu Portu Dinleniyor %d...\n", PORT);

    // Ba�lant�y� kabul etme
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen);
    if (clientSocket == INVALID_SOCKET) {
        printf("Kabul Hatasi: %d\n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    printf("Connection established with client.\n");

    // �stemciden mesaj alma ve cevap verme
    int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0'; // Gelen mesaj� null karakter ile sonland�r
        printf("�stemci Mesaji: %s\n", buffer);

        // Gelen mesaja g�re yan�t olu�turma
        char response[BUFFER_SIZE];
        if (strcmp(buffer, "merhaba") == 0) {
            strcpy(response, "hosgeldin");
        } else if (strcmp(buffer, "nasilsin?") == 0) {
            strcpy(response, "iyiyim, tesekkurler! Siz nasilsiniz?");
        } else {
            strcpy(response, "Anlamad�m. L�tfen tekrar deneyin.");
        }

        // Yan�t� istemciye g�nderme
        send(clientSocket, response, strlen(response), 0);
        printf("Sunucu: %s\n", response);
    }

    // Soketleri kapatma ve Winsock'u temizleme
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}

