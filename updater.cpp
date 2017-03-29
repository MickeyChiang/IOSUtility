#include "serialPort.h"
#include "updater.h"

BOOL updateMSP430(FILE *fptr,int verbose) {
  if(verbose)  printf("Updating MSP430...\n");
  return FALSE;
}

BOOL updateDLPC300(FILE *fptr,int verbose) {
  if(verbose)  printf("Updating DLPC300...\n");
  return FALSE;
}

BOOL updateDLPFPGA(FILE *fptr,int verbose) {
  if(verbose)  printf("Updating DLP FPGA...\n");
  return FALSE;
}
