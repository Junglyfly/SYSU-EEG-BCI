#define _MMC_MOD_

#include <csl_chiphal.h>

#if (_MMC_SUPPORT)

#include <csl_mmc.h>

#define VDD_32_33     0x0010  // 0x007f		
#define OP_TIMEOUT    1024

/*should have a wide range of voltage-- between 2.6 and 3.6, but 3.3 is the preferred*/
#pragma CODE_SECTION(MMC_sendOpCond,".text:MMC_sendOpCond")
int MMC_sendOpCond(MMC_Handle mmc, Uint32 hVddMask)
{              
    Uint16 stat;
    long op_timeout;
    long count;
    Uint16 old_intm;
    Uint16 sd_card_found;
       
    ioport MMC_MmcRegObj *regs = mmc->mmcRegs;             
    op_timeout = OP_TIMEOUT; 
  

     
    if (hVddMask < 128 || hVddMask > 4194304)  /* mask must be within 8 - 23 (bit positions)*/
           return -1;
    
    // on sending the SD_APP_CMD command, the SD card will respond, but the MMC card will ignore. 
    // we use this to find out which card is plugged in.
    
    // The status bit is set for the previous command. This has to be read to be 'cleared'.
    MMC_getStatus(mmc, MMC_ST0_RSPDNE);
    
    MMC_clearResponse(mmc);
    regs->mmcargh = 0x0;
    regs->mmcargl = MMC_STUFF_BITS;
    regs->mmccmd  = SD_APP_CMD;
    
    stat = 0;
    count = OP_TIMEOUT;
         
    while (!(stat & 0x4) && (count-- > 0)){
       stat = MMC_getStatus(mmc,MMC_ST0_RSPDNE);
    }
        
    if (count <= 0)
       sd_card_found = 0;
    else
       sd_card_found = 1;

    MMC_clearResponse(mmc);
    if (sd_card_found){
      do {
         // now send the ACMD41 command. This is the SD equivalent of CMD1 for MMC.
         // as long as the card is working on this command, it will keep the MSB of MMCRSP7 set low. It will
         // set this high when it is done, so we have to loop till this. 
         // This is when we know that the SD card is ready.
         
         regs->mmcargh = (Uint16)((hVddMask >> 16) & 0xFFFF);
         regs->mmcargl = (Uint16)(hVddMask & 0xFFFFu);
         regs->mmccmd  = SD_SEND_OP_COND;
   
         count = OP_TIMEOUT;
         stat = 0;
         
         while (!(stat & 0x4) && (count-- > 0))
            stat = MMC_getStatus(mmc,MMC_ST0_RSPDNE);
            
         if (count < 0) // timeout condition
            return -1;
         
         stat = MMC_RGET_H(mmc, MMCRSP7);
         if (stat & 0x8000)
            return SD_CARD;
         
         regs->mmcargh = 0x0;
         regs->mmcargl = MMC_STUFF_BITS;
         regs->mmccmd  = SD_APP_CMD;

         stat = 0;         
         count = OP_TIMEOUT;
         while (!(stat & 0x4) && (count-- > 0))
            stat = MMC_getStatus(mmc,MMC_ST0_RSPDNE);
            
         if (count < 0) //timeout condition
            return -1;
         
         MMC_clearResponse(mmc);
      }while (op_timeout-- > 0);
    } else {
    
      regs->mmcctl |= 0x3;
      regs->mmcctl &= 0xFFFC;
      
      do {    
         regs->mmcargh = (Uint16)((hVddMask >> 16) & 0xFFFF);
         regs->mmcargl = (Uint16)(hVddMask & 0xFFFFu);
         regs->mmccmd  = MMC_SEND_OP_COND;
 
         stat = 0;
         count = OP_TIMEOUT;

         while (!(stat & 0x4) && (count-- > 0))
            stat = MMC_getStatus(mmc,MMC_ST0_RSPDNE);

         // Break on response fail  
         if (count < 0)
            return -1;
         // Resp = R3                     
         // If the card starts up correctly, it sets the status bit in the OCR register. 
         // This register is returned as response (split across response regs) to the send_op_cond command.
         // Busy bit is in MSB of rsp7.  If '1' (not busy) then we are done.                  
         stat = MMC_RGET_H(mmc, MMCRSP7);
         if (stat & 0x8000)
            return MMC_CARD;
         
         MMC_clearResponse(mmc);
       }while (--op_timeout > 0);
     }

    // if it has timed out, then it must be an incompatible/corrupted card.
    return( -1 );
}  

#endif

