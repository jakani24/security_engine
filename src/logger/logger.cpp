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
		va_list list;
		char buf[500];
		char time[300];
		if((fp=fopen(path_to_file,"a"))==0)
			return 1;
		else
		{
			va_start(list,num_of_args);
			get_time(time,295);
			time[strlen(time)-1]='\0';
			fprintf(fp,"%s\t",time);
			switch(lvl)
			{
				case 1:
					fprintf(fp,"INFORMATION\t");
				break;
				case 2:
					fprintf(fp,"ERROR\t\t");
				break;
				case 3:
					fprintf(fp,"WARNING\t\t");
				break;
			}
			for(int i=0;i<num_of_args;i++)
			{
				strcpy(buf,va_arg(list,char*));
				fprintf(fp,"%s",buf);
			}
			va_end(list);
			fprintf(fp,"\n");
			fclose(fp);
		}
	}
}
#endif