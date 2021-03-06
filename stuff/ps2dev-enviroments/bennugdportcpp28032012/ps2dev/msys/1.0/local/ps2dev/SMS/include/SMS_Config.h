/*
#     ___  _ _      ___
#    |    | | |    |
# ___|    |   | ___|    PS2DEV Open Source Project.
#----------------------------------------------------------
# (c) 2006 Eugene Plotnikov <e-plotnikov@operamail.com>
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
#
*/
#ifndef __SMS_Config_H
# define __SMS_Config_H

# ifndef __SMS_SMB_H
#  include "SMS_SMB.h"
#  endif  /* __SMS_SMB_H */

# define SMS_BF_SKIN 0x00000001
# define SMS_BF_SORT 0x00000002
# define SMS_BF_AVIF 0x00000004
# define SMS_BF_HDLP 0x00000008
# define SMS_BF_SYSP 0x00000010
# define SMS_BF_MENU 0x00000020
# define SMS_BF_DISK 0x00000040
# define SMS_BF_UDFL 0x00000080
# define SMS_BF_SDFX 0x00000100

# define SMS_DF_AUTO_NET 0x00000001
# define SMS_DF_AUTO_USB 0x00000002
# define SMS_DF_AUTO_HDD 0x00000004
# define SMS_DF_GAMEPAD  0x00000008
# define SMS_DF_REMOTE   0x00000010
# define SMS_DF_SMB      0x00000020
# define SMS_DF_CDVD     0x00000040

# define SMS_PF_SUBS  0x00000001
# define SMS_PF_TIME  0x00000002
# define SMS_PF_BLUR  0x00000004
# define SMS_PF_ANIM  0x00000008
# define SMS_PF_OSUB  0x00000010
# define SMS_PF_SPDIF 0x00000020
# define SMS_PF_RAND  0x00000040
# define SMS_PF_REP   0x00000080
# define SMS_PF_ASD   0x00000100
# define SMS_PF_C32   0x00000200
# define SMS_PF_C16   0x00000400
# define SMS_PF_MP3HP 0x00000800
# define SMS_PF_AUDHP 0x00001000

typedef enum SMScrollBarPos {

 SMScrollBarPos_Top      = 0,
 SMScrollBarPos_Bottom   = 1,
 SMScrollBarPos_Inactive = 2,

} SMScrollBarPos;

typedef struct SMSConfig {
/* Version 0 fields - 272 bytes */
 int            m_Version;
 int            m_DX;
 int            m_DY;
 int            m_DisplayMode;
 char           m_Partition[ 256 ];
/* Version 1 fields - 32 bytes */
 unsigned int   m_BrowserABCIdx;
 unsigned int   m_BrowserIBCIdx;
 unsigned int   m_BrowserFlags;
 unsigned int   m_BrowserTxtIdx;
 unsigned int   m_NetworkFlags;
 unsigned int   m_DisplayCharset;
 unsigned short m_PlayerVolume;
 unsigned short m_PlayerAC3RL;
 unsigned int   m_ResMode;
/* Version 2 fields - 60 bytes */
 unsigned int   m_PlayerFlags;
 unsigned int   m_PlayerDisplay;
 unsigned int   m_PlayerSAlign;
 unsigned int   m_PlayerSCNIdx;
 unsigned int   m_PlayerSCBIdx;
 unsigned int   m_PlayerSCIIdx;
 unsigned int   m_PlayerSCUIdx;
 unsigned int   m_BrowserSCIdx;
 unsigned int   m_BrowserSBCIdx;
 unsigned int   m_PlayerSubOffset;
          int   m_PowerOff;
 unsigned int   m_PlayerVBCIdx;
 unsigned int   m_PlayerSBCIdx;
 unsigned int   m_ScrollBarNum;
 unsigned int   m_ScrollBarPos;
/* Version 3 fields - 80 bytes */
          int   m_SubHIncr;
          int   m_SubVIncr;
 char           m_Language[ 56 ];
 float          m_PAR  [ 2 ];
 int            m_DispH[ 2 ];
/* Version 4 fields - 32 bytes */
 int            m_DXPALOther[ 4 ];
 int            m_DYPALOther[ 4 ];
/* Version 4 fields - 4 bytes */
 unsigned int   m_PlayerBrightness;
} SMSConfig;

extern SMSConfig    g_Config        __attribute__(   (  section( ".data" )  )   );
extern unsigned int g_Palette[ 16 ] __attribute__(   (  section( ".data" )  )   );
extern SMBLoginInfo g_SMBLoginInfo  __attribute__(   (  section( ".data" )  )   );

# ifdef __cplusplus
extern "C" {
# endif  /* __cplusplus */

int SMS_LoadConfig ( void );
int SMS_SaveConfig ( void );

# ifdef __cplusplus
}
# endif  /* __cplusplus */
#endif  /* __SMS_Config_H */
