#ifndef __UPDATER_H__
#define __UPDATER_H__

#include <windows.h>
#include <stdio.h>

/* The type of the target updating controller */
typedef enum UP_TYPE_e {
  UP_TYPE_MSP430=0,
  UP_TYPE_DLPC300,
  UP_TYPE_DLPFPGA,
  UP_TYPE_MAX,
  UP_TYPE_UNKNOW=UP_TYPE_MAX
} UP_TYPE_et,*pUP_TYPE_et;

/* Function pointer of updater */
typedef BOOL (*updater_fp)(FILE *fptr,int verbose);

#ifdef __cplusplus
extern "C" {
#endif

  BOOL updateMSP430(FILE *fptr,int verbose);
  BOOL updateDLPC300(FILE *fptr,int verbose);
  BOOL updateDLPFPGA(FILE *fptr,int verbose);

#ifdef __cplusplus
}
#endif

#endif  /* __UPDATER_H__ */
