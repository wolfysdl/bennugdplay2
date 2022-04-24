/*
 *  Copyright � 2006-2011 SplinterGU (Fenix/Bennugd)
 *  Copyright � 2002-2006 Fenix Team (Fenix)
 *  Copyright � 1999-2002 Jos� Luis Cebri�n Pag�e (Fenix)
 *
 *  This file is part of Bennu - Game Development
 *
 *  This software is provided 'as-is', without any express or implied
 *  warranty. In no event will the authors be held liable for any damages
 *  arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute it
 *  freely, subject to the following restrictions:
 *
 *     1. The origin of this software must not be misrepresented; you must not
 *     claim that you wrote the original software. If you use this software
 *     in a product, an acknowledgment in the product documentation would be
 *     appreciated but is not required.
 *
 *     2. Altered source versions must be plainly marked as such, and must not be
 *     misrepresented as being the original software.
 *
 *     3. This notice may not be removed or altered from any source
 *     distribution.
 *
 */

#define _LIBKEY_C

#include <stdio.h>
#include <stdlib.h>

#include <SDL.h>

#include "xctype.h"
#include "bgddl.h"
#include "dlvaracc.h"
#include "libkey.h"

/* ----------------------------------------------------------------- */

#define SHIFTSTATUS             0
#define ASCII                   1
#define SCANCODE                2

#define STAT_RSHIFT             0x0000001
#define STAT_LSHIFT             0x0000002
#define STAT_CTRL               0x0000004
#define STAT_ALT                0x0000008
#define STAT_RCTRL              0x0000010
#define STAT_LCTRL              0x0000020
#define STAT_RALT               0x0000040
#define STAT_LALT               0x0000080
#define STAT_NUM                0x0000100
#define STAT_CAPS               0x0000200
#define STAT_SHIFT              0x0000400

/* ---------------------------------------------------------------------- */

typedef struct
{
    SDLKey sym;   /* SDL virtual keysym */
    SDLMod mod;   /* current key modifiers */
    HOTKEY_CALLBACK callback;
} hotkey ;

static hotkey * hotkey_list = NULL ;
static int hotkey_allocated = 0 ;
static int hotkey_count = 0 ;

/* ----------------------------------------------------------------- */

/* Publicas */
key_equiv key_table[127] ;              /* Now we have a search table with equivs */
unsigned char * keystate = NULL;        /* Pointer to key states */

/* ----------------------------------------------------------------- */

#if SDL_VERSION_ATLEAST(2,0,0)
static int sdl_equiv[SDL_NUM_SCANCODES+1] ;
#else
static int sdl_equiv[SDLK_LAST+1] ;
#endif

static int equivs[] =
{
#if SDL_VERSION_ATLEAST(2,0,0)
    SDL_SCANCODE_ESCAPE,        1,
    SDL_SCANCODE_1,             2,
    SDL_SCANCODE_KP_1,          2,
    SDL_SCANCODE_2,             3,
    SDL_SCANCODE_KP_2,          3,
    SDL_SCANCODE_3,             4,
    SDL_SCANCODE_KP_3,          4,
    SDL_SCANCODE_4,             5,
    SDL_SCANCODE_KP_4,          5,
    SDL_SCANCODE_5,             6,
    SDL_SCANCODE_KP_5,          6,
    SDL_SCANCODE_6,             7,
    SDL_SCANCODE_KP_6,          7,
    SDL_SCANCODE_7,             8,
    SDL_SCANCODE_KP_7,          8,
    SDL_SCANCODE_8,             9,
    SDL_SCANCODE_KP_8,          9,
    SDL_SCANCODE_9,             10,
    SDL_SCANCODE_KP_9,          10,
    SDL_SCANCODE_0,             11,
    SDL_SCANCODE_KP_0,          11,
    SDL_SCANCODE_MINUS,         12,
    SDL_SCANCODE_EQUALS,        13,
    SDL_SCANCODE_BACKSPACE,     14,
    SDL_SCANCODE_TAB,           15,
    SDL_SCANCODE_Q,             16,
    SDL_SCANCODE_W,             17,
    SDL_SCANCODE_E,             18,
    SDL_SCANCODE_R,             19,
    SDL_SCANCODE_T,             20,
    SDL_SCANCODE_Y,             21,
    SDL_SCANCODE_U,             22,
    SDL_SCANCODE_I,             23,
    SDL_SCANCODE_O,             24,
    SDL_SCANCODE_P,             25,
    SDL_SCANCODE_LEFTBRACKET,   26,
    SDL_SCANCODE_RIGHTBRACKET,  27,
    SDL_SCANCODE_RETURN,        28,
    SDL_SCANCODE_KP_ENTER,      28,
    SDL_SCANCODE_LCTRL,         29,
    SDL_SCANCODE_RCTRL,         29,
    SDL_SCANCODE_LCTRL,         96,
    SDL_SCANCODE_RCTRL,         94,
    SDL_SCANCODE_A,             30,
    SDL_SCANCODE_S,             31,
    SDL_SCANCODE_D,             32,
    SDL_SCANCODE_F,             33,
    SDL_SCANCODE_G,             34,
    SDL_SCANCODE_H,             35,
    SDL_SCANCODE_J,             36,
    SDL_SCANCODE_K,             37,
    SDL_SCANCODE_L,             38,
    SDL_SCANCODE_SEMICOLON,     39,
    SDL_SCANCODE_APOSTROPHE,    40,
    SDL_SCANCODE_GRAVE,         41,
    SDL_SCANCODE_LSHIFT,        42,
    SDL_SCANCODE_BACKSLASH,     43,
    SDL_SCANCODE_Z,             44,
    SDL_SCANCODE_X,             45,
    SDL_SCANCODE_C,             46,
    SDL_SCANCODE_V,             47,
    SDL_SCANCODE_B,             48,
    SDL_SCANCODE_N,             49,
    SDL_SCANCODE_M,             50,
    SDL_SCANCODE_COMMA,         51,
    SDL_SCANCODE_PERIOD,        52,
    SDL_SCANCODE_KP_PERIOD,     52,
    SDL_SCANCODE_SLASH,         53,
    SDL_SCANCODE_KP_DIVIDE,     53,
    SDL_SCANCODE_RSHIFT,        54,
    /*  SDL_SCANCODE_PRINT,         55,*/
    SDL_SCANCODE_KP_MULTIPLY,   55,
    SDL_SCANCODE_LALT,          56,
    SDL_SCANCODE_RALT,          56,
    SDL_SCANCODE_LALT,          95,
    SDL_SCANCODE_RALT,          93,
    SDL_SCANCODE_MODE,          93,
    SDL_SCANCODE_MODE,          56,
    SDL_SCANCODE_RGUI,          56,
    SDL_SCANCODE_LGUI,          56,
    SDL_SCANCODE_SPACE,         57,
    SDL_SCANCODE_CAPSLOCK,      58,
    SDL_SCANCODE_F1,            59,
    SDL_SCANCODE_F2,            60,
    SDL_SCANCODE_F3,            61,
    SDL_SCANCODE_F4,            62,
    SDL_SCANCODE_F5,            63,
    SDL_SCANCODE_F6,            64,
    SDL_SCANCODE_F7,            65,
    SDL_SCANCODE_F8,            66,
    SDL_SCANCODE_F9,            67,
    SDL_SCANCODE_F10,           68,
    SDL_SCANCODE_NUMLOCKCLEAR,  69,
    SDL_SCANCODE_SCROLLLOCK,    70,
    SDL_SCANCODE_HOME,          71,
    SDL_SCANCODE_UP,            72,
    SDL_SCANCODE_PAGEUP,        73,
    SDL_SCANCODE_KP_MINUS,      74,
    SDL_SCANCODE_LEFT,          75,
    SDL_SCANCODE_RIGHT,         77,
    SDL_SCANCODE_KP_PLUS,       78,
    SDL_SCANCODE_END,           79,
    SDL_SCANCODE_DOWN,          80,
    SDL_SCANCODE_PAGEDOWN,      81,
    SDL_SCANCODE_INSERT,        82,
    SDL_SCANCODE_DELETE,        83,
    SDL_SCANCODE_F11,           87,
    SDL_SCANCODE_F12,           88,
    SDL_SCANCODE_KP_LESS,       89,
    SDL_SCANCODE_KP_PLUS,       90,
    SDL_SCANCODE_KP_GREATER,    91,
    //SDL_SCANCODE_QUESTION,      92,
    SDL_SCANCODE_GRAVE,         93,
    SDL_SCANCODE_SYSREQ,        55,
    SDL_SCANCODE_PAUSE,         95,
    SDL_SCANCODE_MENU,          97,
    SDL_SCANCODE_LGUI,          98,
    SDL_SCANCODE_RGUI,          99,
    //SDL_SCANCODE_COMPOSE,       99,
#else
    SDLK_ESCAPE,        1,
    SDLK_1,             2,
    SDLK_KP1,           2,
    SDLK_2,             3,
    SDLK_KP2,           3,
    SDLK_3,             4,
    SDLK_KP3,           4,
    SDLK_4,             5,
    SDLK_KP4,           5,
    SDLK_5,             6,
    SDLK_KP5,           6,
    SDLK_6,             7,
    SDLK_KP6,           7,
    SDLK_7,             8,
    SDLK_KP7,           8,
    SDLK_8,             9,
    SDLK_KP8,           9,
    SDLK_9,             10,
    SDLK_KP9,           10,
    SDLK_0,             11,
    SDLK_KP0,           11,
    SDLK_MINUS,         12,
    SDLK_EQUALS,        13,
    SDLK_BACKSPACE,     14,
    SDLK_TAB,           15,
    SDLK_q,             16,
    SDLK_w,             17,
    SDLK_e,             18,
    SDLK_r,             19,
    SDLK_t,             20,
    SDLK_y,             21,
    SDLK_u,             22,
    SDLK_i,             23,
    SDLK_o,             24,
    SDLK_p,             25,
    SDLK_LEFTBRACKET,   26,
    SDLK_RIGHTBRACKET,  27,
    SDLK_RETURN,        28,
    SDLK_KP_ENTER,      28,
    SDLK_LCTRL,         29,
    SDLK_RCTRL,         29,
    SDLK_LCTRL,         96,
    SDLK_RCTRL,         94,
    SDLK_a,             30,
    SDLK_s,             31,
    SDLK_d,             32,
    SDLK_f,             33,
    SDLK_g,             34,
    SDLK_h,             35,
    SDLK_j,             36,
    SDLK_k,             37,
    SDLK_l,             38,
    SDLK_SEMICOLON,     39,
    SDLK_QUOTE,         40,
    SDLK_BACKQUOTE,     41,
    SDLK_LSHIFT,        42,
    SDLK_BACKSLASH,     43,
    SDLK_z,             44,
    SDLK_x,             45,
    SDLK_c,             46,
    SDLK_v,             47,
    SDLK_b,             48,
    SDLK_n,             49,
    SDLK_m,             50,
    SDLK_COMMA,         51,
    SDLK_PERIOD,        52,
    SDLK_KP_PERIOD,     52,
    SDLK_SLASH,         53,
    SDLK_KP_DIVIDE,     53,
    SDLK_RSHIFT,        54,
    /*  SDLK_PRINT,         55,*/
    SDLK_KP_MULTIPLY,   55,
    SDLK_LALT,          56,
    SDLK_RALT,          56,
    SDLK_LALT,          95,
    SDLK_RALT,          93,
    SDLK_MODE,          93,
    SDLK_MODE,          56,
    SDLK_RMETA,         56,
    SDLK_LMETA,         56,
    SDLK_SPACE,         57,
    SDLK_CAPSLOCK,      58,
    SDLK_F1,            59,
    SDLK_F2,            60,
    SDLK_F3,            61,
    SDLK_F4,            62,
    SDLK_F5,            63,
    SDLK_F6,            64,
    SDLK_F7,            65,
    SDLK_F8,            66,
    SDLK_F9,            67,
    SDLK_F10,           68,
    SDLK_NUMLOCK,       69,
    SDLK_SCROLLOCK,     70,
    SDLK_HOME,          71,
    SDLK_UP,            72,
    SDLK_PAGEUP,        73,
    SDLK_KP_MINUS,      74,
    SDLK_LEFT,          75,
    SDLK_RIGHT,         77,
    SDLK_KP_PLUS,       78,
    SDLK_END,           79,
    SDLK_DOWN,          80,
    SDLK_PAGEDOWN,      81,
    SDLK_INSERT,        82,
    SDLK_DELETE,        83,
    SDLK_F11,           87,
    SDLK_F12,           88,
    SDLK_LESS,          89,
    SDLK_PLUS,          90,
    SDLK_GREATER,       91,
    SDLK_QUESTION,      92,
    SDLK_CARET,         93,
    SDLK_SYSREQ,        55,
    SDLK_PAUSE,         95,
    SDLK_MENU,          97,
    SDLK_LSUPER,        98,
    SDLK_RSUPER,        99,
    SDLK_COMPOSE,       99,
#endif
    -1, -1
} ;

/* ----------------------------------------------------------------- */

DLCONSTANT  __bgdexport( libkey, constants_def )[] =
{
    { "_ESC",         TYPE_DWORD,  1 },
    { "_1",           TYPE_DWORD,  2 },
    { "_2",           TYPE_DWORD,  3 },
    { "_3",           TYPE_DWORD,  4 },
    { "_4",           TYPE_DWORD,  5 },
    { "_5",           TYPE_DWORD,  6 },
    { "_6",           TYPE_DWORD,  7 },
    { "_7",           TYPE_DWORD,  8 },
    { "_8",           TYPE_DWORD,  9 },
    { "_9",           TYPE_DWORD,  10 },
    { "_0",           TYPE_DWORD,  11 },
    { "_MINUS",       TYPE_DWORD,  12 },
    { "_PLUS",        TYPE_DWORD,  13 },
    { "_BACKSPACE",   TYPE_DWORD,  14 },
    { "_TAB",         TYPE_DWORD,  15 },
    { "_Q",           TYPE_DWORD,  16 },
    { "_W",           TYPE_DWORD,  17 },
    { "_E",           TYPE_DWORD,  18 },
    { "_R",           TYPE_DWORD,  19 },
    { "_T",           TYPE_DWORD,  20 },
    { "_Y",           TYPE_DWORD,  21 },
    { "_U",           TYPE_DWORD,  22 },
    { "_I",           TYPE_DWORD,  23 },
    { "_O",           TYPE_DWORD,  24 },
    { "_P",           TYPE_DWORD,  25 },
    { "_L_BRACKET",   TYPE_DWORD,  26 },
    { "_R_BRACKET",   TYPE_DWORD,  27 },
    { "_ENTER",       TYPE_DWORD,  28 },
    { "_C_ENTER",     TYPE_DWORD,  28 },
    { "_CONTROL",     TYPE_DWORD,  29 },
    { "_A",           TYPE_DWORD,  30 },
    { "_S",           TYPE_DWORD,  31 },
    { "_D",           TYPE_DWORD,  32 },
    { "_F",           TYPE_DWORD,  33 },
    { "_G",           TYPE_DWORD,  34 },
    { "_H",           TYPE_DWORD,  35 },
    { "_J",           TYPE_DWORD,  36 },
    { "_K",           TYPE_DWORD,  37 },
    { "_L",           TYPE_DWORD,  38 },
    { "_SEMICOLON",   TYPE_DWORD,  39 },
    { "_APOSTROPHE",  TYPE_DWORD,  40 },
    { "_WAVE",        TYPE_DWORD,  41 },
    { "_L_SHIFT",     TYPE_DWORD,  42 },
    { "_BACKSLASH",   TYPE_DWORD,  43 },
    { "_Z",           TYPE_DWORD,  44 },
    { "_X",           TYPE_DWORD,  45 },
    { "_C",           TYPE_DWORD,  46 },
    { "_V",           TYPE_DWORD,  47 },
    { "_B",           TYPE_DWORD,  48 },
    { "_N",           TYPE_DWORD,  49 },
    { "_M",           TYPE_DWORD,  50 },
    { "_COMMA",       TYPE_DWORD,  51 },
    { "_POINT",       TYPE_DWORD,  52 },
    { "_SLASH",       TYPE_DWORD,  53 },
    { "_C_BACKSLASH", TYPE_DWORD,  53 },
    { "_R_SHIFT",     TYPE_DWORD,  54 },
    { "_C_ASTERISK",  TYPE_DWORD,  55 },
    { "_PRN_SCR",     TYPE_DWORD,  55 },
    { "_ALT",         TYPE_DWORD,  56 },
    { "_SPACE",       TYPE_DWORD,  57 },
    { "_CAPS_LOCK",   TYPE_DWORD,  58 },
    { "_F1",          TYPE_DWORD,  59 },
    { "_F2",          TYPE_DWORD,  60 },
    { "_F3",          TYPE_DWORD,  61 },
    { "_F4",          TYPE_DWORD,  62 },
    { "_F5",          TYPE_DWORD,  63 },
    { "_F6",          TYPE_DWORD,  64 },
    { "_F7",          TYPE_DWORD,  65 },
    { "_F8",          TYPE_DWORD,  66 },
    { "_F9",          TYPE_DWORD,  67 },
    { "_F10",         TYPE_DWORD,  68 },
    { "_NUM_LOCK",    TYPE_DWORD,  69 },
    { "_SCROLL_LOCK", TYPE_DWORD,  70 },
    { "_HOME",        TYPE_DWORD,  71 },
    { "_C_HOME",      TYPE_DWORD,  71 },
    { "_UP",          TYPE_DWORD,  72 },
    { "_C_UP",        TYPE_DWORD,  72 },
    { "_PGUP",        TYPE_DWORD,  73 },
    { "_C_PGUP",      TYPE_DWORD,  73 },
    { "_C_MINUS",     TYPE_DWORD,  74 },
    { "_LEFT",        TYPE_DWORD,  75 },
    { "_C_LEFT",      TYPE_DWORD,  75 },
    { "_C_CENTER",    TYPE_DWORD,  76 },
    { "_RIGHT",       TYPE_DWORD,  77 },
    { "_C_RIGHT",     TYPE_DWORD,  77 },
    { "_C_PLUS",      TYPE_DWORD,  78 },
    { "_END",         TYPE_DWORD,  79 },
    { "_C_END",       TYPE_DWORD,  79 },
    { "_DOWN",        TYPE_DWORD,  80 },
    { "_C_DOWN",      TYPE_DWORD,  80 },
    { "_PGDN",        TYPE_DWORD,  81 },
    { "_C_PGDN",      TYPE_DWORD,  81 },
    { "_INS",         TYPE_DWORD,  82 },
    { "_C_INS",       TYPE_DWORD,  82 },
    { "_DEL",         TYPE_DWORD,  83 },
    { "_C_DEL",       TYPE_DWORD,  83 },
    { "_F11",         TYPE_DWORD,  87 },
    { "_F12",         TYPE_DWORD,  88 },
    { "_LESS",        TYPE_DWORD,  89 },
    { "_EQUALS",      TYPE_DWORD,  90 },
    { "_GREATER",     TYPE_DWORD,  91 },
    { "_ASTERISK",    TYPE_DWORD,  92 },
    { "_R_ALT",       TYPE_DWORD,  93 },
    { "_R_CONTROL",   TYPE_DWORD,  94 },
    { "_L_ALT",       TYPE_DWORD,  95 },
    { "_L_CONTROL",   TYPE_DWORD,  96 },
    { "_MENU",        TYPE_DWORD,  97 },
    { "_L_WINDOWS",   TYPE_DWORD,  98 },
    { "_R_WINDOWS",   TYPE_DWORD,  99 },

    { "STAT_RSHIFT",  TYPE_DWORD,  STAT_RSHIFT },
    { "STAT_LSHIFT",  TYPE_DWORD,  STAT_LSHIFT },
    { "STAT_CTRL",    TYPE_DWORD,  STAT_CTRL   },
    { "STAT_ALT",     TYPE_DWORD,  STAT_ALT    },
    { "STAT_RCTRL",   TYPE_DWORD,  STAT_RCTRL  },
    { "STAT_LCTRL",   TYPE_DWORD,  STAT_LCTRL  },
    { "STAT_RALT",    TYPE_DWORD,  STAT_RALT   },
    { "STAT_LALT",    TYPE_DWORD,  STAT_LALT   },
    { "STAT_NUM",     TYPE_DWORD,  STAT_NUM    },
    { "STAT_CAPS",    TYPE_DWORD,  STAT_CAPS   },
    { "STAT_SHIFT",   TYPE_DWORD,  STAT_SHIFT  },

    { NULL          , 0         ,  0           }
} ;

/* ----------------------------------------------------------------- */
/* Definicion de variables globales (usada en tiempo de compilacion) */

char * __bgdexport( libkey, globals_def ) =
    "shift_status;\n"
    "ascii;\n"
    "scan_code;\n";

/* ----------------------------------------------------------------- */
/* Son las variables que se desea acceder.                           */
/* El interprete completa esta estructura, si la variable existe.    */
/* (usada en tiempo de ejecucion)                                    */

DLVARFIXUP  __bgdexport( libkey, globals_fixup )[] =
{
    /* Nombre de variable global, puntero al dato, tama�o del elemento, cantidad de elementos */
    { "shift_status" , NULL, -1, -1 },
    { "ascii"        , NULL, -1, -1 },
    { "scan_code"    , NULL, -1, -1 },
    { NULL           , NULL, -1, -1 }
};

/* ----------------------------------------------------------------- */

static void add_key_equiv( int equiv, int keyconst )
{

}

/* ----------------------------------------------------------------- */

void hotkey_add( int mod, int sym, HOTKEY_CALLBACK callback )
{

}

/* ----------------------------------------------------------------- */

/*
 *  FUNCTION : process_key_events
 *
 *  Process all pending SDL events, updating all global variables
 *  and handling debug key presses and cannot be changed
 *
 *  PARAMS :
 *      None
 *
 *  RETURN VALUE :
 *      None
 */

static void process_key_events()
{

}

/* ----------------------------------------------------------------- */

/* Bigest priority first execute
   Lowest priority last execute */

HOOK __bgdexport( libkey, handler_hooks )[] =
{
    { 4900, process_key_events },
    {    0, NULL               }
} ;

/* ----------------------------------------------------------------- */
/* Funciones de inicializacion del modulo/plugin                     */

void __bgdexport( libkey, module_initialize )()
{

}

/* ----------------------------------------------------------------- */

void __bgdexport( libkey, module_finalize )()
{

}

/* ----------------------------------------------------------------- */

char * __bgdexport( libkey, modules_dependency )[] =
{
    "libsdlhandler",
    NULL
};

/* ----------------------------------------------------------------- */