/****************************************************************************\
*           Copyright (C) 1999-2000 Texas Instruments Incorporated.
*                           All Rights Reserved 
*------------------------------------------------------------------------------
* MODULE NAME... MMC
* FILENAME...... csl_mmcdat.c
* DATE CREATED.. Thu 10 April 2003
* PROJECT....... Chip Support Library
* COMPONENT..... Service Layer
* PREREQUISITS.. 
*------------------------------------------------------------------------------
*
* HISTORY:
*	 Created:		Thu 10 April 2003
*
*------------------------------------------------------------------------------
* DESCRIPTION: Data File for MMC Module
*
\*************************************************************************/
#define _MMC_MOD_
/*************************************************************************\
* Included Header Files                                                  *
\*************************************************************************/

#include <csl_mmc.h>
#include <csl_pwr.h>

#if (_MMC_SUPPORT)

#pragma DATA_SECTION(CSL_MmcData,".csldata:MMC_data")

MMC_CallBackObj isr_init = {0,0,0,0,0,0,0,0,0,0,0,0};
#define CSL_MMCDATAINIT \
{  /* MMC 0 Data Initialization*/\
   0x0000u,                 /* MMC Device number*/\
   0x0007u,                 /* MMC EventId*/\
   0x0000u,                 /* Last status */\
   (MMC_IsrPtr)0x0000u,     /* Default ISR dispatcher function */\
   (MMC_MmcRegObj *)0x4800u,   /* Pointer to start of MMC control registers*/\
   0x0000u,                 /* Next index in defining new cards to MMC module*/\
   0x0000u,                 /* Index of currently selected card*/\
   0x0000u,                 /* Total number of cards in system*/\
   0x0000u,                 /* Number of active cards in system*/\
   0x0000u,                 /* Number of inactive cards in system*/\
  (MMC_CardObj*)0x0000u,    /* Pointer to list of cards attached to MMC controller*/\
  /* Entries for MMC Dispatch Table*/\
  &isr_init,\
 /* MMC 1 Data Initialization*/\
    0x0001u,                 /* MMC Device number*/\
    0x000Du,                 /* MMC EventId*/\
    0x0000u,                 /* Last status */\
    (MMC_IsrPtr)0x0000u,     /* Defualt MMC ISR handler */\
    (MMC_MmcRegObj *)0x4C00u,   /* Pointer to start of MMC control registers*/\
    0x0000u,                 /* Next index in defining new cards to MMC module*/\
    0x0000u,                 /* Index of currently selected card*/\
    0x0000u,                 /* Total number of cards in system*/\
    0x0000u,                 /* Number of active cards in system*/\
    0x0000u,                 /* Number of inactive cards in system*/\
   (MMC_CardObj*)0x0000u,    /* Pointer to list of cards attached to MMC controller*/\
     /*Entries for MMC 1 Dispatch Table*/\
   &isr_init,\
}

CSL_MmcDataObj  CSL_MmcData = CSL_MMCDATAINIT;


#endif
 
