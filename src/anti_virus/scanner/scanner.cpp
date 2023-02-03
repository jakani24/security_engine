#ifndef SCANNER_CPP
#define SCANNER_CPP
void upload_file(char*folder,char*file);
void scan_file(char*folder,char*file)//scan with hash
{
	printf("Found new file %s%s\n",folder,file);
	char cmd[300];
	char hash[300];
	char data[300];
	char total[50];
	char detected[50];
	int wt_secure;
	int wt_scan;
	int error=0;
	FILE*fp;
	//check if file allready exists in vt database
	sprintf(cmd,"certutil -hashfile \"%s%s\" > c:\\programdata\\jakach\\se\\hash.jdbf",folder,file);
	system(cmd);
	//calculated hash of file
	if((fp=fopen("c:\\programdata\\jakach\\se\\hash.jdbf","r"))==0)
	{
		//error
		error=1;
	}
	else
	{
		fgets(hash,295,fp); //first line with filename etc
		fgets(hash,295,fp); //second  line with hash
		hash[strlen(hash)-1]='\0';
		fclose(fp);
		//check on VT
		sprintf(cmd,"curl --request GET --url \"https://www.virustotal.com/vtapi/v2/file/report?apikey=%s&resource=%s\" > c:\\programdata\\jakach\\se\\out.jdbf",api_key,hash);
		//printf("%s\n",cmd);
		system(cmd);
		//printf("%s\n",cmd);
		if((fp=fopen("c:\\programdata\\jakach\\se\\out.jdbf","r"))==0)
		{
			error=2;
		}
		else
		{
			fscanf(fp,"%295s",&data);
			if(strstr(data,"response_code"))
			{
				//file does not exist, scan with upload
				printf("uploading unknown file to VT\n");
				upload_file(folder,file);
			}
			else
			{
				wt_secure=0;
				wt_scan=0;
				rewind(fp);
				while(!strstr(total,"total\"")&&wt_secure<=5000)
				{
					wt_secure++;
					fscanf(fp,"%45s",&total);
				}
				fscanf(fp,"%45s",&total);//total amount of av engines that scaned the file
				total[strlen(total)-1]='\0';//raw number
				if(wt_secure>4995)
					wt_scan++;
				wt_secure=0;
				rewind(fp);
				while(!strstr(detected,"positives")&&wt_secure<=5000)
				{
					wt_secure++;
					fscanf(fp,"%45s",&detected);
				}
				fscanf(fp,"%45s",&detected);//total amount of av engines that detected the file
				detected[strlen(detected)-1]='\0';//raw number
				if(wt_secure>4995)
					wt_scan++;
			//	printf("%s::testtesttesttest::%s::%d\n",detected,total,wt_secure);
				if(atoi(detected)!=0)
				{
					sprintf(data,"WARNING: The scanned file %s%s was detected by %d out of %d engines. Should the file be removed?",folder,file,atoi(detected),atoi(total));
					if(MessageBox(NULL,data,"WARNING",MB_YESNO|MB_ICONWARNING|MB_SYSTEMMODAL)==IDYES)
					{
						sprintf(data,"%s%s",folder,file);
						printf("removing %s\n",data);
						remove(data);
					}
				}
				else if(wt_scan==0)
					printf("No engine detected the file\n");
				if(wt_scan!=0)
				{
					sprintf(data,"WARNING: internal error while scanning file %s%s.",folder,file);
					MessageBox(NULL,data,"WARNING",MB_OK|MB_ICONWARNING|MB_SYSTEMMODAL);
				}
			}
		}
	}
	if(error)
	{
		sprintf(data,"WARNING: the file could not be scanned due to internal errors");
		MessageBox(NULL,data,"WARNING",MB_OK|MB_ICONWARNING|MB_SYSTEMMODAL);
	}
}

void upload_file(char*folder,char*file)
{
	char cmd[300];
	char data[300];
	char total[50];
	char detected[50];
	char scanid[300];
	int wt_secure;
	int error=0;
	int iterator=max;
	bool scan=true;
	bool answer=false;
	FILE*fp;
	
	//upload
	sprintf(cmd,"curl --request POST --url \"https://www.virustotal.com/vtapi/v2/file/scan\" --form \"apikey=%s\" --form \"file=@%s%s\" > c:\\programdata\\jakach\\se\\out.jdbf",api_key,folder,file);
	system(cmd);
	//printf("%s\n",cmd);
	//check for scanid
	if((fp=fopen("c:\\programdata\\jakach\\se\\out.jdbf","r"))==0)
	{
		error=1;
	}
	else
	{
		wt_secure=0;
		while(!strstr(scanid,"resource") && wt_secure<=10000)
		{
			wt_secure++;
			fscanf(fp,"%295s",&scanid);
		}
		if(wt_secure<=9995)
			error=2;
		fscanf(fp,"%295s",&scanid);//resource id
		fclose(fp);
		scanid[strlen(scanid)-1]='\0';
		//
		scanid[0]=' ';
		for(int c=0;c<strlen(scanid);c++)
			scanid[c]=scanid[c+1];
		scanid[strlen(scanid)-1]='\0';
		printf("resource:%s\n",scanid);
		//weve got the resource id now wait for analysis
	
		//download (we've already checked if the file exist so delay some time because scans tend to be very time intensive
		while(iterator>=0 && !answer)
		{
			iterator--;
			sleep(60);//delay
			//attempt to download
			sprintf(cmd,"curl --request GET --url \"https://www.virustotal.com/vtapi/v2/file/report?apikey=%s&resource=%s\" > c:\\programdata\\jakach\\se\\out.jdbf",api_key,scanid);
			system(cmd);
			//printf("%s\n",cmd);
			if((fp=fopen("c:\\programdata\\jakach\\se\\out.jdbf","r"))==0)
			{
				error=3;
			}
			else
			{
				scan=false;
				while(!feof(fp))
				{
					fscanf(fp,"%295s",&data);
					if(strstr(data,"response"))
						fscanf(fp,"%295s",&data);
					data[strlen(data)-1]='\0';
					if(atoi(data)==1)//qued
						scan=true;
				}
				if(scan==true)
				{
					wt_secure=0;
					rewind(fp);
					while(!strstr(data,"resource")&&wt_secure<=1000)
					{
						wt_secure++;
						fscanf(fp,"%295s",&data);
					}
					if(wt_secure<=9995)
						error=4;
					fscanf(fp,"%295s",&data);
					data[strlen(data)-1]='\0';
					//scanresult[strlen(scanresult)-2]='\0';
					data[0]=' ';
					for(int c=0;c<strlen(data);c++)
						data[c]=data[c+1];	
					data[strlen(data)-1]='\0';
					//check if response is for this file
					printf("d:%s\ni:%s\n",data,scanid);
					//if(strcmp(data,scanid)==0)
					if(1)
					{
						printf("Got response, validating\n");		
						rewind(fp);
						wt_secure=0;					
						while(!strstr(data,"positives")&&wt_secure<=10000)
						{
							wt_secure++;
							fscanf(fp,"%295s",&data);
						}
						if(wt_secure<=9995)
							error=4;
							
						fscanf(fp,"%45s",&detected);//how many engines detected the file?
						rewind(fp);
						wt_secure=0;					
						while(!strstr(data,"total\"")&&wt_secure<=10000)
						{
							wt_secure++;
							fscanf(fp,"%295s",&data);
						}
						if(wt_secure<=9995)
							error=4;
							
						fscanf(fp,"%45s",&total);//how many engines scanned the file?
						total[strlen(total)-1]='\0';//remove ","
						detected[strlen(detected)-1]='\0';//remove ","
						//printf("%s::testtesttesttest::%s::%d\n",detected,total,wt_secure);
						if(atoi(detected)!=0)
						{
							answer=true;
							sprintf(data,"WARNING: The scanned file %s%s was detected by %d out of %d engines. Should the file be removed?",folder,file,atoi(detected),atoi(total));
							if(MessageBox(NULL,data,"WARNING",MB_YESNO|MB_ICONWARNING|MB_SYSTEMMODAL)==IDYES)
							{
								sprintf(data,"%s%s",folder,file);
								printf("removing %s\n",data);
								remove(data);
							}
						}
						else
						{
							answer=true;
							printf("No engine detected the file\n");	
						}					
					}
				}
				else
				{
					fclose(fp);
				}
			}
		}
	}
	if(error && !answer && iterator)
	{
		sprintf(data,"WARNING: the file could not be scanned due to internal errors");
		MessageBox(NULL,data,"WARNING",MB_OK|MB_ICONWARNING|MB_SYSTEMMODAL);
	}
	else if(iterator<=0 && !answer)
	{
		sprintf(data,"WARNING: the file could not be scanned due to connection timeout; you can see the results by yourself via %s on VT",scanid);
		MessageBox(NULL,data,"WARNING",MB_OK|MB_ICONWARNING|MB_SYSTEMMODAL);		
	}
}
#endif