#ifndef MODULE_CPP
#define MODULE_CPP
void usb_settings();
void av_settings();
void module_settings()
{
	char in;
	system("cls");
	printf("Module id\t\tModule name\n");
	printf("[0]\t\t\tAnti bad usb\n");
	printf("[1]\t\t\tAnti virus\n");
	printf("Please choose the module you want to edit\n");
	in=getch();
	switch(in)
	{
		case '0':
			usb_settings();
		break;
		case '1':
			av_settings();
		break;
	}
}
void av_settings()
{
	char in;
	char id[300];
	FILE*fp;
	printf("ID\t\t\tname\n");
	printf("[0]\t\t\tAdd path to scan\n");
	printf("[1]\t\t\tAdd VT api token\n");
	printf("[2]\t\t\tchange maximum of attempts to connect to VT\n");
	in=getch();
	switch(in)
	{
		case '0':
			if((fp=fopen("c:\\programdata\\jakach\\se\\folders.jdbf","a"))==0)
				printf("FATALE ERROR\n");
			else
			{
				printf("Please enter the path of the folder: (eg: \"c:\\users\\janis\\downloads\\\")\n");
				scanf("%395s",&id);
				fprintf(fp,"%s\n",id);
				printf("Please enter the name of the folder: (eg \"downloads\")\n");
				scanf("%395s",&id);
				fprintf(fp,"%s\n",id);
				fclose(fp);
			}
		break;
		case '1':
			printf("Please enter your api key\n");
			scanf("%295s",&id);
			if((strcmp(id,"!exit"))!=0)
			{
				if((fp=fopen("c:\\programdata\\jakach\\se\\api.jdbf","w"))==0)
					printf("FATALE ERROR\n");
				else
				{
					fprintf(fp,"%s",&id);
					fclose(fp);
					printf("done\n");
				}
			}			
		break;
		case '2':
			printf("Please enter the maximum of attempts before timeout\n");
			scanf("%d",&in);
			if((fp=fopen("c:\\programdata\\jakach\\se\\max.jdbf","w"))==0)
				printf("FATALE ERROR\n");
			else
			{
				fprintf(fp,"%d",in);
				fclose(fp);
				printf("done\n");
			}			
		break;
	}
}
void usb_settings()
{
	char in;
	char id[300];
	printf("ID\t\t\tname\n");
	printf("[0]\t\t\tAdd excluded device\n");
	in=getch();
	switch(in)
	{
		case '0':
			printf("Please enter the usb id of the device: (enter !exit to exit without entering an id)\n");
			scanf("%295s",&id);
			if((strcmp(id,"!exit"))!=0)
			{
				FILE*fp;
				if((fp=fopen("c:\\programdata\\jakach\\se\\excluded.jdbf","a"))==0)
					printf("FATALE ERROR\n");
				else
				{
					fprintf(fp,"%s\n",&id);
					fclose(fp);
					printf("done\n");
				}
			}
		break;
	}
}
#endif