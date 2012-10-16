#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<errno.h>
#include<string.h>
#include<signal.h>
#include<unistd.h>


#define FILE_PATH 	"/home/slacker/audacious/msgq/"
#define PROJ_ID		    0x55
#define INFORM          0x01
#define REPORT          0x02
#define FATAL           0x03

typedef struct{
    int msgType;
    char msg[256];
}msgq_t;



msgq_t mqstructVar;
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
void *sighandler(int sigNum)
{
    static char fnName[]="sighandler():";
    if( (sigNum == 11)||(sigNum == 2) || (sigNum == 9))
    {
        stopService(0);
        snprintf(dbgLogStr,sizeof(dbgLogStr),"%s: Sig Num:%d",fnName,sigNum);
        dbgLog(INFORM,dbgLogStr);
        _exit(0);
    }
    else
      {
        snprintf(dbgLogStr,sizeof(dbgLogStr),"%s: Sig Num:%d",fnName,sigNum);
        dbgLog(INFORM,dbgLogStr);
        _exit(0);
        
      } 
}
int main(void)
{
    static char fnName[]="main():";	
    
    signal(SIGINT,(void *)&sighandler); 
    signal(2,(void *)&sighandler); 
    signal(9,(void *)&sighandler); 
    signal(11,(void *)&sighandler); 
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
    errno =0;
    mqstructVar.msgType=1;
    strncpy(mqstructVar.msg,"Hi",3);
    if((msgsnd(msgQId,(char *)&mqstructVar,sizeof(mqstructVar),IPC_NOWAIT)) < 0)
    {
        snprintf(dbgLogStr,sizeof(dbgLogStr),"%s:%s:%d msgsnd error:%d",__FILE__,fnName,__LINE__,errno);
        dbgLog(INFORM,dbgLogStr);
    }
    else
    {
        dbgLog(INFORM,"Msg: Hi, sent");
    }
    errno =0;
    mqstructVar.msgType=2;
    strncpy(mqstructVar.msg,"Hello",6);
    if((msgsnd(msgQId,(char *)&mqstructVar,sizeof(mqstructVar),IPC_NOWAIT)) < 0)
    {
        snprintf(dbgLogStr,sizeof(dbgLogStr),"%s:%s:%d msgsnd error:%d",__FILE__,fnName,__LINE__,errno);
        dbgLog(INFORM,dbgLogStr);
    }
    else
    {
        dbgLog(INFORM,"Msg: Hello, sent");
    }
    sleep(30);

    stopService(0);
    return 0;
}


