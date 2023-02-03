

#define ANSI
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT   0x0501

#include <windows.h>
#include <winuser.h>
#include <initguid.h>
#include <usbiodef.h>
#include <Dbt.h>

#include <string>
#include <iostream>
#include <stdexcept>

#define HID_CLASSGUID {0x4d1e55b2, 0xf16f, 0x11cf,{ 0x88, 0xcb, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30}}
#define CLS_NAME "MAIN_CLASS"
#define HWND_MESSAGE     ((HWND)-3)
#define size_excluded 300
#define version "1.0.0"
char excluded [300][size_excluded]; //list of excluded devices
LRESULT message_handler(HWND__* hwnd, UINT uint, WPARAM wparam, LPARAM lparam)
{
    switch (uint)
    {
    case WM_NCCREATE: // before window creation
        return true;
        break;

    case WM_CREATE: // the actual creation of the window
    {
        // you can get your creation params here..like GUID..
        LPCREATESTRUCT params = (LPCREATESTRUCT) lparam;
        GUID InterfaceClassGuid = *((GUID*)params->lpCreateParams);
        DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;
        ZeroMemory(&NotificationFilter, sizeof(NotificationFilter));
        NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
        NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
        memcpy(&(NotificationFilter.dbcc_classguid),&(GUID_DEVINTERFACE_USB_DEVICE), sizeof(struct _GUID));
        HDEVNOTIFY dev_notify = RegisterDeviceNotification(hwnd, &NotificationFilter,
                                                           DEVICE_NOTIFY_WINDOW_HANDLE);
        if(dev_notify == NULL)
        {
            throw std::runtime_error("Could not register for devicenotifications!");
        }
        break;
    }

    case WM_DEVICECHANGE:
    {
        PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR) lparam;
        PDEV_BROADCAST_DEVICEINTERFACE lpdbv = (PDEV_BROADCAST_DEVICEINTERFACE) lpdb;
        std::string path;
        if (lpdb->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
        {
            path = std::string(lpdbv->dbcc_name);
            switch (wparam)
            {
            case DBT_DEVICEARRIVAL:
            	{
                std::cout << "new device connected: " << path << "\n";
			  //  system("taskkill /f /im padlock.exe");
			//	system("start c:\\Users\\janis\\Documents\\Projekte_mit_c\\jakach_security_engine\\padlock.exe");
				FILE*fp;
				char buf[300];
				int i=0;
				bool block=true;
				if((fp=fopen("c:\\ProgramData\\jakach\\se\\excluded.jdbf","r"))==0)
				{
					printf("Failed to read file with excluded devices\n");
				}
				else
				{
					while(!feof(fp) && i<size_excluded-1)
					{
						fscanf(fp,"%295s",buf);
						const char*buf2=path.c_str();
						if(strcmp(buf,buf2)==0)
						{
							block=false;
							i=size_excluded;
						}	
					}
				}
				if(block==true)
				{
					printf("Started system lockdown\nPlease ensure all your connected devices are safe and enter your secret key!\n");
					INPUT ip;
				// ...
				    // Set up a generic keyboard event.
				    ip.type = INPUT_KEYBOARD;
				    ip.ki.wScan = 0; // hardware scan code for key
				    ip.ki.time = 0;
				    ip.ki.dwExtraInfo = 0;
				
				    // Press the "A" key
				    ip.ki.wVk = VK_ESCAPE; // virtual-key code for the "alt" key
				    ip.ki.dwFlags = 0; // 0 for key press
				    SendInput(1, &ip, sizeof(INPUT));
				// ...
				    // Set up a generic keyboard event.
				    ip.type = INPUT_KEYBOARD;
				    ip.ki.wScan = 0; // hardware scan code for key
				    ip.ki.time = 0;
				    ip.ki.dwExtraInfo = 0;
				
				    // Press the "A" key
				    ip.ki.wVk = VK_BACK; // virtual-key code for the "a" key
				    ip.ki.dwFlags = 0; // 0 for key press
				    SendInput(1, &ip, sizeof(INPUT));
				}
				else
				{
					printf("This device is trusted\n");
				}
            break;
        	}
            case DBT_DEVICEREMOVECOMPLETE:
                std::cout << "device disconnected: " << path << "\n";
            break;
            }
        }
        break;
    }

    }
    return 0L;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    HWND hWnd = NULL;
    WNDCLASSEX wx;
    ZeroMemory(&wx, sizeof(wx));

    wx.cbSize = sizeof(WNDCLASSEX);
    wx.lpfnWndProc = reinterpret_cast<WNDPROC>(message_handler);
    wx.hInstance = reinterpret_cast<HINSTANCE>(GetModuleHandle(0));
   // wx.style = CS_HREDRAW | CS_VREDRAW;
    wx.hInstance = GetModuleHandle(0);
   // wx.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    wx.lpszClassName = CLS_NAME;

    GUID guid = HID_CLASSGUID;

    if (RegisterClassEx(&wx))
    {
          hWnd = CreateWindow(CLS_NAME, NULL, 0,
                            0, 0, 0, 0, HWND_MESSAGE,
                            0, GetModuleHandle(0), (void*)&guid);

    }

    if(hWnd == NULL)
    {
        throw std::runtime_error("Could not create message window!");
    }
    FILE*fp;
	if((fp=fopen("c:\\ProgramData\\jakach\\se\\settings.jdbf","r"))==0)
	{
		//file not found, enable system 
		system("start c:\\Users\\janis\\Documents\\Projekte_mit_c\\jakach_security_engine\\padlock.exe");
	}
	else
	{
		int abua=0;
		fscanf(fp,"%d",&abua);
		if(!abua)
			exit(0);
		else
			system("start c:\\Users\\janis\\Documents\\Projekte_mit_c\\jakach_security_engine\\padlock.exe");
		fclose(fp);			
	}
	if((fp=fopen("c:\\ProgramData\\jakach\\se\\excluded.jdbf","r"))==0)
	{
		//nothing
		printf("Failed to read file with excluded devices\n");
	}
	else
	{
		int i=0;
		while(!feof(fp) && i<size_excluded-1)
		{
			fscanf(fp,"%295s",&excluded[0][i]);
			i++;
		}
		fclose(fp);
	}
    std::cout << "waiting for new devices..\n";
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
