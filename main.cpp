#include <iostream>
#include <windows.h>
#include <conio.h>

using namespace std;

int laste1 = 0, laste2 = 0;

int sendread(HANDLE hSerial, uint8_t l)
{
    uint8_t* ch = &l;
    DWORD dwBytesRead = 0;
    uint8_t low;
    uint8_t high;
    uint16_t enkoder1, enkoder2, bateria, dalmierz;
    if(!WriteFile(hSerial, ch, 1, &dwBytesRead, NULL))
    {
        cout << "error writing" << endl;
    }
    dwBytesRead = 0;
    if (l == 3)
    {
        if(!ReadFile(hSerial, &low, 1, &dwBytesRead, NULL))
        {
            cout << "error reading" << endl;
        }
        if(!ReadFile(hSerial, &high, 1, &dwBytesRead, NULL))
        {
            cout << "error reading" << endl;
        }
        dalmierz = low | (high << 8);
        cout << "Dalmierz: " << dalmierz << endl;
    }
    else if (l == 4)
    {
        if(!ReadFile(hSerial, &low, 1, &dwBytesRead, NULL))
        {
            cout << "error reading" << endl;
        }
        if(!ReadFile(hSerial, &high, 1, &dwBytesRead, NULL))
        {
            cout << "error reading" << endl;
        }
        bateria = low | (high << 8);
        cout << "Bateria: " << bateria << endl;
    }
    else if (l == 5)
    {
        if(!ReadFile(hSerial, &low, 1, &dwBytesRead, NULL))
        {
            cout << "error reading" << endl;
        }
        if(!ReadFile(hSerial, &high, 1, &dwBytesRead, NULL))
        {
            cout << "error reading" << endl;
        }
        enkoder1 = low | (high << 8);
        cout << "Enkoder1: " << enkoder1 << " ró¿nica: " << enkoder1 - laste1 << endl;
        laste1 = enkoder1;
    }
    else if (l == 6)
    {
        if(!ReadFile(hSerial, &low, 1, &dwBytesRead, NULL))
        {
            cout << "error reading" << endl;
        }
        if(!ReadFile(hSerial, &high, 1, &dwBytesRead, NULL))
        {
            cout << "error reading" << endl;
        }
        enkoder2 = low | (high << 8);
        cout << "Enkoder2: " << enkoder2 << " ró¿nica: " << enkoder2 - laste2 << endl;
        laste2 = enkoder2;
    }
}

int main()
{
    HANDLE hSerial;
    hSerial = CreateFile("COM9",
                         GENERIC_READ | GENERIC_WRITE,
                         0,
                         0,
                         OPEN_EXISTING,
                         FILE_ATTRIBUTE_NORMAL,
                         0);
    if(hSerial==INVALID_HANDLE_VALUE)
    {
        if(GetLastError()==ERROR_FILE_NOT_FOUND)
        {
            cout << "serial port doesnt exist" << endl;
        }
        cout << "error" << endl;
    }

    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength=sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams))
    {
        cout << "error getting state" << endl;
    }
    dcbSerialParams.BaudRate=CBR_9600;
    dcbSerialParams.ByteSize=8;
    dcbSerialParams.StopBits=ONESTOPBIT;
    dcbSerialParams.Parity=NOPARITY;
    if(!SetCommState(hSerial, &dcbSerialParams))
    {
        cout << "error setting serial port state" << endl;
    }

    COMMTIMEOUTS timeouts={0};
    timeouts.ReadIntervalTimeout=50;
    timeouts.ReadTotalTimeoutConstant=50;
    timeouts.ReadTotalTimeoutMultiplier=10;
    timeouts.WriteTotalTimeoutConstant=50;
    timeouts.WriteTotalTimeoutMultiplier=10;
    if(!SetCommTimeouts(hSerial, &timeouts))
    {
        cout << "error occureed" << endl;
    }

    char ch;
    uint8_t k=0;
    int l;
    int i = 0;
    uint16_t adc;
    DWORD dwBytesRead = 0;
    while (l != 99)
    {
        cin >> l;
        k = l;
        sendread(hSerial, k);
    }
    /*while (ch != 'q')
    {
        ch = getch();
        sendread(hSerial, k);
    }*/

    CloseHandle(hSerial);
    return 0;
}
