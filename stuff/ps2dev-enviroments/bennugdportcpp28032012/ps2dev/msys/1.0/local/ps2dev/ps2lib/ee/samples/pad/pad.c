/*
  _____     ___ ____
   ____|   |    ____|      PS2LIB OpenSource Project
  |     ___|   |____       (C)2002, Pukko
  ------------------------------------------------------------------------
  pad.h
                           Pad externals
                           rev 1.3 (20030416)
  Pad demo app
  Quick and dirty, little or no error checks etc.. 
  Distributed as is
*/

#include <tamtypes.h>
#include <kernel.h>
#include <sifrpc.h>
#include <loadfile.h>

#include "libpad.h"

#define ROM_PADMAN

#if defined(ROM_PADMAN) && defined(NEW_PADMAN)
#error Only one of ROM_PADMAN & NEW_PADMAN should be defined!
#endif

#if !defined(ROM_PADMAN) && !defined(NEW_PADMAN)
#error ROM_PADMAN or NEW_PADMAN must be defined!
#endif


/*
 * Global var's
 */
// pad_dma_buf is provided by the user, one buf for each pad
// contains the pad's current state
static char padBuf[256] __attribute__((aligned(64)));

static char actAlign[6];
static int actuators;


/*
 * Local functions
 */

/*
 * loadModules()
 */
void
loadModules(void)
{
    int ret;

    
#ifdef ROM_PADMAN
    ret = SifLoadModule("rom0:SIO2MAN", 0, NULL);
#else
    ret = SifLoadModule("rom0:XSIO2MAN", 0, NULL);
#endif
    if (ret < 0) {
        scr_printf("sifLoadModule sio failed: %d\n", ret);
        SleepThread();
    }    

#ifdef ROM_PADMAN
    ret = SifLoadModule("rom0:PADMAN", 0, NULL);
#else
    ret = SifLoadModule("rom0:XPADMAN", 0, NULL);
#endif 
    if (ret < 0) {
        scr_printf("sifLoadModule pad failed: %d\n", ret);
        SleepThread();
    }
}

/*
 * waitPadReady()
 */
int waitPadReady(int port, int slot)
{
    int state;
    int lastState;
    char stateString[16];

    state = padGetState(port, slot);
    lastState = -1;
    while((state != PAD_STATE_STABLE) && (state != PAD_STATE_FINDCTP1)) {
        if (state != lastState) {
            padStateInt2String(state, stateString);
            scr_printf("Please wait, pad(%d,%d) is in state %s\n", 
                       port, slot, stateString);
        }
        lastState = state;
        state=padGetState(port, slot);
    }
    // Were the pad ever 'out of sync'?
    if (lastState != -1) {
        scr_printf("Pad OK!\n");
    }
}


/*
 * initializePad()
 */
int
initializePad(int port, int slot)
{

    int ret;
    int modes;
    int i;

    waitPadReady(port, slot);

    // How many different modes can this device operate in?
    // i.e. get # entrys in the modetable
    modes = padInfoMode(port, slot, PAD_MODETABLE, -1);
    scr_printf("The device has %d modes\n", modes);

    if (modes > 0) {
        scr_printf("( ");
        for (i = 0; i < modes; i++) {
            scr_printf("%d ", padInfoMode(port, slot, PAD_MODETABLE, i));
        }
        scr_printf(")");
    }

    scr_printf("It is currently using mode %d\n", 
               padInfoMode(port, slot, PAD_MODECURID, 0));

    // If modes == 0, this is not a Dual shock controller 
    // (it has no actuator engines)
    if (modes == 0) {
        scr_printf("This is a digital controller?\n");
        return 1;
    }

    // Verify that the controller has a DUAL SHOCK mode
    i = 0;
    do {
        if (padInfoMode(port, slot, PAD_MODETABLE, i) == PAD_TYPE_DUALSHOCK)
            break;
        i++;
    } while (i < modes);
    if (i >= modes) {
        scr_printf("This is no Dual Shock controller\n");
        return 1;
    }

    // If ExId != 0x0 => This controller has actuator engines
    // This check should always pass if the Dual Shock test above passed
    ret = padInfoMode(port, slot, PAD_MODECUREXID, 0);
    if (ret == 0) {
        scr_printf("This is no Dual Shock controller??\n");
        return 1;
    }

    scr_printf("Enabling dual shock functions\n");

    // When using MMODE_LOCK, user cant change mode with Select button
    padSetMainMode(port, slot, PAD_MMODE_DUALSHOCK, PAD_MMODE_LOCK);

    waitPadReady(port, slot);
    scr_printf("infoPressMode: %d\n", padInfoPressMode(port, slot));

    waitPadReady(port, slot);        
    scr_printf("enterPressMode: %d\n", padEnterPressMode(port, slot));

    waitPadReady(port, slot);
    actuators = padInfoAct(port, slot, -1, 0);
    scr_printf("# of actuators: %d\n",actuators);

    if (actuators != 0) {
        actAlign[0] = 0;   // Enable small engine
        actAlign[1] = 1;   // Enable big engine
        actAlign[2] = 0xff;
        actAlign[3] = 0xff;
        actAlign[4] = 0xff;
        actAlign[5] = 0xff;

        waitPadReady(port, slot);
        scr_printf("padSetActAlign: %d\n", 
                   padSetActAlign(port, slot, actAlign));
    }
    else {
        scr_printf("Did not find any actuators.\n");
    }

    waitPadReady(port, slot);

    return 1;
}





int
main()
{
    int ret;
    int port, slot;
    int i;
    struct padButtonStatus buttons;
    u32 paddata;
    u32 old_pad = 0;
    u32 new_pad;


    SifInitRpc(0);

    init_scr();

    scr_printf("Hi!\n"
               "Very welcome to this small and stupid pad test application\n"
               "Hope you will find the source useful though =)\n"
               "Please, use & abuse the code, but I would not mind \n"
               "a small greeting when you do :)\n"
               "I myself would like to leave a special thanks to Gustavo "
               "S:\nWithout your psx2lib this would've been impossible\n\n"
               "                                                 - pukko\n\n");


    loadModules();

    padInit(0);



    port = 0; // 0 -> Connector 1, 1 -> Connector 2
    slot = 0; // Always zero if not using multitap

    scr_printf("PortMax: %d\n", padGetPortMax());
    scr_printf("SlotMax: %d\n", padGetSlotMax(port));

    
    if((ret = padPortOpen(port, slot, padBuf)) == 0) {
        scr_printf("padOpenPort failed: %d\n", ret);
        SleepThread();
    }
    
    if(!initializePad(port, slot)) {
        scr_printf("pad initalization failed!\n");
        SleepThread();
    }
    
    for (;;) {      // We are phorever people
            
        i=0;
        ret=padGetState(port, slot);
        while((ret != PAD_STATE_STABLE) && (ret != PAD_STATE_FINDCTP1)) {
            if(ret==PAD_STATE_DISCONN) {
                scr_printf("Pad(%d, %d) is disconnected\n", port, slot);
            }
            ret=padGetState(port, slot);
        }
        if(i==1) {
            scr_printf("Pad: OK!\n");
        }
            
        ret = padRead(port, slot, &buttons); // port, slot, buttons
            
        if (ret != 0) {
            paddata = 0xffff ^ ((buttons.btns[0] << 8) | 
                                buttons.btns[1]);
                
            new_pad = paddata & ~old_pad;
            old_pad = paddata;
                
            // Directions
            if(new_pad & PAD_LEFT) {
                scr_printf("LEFT\n");
            }
            if(new_pad & PAD_DOWN) {
                scr_printf("DOWN\n");
            }
            if(new_pad & PAD_RIGHT) {
                scr_printf("RIGHT\n");
                /*
                       padSetMainMode(port, slot, 
                                      PAD_MMODE_DIGITAL, PAD_MMODE_LOCK));
                */
            }
            if(new_pad & PAD_UP) {
                scr_printf("UP\n");
            }
            if(new_pad & PAD_START) {
                scr_printf("START\n");
            }
            if(new_pad & PAD_R3) {
                scr_printf("R3\n");
            }
            if(new_pad & PAD_L3) {
                scr_printf("L3\n");
            }
            if(new_pad & PAD_SELECT) {
                scr_printf("SELECT\n");
            }
            if(new_pad & PAD_SQUARE) {
                scr_printf("SQUARE\n");
            }
            if(new_pad & PAD_CROSS) {
                padEnterPressMode(port, slot);
                scr_printf("CROSS - Enter press mode\n");
            }
            if(new_pad & PAD_CIRCLE) {
                padExitPressMode(port, slot);
                scr_printf("CIRCLE - Exit press mode\n");
            }
            if(new_pad & PAD_TRIANGLE) {
                // Check for the reason below..
                scr_printf("TRIANGLE (press mode disabled, see code)\n");
            }
            if(new_pad & PAD_R1) {
                actAlign[0] = 1; // Start small engine
                padSetActDirect(port, slot, actAlign);
                scr_printf("R1 - Start little engine\n");
            }
            if(new_pad & PAD_L1) {
                actAlign[0] = 0; // Stop engine 0
                padSetActDirect(port, slot, actAlign);                
                scr_printf("L1 - Stop little engine\n");
            }
            if(new_pad & PAD_R2) {
                scr_printf("R2\n");
            }
            if(new_pad & PAD_L2) {
                scr_printf("L2\n");
            }

            // Test the press mode
            /* Calling SetActAlign repetedly will kill the iop :P
             * (guess the EE is to fast for IOP to handle..)
             * So I'd recommend to change the actuator values only once per
             * vblank or so..
             */
            if(buttons.triangle_p) {
#if 0
                actAlign[1] = (i >> 3); buttons.triangle_p;  // Big engine
                padSetActDirect(port, slot, actAlign);
#else
                scr_printf("TRIANGLE %d\n", buttons.triangle_p);
#endif
            }
            // Start little engine if we move right analogue stick right
            if(buttons.rjoy_h > 0xf0)
            {
                // Stupid check to see if engine is already running,
                // just to prevent overloading the IOP with requests
                if (actAlign[0] == 0) {
                    actAlign[0] = 1;
                    padSetActDirect(port, slot, actAlign);
                }
            }
        } 
    } // for

    scr_printf("Goto sleep!\n");
    SleepThread();

    return 0;
}
