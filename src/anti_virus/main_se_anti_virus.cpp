#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string>
#include <windows.h>
#define dir "c:\\users\\janis\\downloads\\"
#define dirname "downloads"
#define version "1.0.1"
char api_key[300];
int max=6;//maximum of attempts before timeout
#include "scanner/scanner.cpp"
void update_index(char*,char*);
void check_index(char*,char*);
void scan_single(char*);
int main(int argc, char* argv[])
{
	if(argc>=2 && strcmp(argv[1],"--scan")==0)
	{
		if(argc>=4 && strcmp(argv[3],"--level")==0)
		{
			max=atoi(argv[4]);
		}
		scan_file("",argv[2]);
	}
	else
	{
	    FILE*fp;
	    int ava,abua,folders;
	    char folderpath[300];
	    char foldername[300];
	    if((fp=fopen("c:\\programdata\\jakach\\se\\api.jdbf","r"))==0)
	    {
	    	MessageBox(NULL,"Fatale error; could not read api key;","ERROR",MB_OK);
	    	exit(0);
		}
		else
		{
			fscanf(fp,"%295s",&api_key);
			fclose(fp);
		}
		if((fp=fopen("c:\\ProgramData\\jakach\\se\\max.jdbf","r"))==0)
		{
			//file not found, max=6; 
		}
		else
		{
			fscanf(fp,"%d",&max);
			fclose(fp);
		}
		if((fp=fopen("c:\\ProgramData\\jakach\\se\\settings.jdbf","r"))==0)
		{
			//file not found, enable system 
		}
		else
		{
			int abua=0;
			fscanf(fp,"%d",&abua);
			fscanf(fp,"%d",&ava);
			if(!ava)
				exit(0);
			fclose(fp);	
			if((fp=fopen("c:\\programdata\\jakach\\se\\folders.jdbf","r"))==0)
				folders=0;
			else
				folders=1;
		}
		while(true)
		{
			if(folders==0)
			{
				printf("Checking for new files\n");
				check_index(dir,dirname);
				printf("updating index\n");
				update_index(dir,dirname);
				printf("timeout\n");
			}
			else
			{
				while(!feof(fp))
				{
					fgets(folderpath,295,fp);
					fgets(foldername,295,fp);
					if(!feof(fp))
					{
						folderpath[strlen(folderpath)-1]='\0';
						foldername[strlen(foldername)-1]='\0';
						printf("Checking for new files on %s\n",folderpath);
						check_index(folderpath,foldername);
						printf("updating index\n");
						update_index(folderpath,foldername);
						printf("timeout\n");
					}				
				}
				rewind(fp);
			}
			sleep(5);
		}
	}
  return 0;
}


void update_index(char*folder,char*fname)
{
	FILE*fp;
	char path[300];
	sprintf(path,"c:\\ProgramData\\jakach\\se\\folders\\%s.jdbf",fname);
	if((fp=fopen(path,"w"))==0)
	{
		MessageBox(NULL,"[Jakach_security_engine] FATALE: couldn't make index","ERROR",MB_OK);
		printf("FATALE ERROR\n");
	}
	else
	{
		  DIR *dp;
		  struct dirent *ep;
		  char a[300];
		  dp = opendir (folder);
		  if (dp != NULL)
		    {
		      while (ep = readdir (dp))
		      {
		      		strcpy(a,ep->d_name);
		     	//	puts(ep->d_name);
		     		if((strcmp(a,".."))!=0 && (strcmp(a,"."))!=0)
		     			fprintf(fp,"%s\n",a);
			  }
		      (void) closedir (dp);
		    }
		  else
		    perror ("Couldn't open the directory");
		fclose(fp);
	}
}
void check_index(char*folder,char*fname)
{
	FILE*fp;
	bool scan=true;
	char path[300];
	sprintf(path,"c:\\ProgramData\\jakach\\se\\folders\\%s.jdbf",fname);
	if((fp=fopen(path,"r"))==0)
	{
		MessageBox(NULL,"[Jakach_security_engine] FATALE: couldn't open index","ERROR",MB_OK);
		printf("FATALE ERROR\n");
	}
	else
	{
		  DIR *dp;
		  struct dirent *ep;
		  char a[300];
		  char buf[300];
		  char cmd[500];
		  int wt_secure=0;
		  bool wt_scan=true;
		  dp = opendir (folder);
		  if (dp != NULL)
		    {
		      while (ep = readdir (dp))
		      {
		      		strcpy(a,ep->d_name);
		     		if((strcmp(a,".."))!=0 && (strcmp(a,"."))!=0)
					{
						rewind(fp);
						scan=true;
						while(!feof(fp))
						{
							fgets(buf,295,fp);
							buf[strlen(buf)-1]='\0';
							if((strcmp(a,buf))==0)
								scan=false;
						}
						if(scan)
						{
							scan_file(folder,a);
						}	
					}
			  }
		      (void) closedir (dp);
		    }
		  else
		    perror ("Couldn't open the directory");
		fclose(fp);
	}	
}

