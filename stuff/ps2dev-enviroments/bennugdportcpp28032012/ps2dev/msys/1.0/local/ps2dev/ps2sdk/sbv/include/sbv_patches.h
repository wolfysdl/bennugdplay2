/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright (c) 2003  Marcus R. Brown <mrbrown@0xd6.org>
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
#
# $Id: sbv_patches.h 629 2004-10-11 00:45:00Z mrbrown $
# SBV patches.
*/

#ifndef SBV_PATCHES_H
#define SBV_PATCHES_H

#ifdef __cplusplus
extern "C" {
#endif

int sbv_patch_enable_lmb(void);

int sbv_patch_disable_prefix_check(void);

#ifdef __cplusplus
}
#endif

#endif /* SBV_PATCHES_H */
