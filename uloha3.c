#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#define DEFAULT_BUFLEN 4096
#pragma comment(lib, "Ws2_32.lib")

// gcc -o trial.exe main.c -lws2_32
//za cmd


void print_log(char *message,int mode) //pre ulohu 13, musim zachovat nas chat
{
    FILE *fp;
    fp = fopen("log.txt", "a");// mode a, append, pridat do subora
    if(mode==1)
    {
        fprintf(fp, "Me: %s\n",message);
    }
    else if(mode==2)
    {
        fprintf(fp,"Morpheus: %s\n",message);
    }
    fclose(fp);
}

char* replace_char(char* str, char find, char replace) //menim poziciu \n a \0 aby bolo zarovnane
{
    char *current_pos = strchr(str, find);
    while (current_pos)
    {
        *current_pos = replace;
        current_pos = strchr(current_pos, find);
    }
    return str;
}

void gotoxy(int x, int y) //menim koordinaty, pre vypis na pravu stranu
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int find_break(int col,int i,char *message) //hladam space, aby sa slova nerozdelovali
{
    if(strlen(message)-i<(col/2-1))
    {
        return -1;
    }

    char blank=' ';

    for(int j=i-1+col/2;j>i;j--)
    {
        if(message[j] == blank)
            return j;
    }
    return -1;
}

void print_sentence(char *message)//moje odpovede
{
    fflush(stdout);
    int counter=0;

    for(int i=0;i<strlen(message);i++)
    {
        if(message[i]=='\0')
            return;
        printf("%c",message[i]);
        counter+=1;
    }

    printf("\n\n");
    fflush(stdout);
    print_log(message,1);
}


void print_sentence_right(char *message)//morpheusove odpovede
{
    fflush(stdout);

    int counter=0;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int ret,breakpoint,rows,columns;

    ret = GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    gotoxy(csbi.dwCursorPosition.X+(columns/2),csbi.dwCursorPosition.Y);
    breakpoint = find_break(columns,0,message);

    for(int i=0;i<strlen(message);i++)
    {
        if(message[i]=='\0')
            return;
        if(counter+1==columns/2 || i == breakpoint)
        {
            counter=0;

            breakpoint = find_break(columns,i,message);
            gotoxy(columns/2,csbi.dwCursorPosition.Y+1);
            ret = GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&csbi);

            continue;
        }
        printf("%c",message[i]);
        counter+=1;
    }
    printf("\n\n");
    fflush(stdout);
    print_log(message,2);
}

int isPrime(int n) // pre ulohu 12
{
    if(n==1||n==0)
        return 0;
    int  i, flag = 0;
    for (i = 2; i <= n / 2; ++i)
    {
        if (n % i == 0)
        {
            flag = 1;
            break;
        }
    }
    if(flag==1)
        return 0;
    return 1;
}

void encryptDecrypt(char inpString[]) //8 uloha, dekodovat text podla xor s klucom 55
{
    int xorKey = 55;
    int len = 132;
    for (int i = 0; i < len; i++){
        inpString[i] = inpString[i] ^ xorKey;
    }
}

int main()
{
    system("cls");

    int indeX = 0;
    int indeY = 0;

    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD point = { indeX,indeY };
    SetConsoleCursorPosition(hConsole, point);
    SetConsoleOutputCP(CP_UTF8);
    WSADATA wsaData;

    int iResult;
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (iResult != 0)
    {
        printf("WSAStartup failed : %d\n", iResult);
        return 1;
    }

    struct addrinfo *result = NULL, *ptr = NULL;
    struct addrinfo hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    //adresa cez ktoru komunikujeme
    iResult = getaddrinfo("147.175.115.34", "777", &hints, &result);

    if (iResult != 0)     //kontrola, ci nenastala chyba
    {
        printf("getaddrinfo failed : %d\n", iResult);
        WSACleanup();
        return 1;
    }
    else
        printf("getaddrinfo didn’t fail…\n");


    SOCKET ConnectSocket = INVALID_SOCKET;
    ptr = result;

    //vytvorenie socketu
    ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

    //chyba socketu
    if (ConnectSocket == INVALID_SOCKET)
    {
        printf("Error at socket() : %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }
    else printf("No socket error\n");

    iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);

    if (iResult == SOCKET_ERROR)     //kontrola, ci nenastala chyba
        printf("Not connected to server…\n");
    else
        printf("Connected to server!\nPress Enter To start.");

    if (iResult == SOCKET_ERROR)    //osetrenie chyboveho stavu
    {
        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;
        WSACleanup();
        return 1;
    }

    Sleep(250);

    int size = 4096;
    char sendbuf[size]; //buffer (v zasade retazec), kam sa budu ukladat data, ktore chceme posielat

    fgets(sendbuf, size, stdin);

    strtok(sendbuf, "\n");

    iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
    if (iResult == SOCKET_ERROR)
    {
        printf("send failed : %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }
    printf("Bytes Sent : %ld\n", iResult);     //vypisanie poctu odoslanych dat

    //prijimanie

    int recvbuflen = DEFAULT_BUFLEN;
    char recvbuf[DEFAULT_BUFLEN];

    iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);     //funkcia na príjimanie

    if (iResult > 0)
        printf("Bytes received : %d\n", iResult);     //prisli validne data, vypis poctu
    else if (iResult == 0)
        printf("Connection closed\n");     //v tomto pripade server ukoncil komunikaciu
    else
        printf("recv failed with error : %d\n", WSAGetLastError());     //ina chyba

    //vypis morpheusa prvy krat

    char *ps;

    ps = replace_char(recvbuf,'\n','\0');
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);

    print_sentence_right(ps);
    SetConsoleOutputCP(CP_UTF8);

    //moje odpovede
    int c = 0;

    while (c < 100)
    {
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
        char* ps2;

        if (c == 0)
        {
            char id[] = "111344\0";
            SetConsoleOutputCP(CP_UTF8);
            char *name = "Macek Marko";
            print_sentence(name);
            print_sentence("111344\0");
            iResult = send(ConnectSocket, id, (int)strlen(id), 0);
        }
        else if (c == 1)
        {
            print_sentence("Enter\0");
            char enter[] = "Enter\0";
            iResult = send(ConnectSocket, enter, (int)strlen(enter), 0);
        }
        else if (c == 2)
        {
            char response[] = "7545477\0";
            print_sentence("7545477\0");
            iResult = send(ConnectSocket, response, (int)strlen(response), 0);
        }
        else if (c == 3)
        {
            char response[] = "753421\0";
            print_sentence("754321\0");
            iResult = send(ConnectSocket, response, (int)strlen(response), 0);
        }
        else if (c == 4)
        {
            char response[] = "2\0";
            print_sentence("2\0");
            iResult = send(ConnectSocket, response, (int)strlen(response), 0);
        }
        else if (c == 5)
        {
            char response[] = "333222111\0";
            print_sentence("333222111\0");
            iResult = send(ConnectSocket, response,(int)strlen(response), 0);
        }
        else if (c == 6)
        {
            char response[] = "123\0";
            print_sentence("123\0");
            iResult = send(ConnectSocket, response, (int)strlen(response), 0);
        }
        else if (c == 8)
        {
            char response[]="48\0";
            print_sentence("48\0");
            iResult = send(ConnectSocket, response, (int)strlen(response), 0);
        }
        else if (c == 9)
        {
            char response[]="2\0";
            print_sentence("2\0");
            iResult = send(ConnectSocket, response, (int)strlen(response), 0);
        }
        else if (c == 10)
        {
            char response[]="E.T.\0";
            print_sentence("E.T.\0");
            iResult = send(ConnectSocket, response, (int)strlen(response), 0);
        }
        else if (c == 11)
        {
            char response[]="PRIMENUMBER\0";
            print_sentence("PRIMENUMBER\0");
            iResult = send(ConnectSocket, response, (int)strlen(response), 0);
        }
        else if (c == 12)
        {
            char response[DEFAULT_BUFLEN];
            int counter = 0;
            for(int i=0;i<strlen(ps2);i++)
            {
                if(isPrime(i+1))
                {
                    response[counter++] = ps2[i];
                }
                if(ps2[i]=='\0')
                    break;
            }
            response[counter++]='\0';
            print_sentence(response);
            iResult = send(ConnectSocket, response, (int)strlen(response), 0);
        }
        else if (c == 13)
        {
            char response[]="Trinity\0";
            print_sentence("Trinity\0");
            iResult = send(ConnectSocket, response, (int)strlen(response), 0);
        }
        else if (c == 14)
        {
            char response[]="DMA\0";
            print_sentence("DMA\0");
            iResult = send(ConnectSocket, response, (int)strlen(response), 0);
        }
        else if (c == 15)
        {
            char response[]="NIE\0";
            print_sentence("NIE\0");
            iResult = send(ConnectSocket, response, (int)strlen(response), 0);
        }
        else if(c==16)
        {
            exit(0);
        }
        else
            {
            print_sentence("Enter\0");
            char enter[] = "Enter\0";
            iResult = send(ConnectSocket, enter, (int)strlen(enter), 0);
        }

        int recvbuflen2 = DEFAULT_BUFLEN;
        char recvbuf2[DEFAULT_BUFLEN];

        //odpovede od morpheusa

        iResult = recv(ConnectSocket, recvbuf2, recvbuflen2, 0);     //funkcia na príjimanie

        ps2 = replace_char(recvbuf2, '\n', '\0');
        print_sentence_right(ps2); // pre server

        if(c==6)
        {
            encryptDecrypt(ps2);
        }

        c++;
    }

    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}

