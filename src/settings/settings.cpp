#include <iostream>
#include <windows.h>
#include <conio.h>
 
#include "module_settings.cpp"
int main()
{
	int abua=0;
	int ava=0;
	int input=0;
	FILE*fp;
	while(true)
	{
		if((fp=fopen("c:\\ProgramData\\jakach\\se\\settings.jdbf","r"))==0)
		{
			printf("Error while opening file\n");
			system("pause");
			exit(0);
		}
		else
		{
			system("cls");
			fscanf(fp,"%d",&abua);
			fscanf(fp,"%d",&ava);
			fclose(fp);
			printf("Welcome to the Jakach security engine settings\n\n\n"); 
			printf("Module id\t\tModule name\t\tStatus\n");
			printf("[0]\t\t\tAnti bad usb\t\t%s\n",abua ? "ONLINE" : "OFFLINE");
			printf("[1]\t\t\tAnti virus\t\t%s\n",ava ? "ONLINE" : "OFFLINE");
			printf("\n\nTo change setting press module number on your keyboard\nPress x to exit; Press s to start all services; Press z for module specific settings\nChanges will be applied on next reboot\n");
			input=getch();
			switch(input)
			{
				case '0':
					if((fp=fopen("c:\\ProgramData\\jakach\\se\\settings.jdbf","w"))==0)
					{
						printf("Error while opening file\n");
					}
					else
					{
						fprintf(fp,"%d\n",!abua);
						fprintf(fp,"%d",ava);
						fclose(fp);
					}
				break;
				case '1':
					if((fp=fopen("c:\\ProgramData\\jakach\\se\\settings.jdbf","w"))==0)
					{
						printf("Error while opening file\n");
					}
					else
					{
						fprintf(fp,"%d\n",abua);
						fprintf(fp,"%d",!ava);
						fclose(fp);
					}
				break;
				case 'x':
					exit(0);
				break;
				case 'z':
					//specific settings
					module_settings();
				break;
				case 's':
					printf("starting\n");
					system("taskkill /f /im padlock.exe");
					system("taskkill /f /im usb.exe");
					system("taskkill /f /im se_anti_virus.exe");
					system("\"C:\\Users\\janis\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\start_av.bat\"");
					printf("done");
				break;
			}
		}
	}
	
}