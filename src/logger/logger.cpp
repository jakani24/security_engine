#ifndef LOGGER_CPP
#define LOGGER_CPP
#include <time.h>
#include <stdio.h>
#include <string>
#include <cstdarg>
namespace logger
{
	void get_time(char*out,int max)
	{
		struct tm*ptr;
		time_t lt;
		lt=time(NULL);
		ptr=localtime(&lt);
		strcpy_s(out,max,asctime(ptr));		
	}
	int log(int lvl,char*path_to_file,int num_of_args,...)
	{
		FILE*fp;
		FILE*inf;
		FILE*err;
		FILE*warn;
		FILE*set;
		
		va_list list;
		char buf[500];
		char time[300];
		char path[400];
		strcpy_s(path,295,path_to_file);
		strcat(path,"ALL_LOGS.jdbf");
		if((fp=fopen(path,"a"))==0)
			return 1;
			
		strcpy_s(path,295,path_to_file);
		strcat(path,"INFORMATION.jdbf");
		if((inf=fopen(path,"a"))==0)
			return 2;

		strcpy_s(path,295,path_to_file);
		strcat(path,"ERROR.jdbf");
		if((err=fopen(path,"a"))==0)
			return 3;
			
		strcpy_s(path,295,path_to_file);
		strcat(path,"WARNING.jdbf");
		if((warn=fopen(path,"a"))==0)
			return 4;
			
		strcpy_s(path,295,path_to_file);
		strcat(path,"SETTING.jdbf");
		if((set=fopen(path,"a"))==0)
			return 5;

		va_start(list,num_of_args);
		get_time(time,295);
		time[strlen(time)-1]='\0';
		fprintf(fp,"%s\t",time);
		switch(lvl)
		{
			case 1:
				fprintf(inf,"%s\t",time);
			break;
			case 2:
				fprintf(err,"%s\t",time);
			break;
			case 3:
				fprintf(warn,"%s\t",time);
			break;
			case 4:
				fprintf(set,"%s\t",time);
			break;
		}
		switch(lvl)
		{
			case 1:
				fprintf(fp,"INFORMATION\t");
				fprintf(inf,"INFORMATION\t");
			break;
			case 2:
				fprintf(fp,"ERROR\t\t");
				fprintf(err,"ERROR\t\t");
			break;
			case 3:
				fprintf(fp,"WARNING\t\t");
				fprintf(warn,"WARNING\t\t");
			break;
			case 4:
				fprintf(fp,"SETTING\t\t");
				fprintf(set,"SETTING\t\t");
			break;
		}
		for(int i=0;i<num_of_args;i++)
		{
			strcpy(buf,va_arg(list,char*));
			fprintf(fp,"%s",buf);
			switch(lvl)
			{
				case 1:
					fprintf(inf,"%s",buf);
				break;
				case 2:
					fprintf(err,"%s",buf);
				break;
				case 3:
					fprintf(warn,"%s",buf);
				break;
				case 4:
					fprintf(set,"%s",buf);
				break;
			}
		}
		va_end(list);
		fprintf(fp,"\n");
		switch(lvl)
		{
			case 1:
				fprintf(inf,"\n");
			break;
			case 2:
				fprintf(err,"\n");
			break;
			case 3:
				fprintf(warn,"\n");
			break;
			case 4:
				fprintf(set,"\n");
			break;
		}	
		fclose(fp);	
		fclose(inf);		
		fclose(err);		
		fclose(warn);
		fclose(set);			
		
	}
}
#endif