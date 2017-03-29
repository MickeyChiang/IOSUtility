#include "serialPort.h"
#include "updater.h"
#include <string.h>
#include <io.h>

static void _usage(const char *prog) {
  printf("[Usage] %s [OPTION...]\n\n",prog);
  printf("  Main options:\n");
  printf("     -p COM       the COM port number\n");
  printf("     -b baudrate  the baudrate\n");
  printf("     -t timeout   the rx timeout (in millisecond)\n");
  printf("     -m file      update msp430 firmware\n");
  printf("     -d file      update dlpc300 flash\n");
  printf("     -f file      update dlp fpga flash\n");
  printf("     -h|-?        display this help\n\n");
  printf("  Informative output:\n");
  printf("     -v           verbosely list transmission\n");
}

static BOOL _getOptions(int argc,char **argv,int *lpCOM,int *lpBaud,int *lpTimeout,pUP_TYPE_et pUpType,
                        FILE **pfptr,int *lpVerbose) {
  int i;

  /* the default settings */
  (*lpCOM)    =1;  /* COM1 */
  (*lpBaud)   =115200;
  (*lpTimeout)=10000;  /* 10 seconds */
  (*pUpType)  =UP_TYPE_UNKNOW;
  (*pfptr)    =NULL;
  (*lpVerbose)=0;

  for(i=1;i<argc;i++) {
    if('-'==argv[i][0]) {
      if(0x00==argv[i][2]) {
        switch(argv[i][1]) {
          case 'p': case 'P':
            i++;
            if(i<argc)  (*lpCOM)=atoi((const char *)argv[i]);
            break;
          case 'b': case 'B':
            i++;
            if(i<argc)  (*lpBaud)=atoi((const char *)argv[i]);
            break;
          case 't': case 'T':
            i++;
            if(i<argc)  (*lpTimeout)=atoi((const char *)argv[i]);
            break;
          case 'm': case 'M': case 'd': case 'D': case 'f': case 'F':
            i++;
            if(i<argc) {
              if(UP_TYPE_UNKNOW==(*pUpType)) {
                if(0==_access((const char *)argv[i],0)) {
                  if(('m'==argv[i-1][1])||('M'==argv[i-1][1]))           (*pUpType)=UP_TYPE_MSP430;
                  else if(('d'==argv[i-1][1])||('D'==argv[i-1][1]))      (*pUpType)=UP_TYPE_DLPC300;
                  else  /* ('f'==argv[i-1][1])||('F'==argv[i-1][1])) */  (*pUpType)=UP_TYPE_DLPFPGA;
                  if(0!=fopen_s(pfptr,(const char *)argv[i],"rb")) {
                    printf("[ERROR] \"%s\" can not be opened!\n",argv[i]);
                    return FALSE;
                  }
                } else {
                  printf("[ERROR] \"%s\" is not a valid file!\n",argv[i]);
                  return FALSE;
                }
              } else {
                printf("[ERROR] duplicated commands!\n");
                return FALSE;
              }
            }
            break;
          case 'v': case 'V':
            (*lpVerbose)=1;
            break;
          case 'h': case 'H': case '?':
            return FALSE;
          default:
            printf("[ERROR] Invalid option: %s\n",argv[i]);
            return FALSE;
        }
      } else {
        printf("[ERROR] Invalid option: %s!\n",argv[i]);
        return FALSE;
      }
    }
  }
  return (UP_TYPE_UNKNOW==(*pUpType))?FALSE:TRUE;
}

static updater_fp sg_updater[UP_TYPE_MAX]={updateMSP430,updateDLPC300,updateDLPFPGA};

int main(int argc,char **argv) {
  int         COM     =0;
  int         baudrate=0;
  int         timeout =0;
  UP_TYPE_et  upType  =UP_TYPE_UNKNOW;
  FILE       *fptr    =NULL;
  int         verbose  =0;

  if(_getOptions(argc,argv,&COM,&baudrate,&timeout,&upType,&fptr,&verbose)) {
    HANDLE hSerial=serialPortConnect(COM,baudrate,timeout);

    if(INVALID_HANDLE_VALUE==hSerial)  return 1;
    else {
      printf("Updated....[%s]!\n",sg_updater[upType](fptr,verbose)?"SUCCESS":"FAIL");
      fclose(fptr);
      serialPortClose(hSerial);
    }
  } else  _usage(argv[0]);
  return 0;
}
