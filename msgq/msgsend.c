#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<errno.h>

#define FILE_PATH 	"/home/slacker/audacious/msgq/"
#define PROJ_ID		    0x55
#define INFORM          0x01
#define REPORT          0x02
#define FATAL           0x03
key_t keyValue;
int msgQId;
char dbgLogStr[256];

int dbgLog(int dbgLevel,char *log);
int stopService(char flag);

int dbgLog(int dbgLevel,char *log)
{
    static char fnName[]="dbgLog():";
    printf("\n %s \n",log);
    return 0;
}    
int stopService(char flag)
{
    static char fnName[]="stopService():";
    errno =0;
    if( (msgctl(msgQId,IPC_RMID,NULL)) < 0)
    {
        sprintf(dbgLogStr,
                    "%s:%s:%d: Err while removing MSG-Q. Err:%d",
                            __FILE__,fnName,__LINE__,errno);
        dbgLog(INFORM,dbgLogStr);
        return -1;
    }
    snprintf(dbgLogStr,sizeof(dbgLogStr),"%s: MSG Q remove-SUCCESS",fnName);
    dbgLog(INFORM,dbgLogStr);
    return 0;
}
int main(void)
{
    static char fnName[]="main():";	
     
     errno=0;	
     keyValue = ftok(FILE_PATH,PROJ_ID);
	 if(keyValue <= 0)
     {
        snprintf(dbgLogStr,sizeof(dbgLogStr),
                    "%s:%s:%d: ftok failed. Err:%d",
                        __FILE__,fnName,__LINE__,errno);
        dbgLog(INFORM,dbgLogStr);
        return 0;
     }       
    errno=0;
     msgQId  =   msgget(keyValue, IPC_CREAT|0644);
    if(msgQId < 0)
    {
        snprintf(dbgLogStr,
                    sizeof(dbgLogStr),
                    "%s:%s:%d msgget error:%d",
                    __FILE__,fnName,__LINE__,errno);
        dbgLog(INFORM,dbgLogStr);
        return 0;
    }     
    snprintf(dbgLogStr,sizeof(dbgLogStr),"%s: MSG Q ID:%d",fnName,msgQId);
    dbgLog(INFORM,dbgLogStr);
    
    sleep(5);

    stopService(0);
    return 0;
}


