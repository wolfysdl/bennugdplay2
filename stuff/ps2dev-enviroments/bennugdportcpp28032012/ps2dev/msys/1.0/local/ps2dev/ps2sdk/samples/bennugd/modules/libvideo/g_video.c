/*
 *  Copyright ? 2006-2011 SplinterGU (Fenix/Bennugd)
 *  Copyright ? 2002-2006 Fenix Team (Fenix)
 *  Copyright ? 1999-2002 Jos? Luis Cebri?n Pag?e (Fenix)
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

/* --------------------------------------------------------------------------- */

#include <stdlib.h>
#include <string.h>

#include "bgdrtm.h"

#include "bgddl.h"
#include "dlvaracc.h"

#include "libvideo.h"

#ifdef _WIN32
#include <initguid.h>
#include "ddraw.h"
#endif

/* --------------------------------------------------------------------------- */

GRAPH * icon = NULL ;
#if SDL_VERSION_ATLEAST(2,0,0)
SDL_Window  * window = NULL;
SDL_Surface * shadow_screen = NULL ;
SDL_Rect    * blitting_rect = NULL ;
#endif
SDL_Surface * screen = NULL ;
SDL_Surface * scale_screen = NULL ;

char * apptitle = NULL ;

int scr_width = 320 ;
int scr_height = 240 ;

int scr_initialized = 0 ;

int enable_16bits = 0 ;
int enable_32bits = 0 ;
int enable_scale = 0 ;
int full_screen = 0 ;
int double_buffer = 0 ;
int hardware_scr = 0 ;
int grab_input = 0 ;
int frameless = 0 ;
int scale_mode = SCALE_NONE ;
int waitvsync = 0 ;

int scale_resolution = 0 ;
int * scale_resolution_table_w = NULL;
int * scale_resolution_table_h = NULL;
int scale_resolution_aspectratio = 0;
int scale_resolution_orientation = 0;

int scale_resolution_aspectratio_offx = 0;
int scale_resolution_aspectratio_offy = 0;

/* --------------------------------------------------------------------------- */

DLCONSTANT  __bgdexport( libvideo, constants_def )[] =
{
    { "M320X200"            , TYPE_DWORD    , 3200200               },
    { "M320X240"            , TYPE_DWORD    , 3200240               },
    { "M320X400"            , TYPE_DWORD    , 3200400               },
    { "M360X240"            , TYPE_DWORD    , 3600240               },
    { "M376X282"            , TYPE_DWORD    , 3760282               },
    { "M400X300"            , TYPE_DWORD    , 4000300               },
    { "M512X384"            , TYPE_DWORD    , 5120384               },
    { "M640X400"            , TYPE_DWORD    , 6400400               },
    { "M640X480"            , TYPE_DWORD    , 6400480               },
    { "M800X600"            , TYPE_DWORD    , 8000600               },
    { "M1024X768"           , TYPE_DWORD    , 10240768              },
    { "M1280X1024"          , TYPE_DWORD    , 12801024              },

    { "MODE_WINDOW"         , TYPE_DWORD    , MODE_WINDOW           },
    { "MODE_2XSCALE"        , TYPE_DWORD    , MODE_2XSCALE          },
    { "MODE_FULLSCREEN"     , TYPE_DWORD    , MODE_FULLSCREEN       },
    { "MODE_DOUBLEBUFFER"   , TYPE_DWORD    , MODE_DOUBLEBUFFER     },
    { "MODE_HARDWARE"       , TYPE_DWORD    , MODE_HARDWARE         },

    { "MODE_WAITVSYNC"      , TYPE_DWORD    , MODE_WAITVSYNC        },
    { "WAITVSYNC"           , TYPE_DWORD    , MODE_WAITVSYNC        },

    { "DOUBLE_BUFFER"       , TYPE_DWORD    , MODE_DOUBLEBUFFER     },  /* Obsolete */
    { "HW_SURFACE"          , TYPE_DWORD    , MODE_HARDWARE         },  /* Obsolete */

    { "MODE_8BITS"          , TYPE_DWORD    , 8                     },
    { "MODE_16BITS"         , TYPE_DWORD    , 16                    },
    { "MODE_32BITS"         , TYPE_DWORD    , 32                    },

    { "MODE_8BPP"           , TYPE_DWORD    , 8                     },
    { "MODE_16BPP"          , TYPE_DWORD    , 16                    },
    { "MODE_32BPP"          , TYPE_DWORD    , 32                    },

    { "MODE_MODAL"          , TYPE_DWORD    , MODE_MODAL            },  /* GRAB INPU */
    { "MODE_FRAMELESS"      , TYPE_DWORD    , MODE_FRAMELESS        },  /* FRAMELESS window */

    { "SCALE_NONE"          , TYPE_DWORD    , SCALE_NONE            },

    { "SRO_NORMAL"          , TYPE_DWORD    , SRO_NORMAL            },
    { "SRO_LEFT"            , TYPE_DWORD    , SRO_LEFT              },
    { "SRO_DOWN"            , TYPE_DWORD    , SRO_DOWN              },
    { "SRO_RIGHT"           , TYPE_DWORD    , SRO_RIGHT             },

    { "SRA_STRETCH"         , TYPE_DWORD    , SRA_STRETCH           },
    { "SRA_PRESERVE"        , TYPE_DWORD    , SRA_PRESERVE          },

    { NULL                  , 0             , 0                     }
} ;

/* --------------------------------------------------------------------------- */

#define GRAPH_MODE                      0
#define SCALE_MODE                      1
#define FULL_SCREEN                     2
#define SCALE_RESOLUTION                3
#define SCALE_RESOLUTION_ASPECTRATIO    4
#define SCALE_RESOLUTION_ORIENTATION    5

/* --------------------------------------------------------------------------- */
/* Definicion de variables globales (usada en tiempo de compilacion) */

char * __bgdexport( libvideo, globals_def ) =
    "graph_mode = 0;\n"
    "scale_mode = 0;\n"
    "full_screen = 0;\n"
    "scale_resolution = 0;\n"
    "scale_resolution_aspectratio = 0;\n"
    "scale_resolution_orientation = 0;\n"
    ;

/* --------------------------------------------------------------------------- */
/* Son las variables que se desea acceder.                           */
/* El interprete completa esta estructura, si la variable existe.    */
/* (usada en tiempo de ejecucion)                                    */

DLVARFIXUP __bgdexport( libvideo, globals_fixup )[] =
{
    /* Nombre de variable global, puntero al dato, tama?o del elemento, cantidad de elementos */
    { "graph_mode" , NULL, -1, -1 },
    { "scale_mode" , NULL, -1, -1 },
    { "full_screen" , NULL, -1, -1 },
    { "scale_resolution", NULL, -1, -1 },

    /* new vars for use with scale_resolution */
    { "scale_resolution_aspectratio", NULL, -1, -1 },
    { "scale_resolution_orientation", NULL, -1, -1 },

    { NULL , NULL, -1, -1 }
};

/* --------------------------------------------------------------------------- */

#ifdef _WIN32
/* Based on allegro */

LPDIRECTDRAW2 directdraw = NULL;
DDCAPS ddcaps;

HRESULT WINAPI( *_DirectDrawCreate )( GUID FAR *lpGUID, LPDIRECTDRAW FAR *lplpDD, IUnknown FAR *pUnkOuter );

/* --------------------------------------------------------------------------- */

int init_dx( void )
{
    HINSTANCE handle;
    LPDIRECTDRAW directdraw1;
    HRESULT hr;
    LPVOID temp;

    handle = LoadLibrary( "DDRAW.DLL" );
    if ( handle == NULL ) return -1;

    _DirectDrawCreate = GetProcAddress( handle, "DirectDrawCreate" );

    hr = _DirectDrawCreate( NULL, &directdraw1, NULL );
    if ( FAILED( hr ) ) return -1;

    hr = IDirectDraw_QueryInterface( directdraw1, &IID_IDirectDraw2, &directdraw );
    if ( FAILED( hr ) ) return -1;

    IDirectDraw_Release( directdraw1 );

    hr = IDirectDraw2_SetCooperativeLevel( directdraw, NULL, DDSCL_NORMAL );
    if ( FAILED( hr ) ) return -1;

    /* get capabilities */
    ddcaps.dwSize = sizeof( ddcaps );
    hr = IDirectDraw2_GetCaps( directdraw, &ddcaps, NULL );
    if ( FAILED( hr ) ) return -1;

    return 0;
}
#endif

/* --------------------------------------------------------------------------- */

void gr_wait_vsync()
{
#ifdef _WIN32
    if ( directdraw ) IDirectDraw2_WaitForVerticalBlank( directdraw, DDWAITVB_BLOCKBEGIN, NULL );
#endif
}

/* --------------------------------------------------------------------------- */

void gr_set_caption( char * title )
{
#if SDL_VERSION_ATLEAST(2,0,0)
    SDL_SetWindowTitle(window, apptitle = title);
#else
    SDL_WM_SetCaption( apptitle = title, "" ) ;
#endif
}

/* --------------------------------------------------------------------------- */

int gr_set_icon( GRAPH * map )
{
    if (( icon = map ))
    {
        SDL_Surface *ico = NULL;
        if ( icon->format->depth == 8 )
        {
            SDL_Color palette[256];
            if ( sys_pixel_format && sys_pixel_format->palette )
            {
                int n ;
                for ( n = 0 ; n < 256 ; n++ )
                {
                    palette[ n ].r = sys_pixel_format->palette->rgb[ n ].r;
                    palette[ n ].g = sys_pixel_format->palette->rgb[ n ].g;
                    palette[ n ].b = sys_pixel_format->palette->rgb[ n ].b;
                }
            }

            ico = SDL_CreateRGBSurfaceFrom( icon->data, 32, 32, 8, 32, 0x00, 0x00, 0x00, 0x00 ) ;

#if SDL_VERSION_ATLEAST(2,0,0)
            SDL_SetPaletteColors(ico->format->palette, palette, 0, 256);
#else
            SDL_SetPalette( ico, SDL_LOGPAL, palette, 0, 256 );
#endif
        }
        else
        {
            ico = SDL_CreateRGBSurfaceFrom( icon->data, 32, 32, icon->format->depth, icon->pitch, icon->format->Rmask, icon->format->Gmask, icon->format->Bmask, icon->format->Amask ) ;
        }
#if SDL_VERSION_ATLEAST(2,0,0)
        SDL_SetWindowIcon(window, ico);
#else
        SDL_SetColorKey( ico, SDL_SRCCOLORKEY, SDL_MapRGB( ico->format, 0, 0, 0 ) ) ;
        SDL_WM_SetIcon( ico, NULL );
#endif
        SDL_FreeSurface( ico ) ;
    }

    return 1 ;
}

/* --------------------------------------------------------------------------- */

int gr_set_mode( int width, int height, int depth )
{
    int n ;
    int sdl_flags = 0;
    int surface_width = width;
    int surface_height = height;
    char * e;

    enable_scale = ( GLODWORD( libvideo, GRAPH_MODE ) & MODE_2XSCALE ) ? 1 : 0 ;
    full_screen = ( GLODWORD( libvideo, GRAPH_MODE ) & MODE_FULLSCREEN ) ? 1 : 0 ;
    double_buffer = ( GLODWORD( libvideo, GRAPH_MODE ) & MODE_DOUBLEBUFFER ) ? 1 : 0 ;
    hardware_scr = ( GLODWORD( libvideo, GRAPH_MODE ) & MODE_HARDWARE ) ? 1 : 0 ;
    grab_input = ( GLODWORD( libvideo, GRAPH_MODE ) & MODE_MODAL ) ? 1 : 0 ;
    frameless = ( GLODWORD( libvideo, GRAPH_MODE ) & MODE_FRAMELESS ) ? 1 : 0 ;
    waitvsync = ( GLODWORD( libvideo, GRAPH_MODE ) & MODE_WAITVSYNC ) ? 1 : 0 ;
    scale_mode = GLODWORD( libvideo, SCALE_MODE );
    full_screen |= GLODWORD( libvideo, FULL_SCREEN );

    scale_resolution = GLODWORD( libvideo, SCALE_RESOLUTION );

    if ( GLOEXISTS( libvideo, SCALE_RESOLUTION_ASPECTRATIO ) ) scale_resolution_aspectratio = GLODWORD( libvideo, SCALE_RESOLUTION_ASPECTRATIO );
    if ( GLOEXISTS( libvideo, SCALE_RESOLUTION_ORIENTATION ) ) scale_resolution_orientation = GLODWORD( libvideo, SCALE_RESOLUTION_ORIENTATION );

    /* Overwrite all params */

    if ( ( e = getenv( "SCALE_RESOLUTION"             ) ) ) scale_resolution = atol( e );
    if ( ( e = getenv( "SCALE_RESOLUTION_ASPECTRATIO" ) ) ) scale_resolution_aspectratio = atol( e );
    if ( ( e = getenv( "SCALE_RESOLUTION_ORIENTATION" ) ) ) scale_resolution_orientation = atol( e );

    if ( scale_resolution_orientation < 0 || scale_resolution_orientation > 4 ) scale_resolution_orientation = 0;

    if ( !depth )
    {
        enable_32bits = ( GLODWORD( libvideo, GRAPH_MODE ) & MODE_32BITS ) ? 1 : 0 ;
        if ( !enable_32bits )
            enable_16bits = ( GLODWORD( libvideo, GRAPH_MODE ) & MODE_16BITS ) ? 1 : 0 ;
        else
            enable_16bits = 0;
        depth = enable_32bits ? 32 : ( enable_16bits ? 16 : 8 );
    }
    else if ( depth == 16 )
    {
        enable_16bits = 1;
        enable_32bits = 0;
    }
    else if ( depth == 32 )
    {
        enable_16bits = 0;
        enable_32bits = 1;
    }

// SDL 2.0 uses a different approach to managing resolutions/windows than SDL1.2
#if SDL_VERSION_ATLEAST(2,0,0)
    sdl_flags = SDL_WINDOW_SHOWN;
    if ( full_screen ) sdl_flags |= SDL_WINDOW_FULLSCREEN;
    if ( frameless ) sdl_flags   |= SDL_WINDOW_BORDERLESS;
    
    // Delete old surfaces, if they're in use
    if ( scale_screen ) {
        SDL_FreeSurface( scale_screen ) ;
        scale_screen = NULL;
    }
    if ( shadow_screen ) {
        SDL_FreeSurface( shadow_screen ) ;
        shadow_screen = NULL;
    }
    if ( screen ) {
        SDL_FreeSurface( screen ) ;
        screen = NULL;
    }
    
    // Delete the old window (looks like this crashes in Android)
    if( window ) {
        SDL_DestroyWindow(window);
        window = NULL;
    }
    
    // Create the new window and retrieve its associated surface
    SDL_Log("No scaling, asked for %dx%d", width, height);
    
    window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              width, height, sdl_flags);
    if ( !window )
        return -1;
    
    SDL_SetWindowGrab(window, grab_input ? SDL_TRUE : SDL_FALSE);
    
    screen = SDL_GetWindowSurface(window);
    if ( !screen )
        return -1;
    
    // Check the surface we've created matches what the user asked for
    // Otherwise create a new surface with the given properties
    // BUT if the given width and/or height is 0, continue happily
    if( ( screen->format->BitsPerPixel != depth ||
          screen->w != width || screen->h != height ) &&
        ( width != 0 && height != 0 ) ) {
        shadow_screen = screen;
        screen = SDL_CreateRGBSurface(0, width, height, depth,
                                             shadow_screen->format->Rmask, 
                                             shadow_screen->format->Gmask,
                                             shadow_screen->format->Bmask,
                                             shadow_screen->format->Amask);
        if ( !screen )
            return -1;

        // Define the SDL_Rect where the game's surface should be blitted
        blitting_rect->x = (screen->w - width) / 2;
        blitting_rect->y = (screen->h - height) / 2;
        blitting_rect->w = width;
        blitting_rect->h = height;
    }
    
    // For debugging purposes
    SDL_Log("Video mode set to %dx%dx%d", screen->w, screen->h, depth);
#else
    if ( scale_resolution_table_w )
    {
        free( scale_resolution_table_w );
        scale_resolution_table_w = NULL;
    }

    if ( scale_resolution_table_h )
    {
        free( scale_resolution_table_h );
        scale_resolution_table_h = NULL;
    }

    if ( scale_resolution )
    {
        surface_width  = scale_resolution / 10000 ;
        surface_height = scale_resolution % 10000 ;
    }
    else
    {
        scale_resolution = 0;

        if ( scale_mode != SCALE_NONE ) enable_scale = 1;
        if ( enable_scale && scale_mode == SCALE_NONE ) scale_mode = SCALE_SCALE2X;

        if ( enable_scale )
        {
            enable_16bits = 1;
            depth = 16;

            surface_width  *= 2;
            surface_height *= 2;
        }
    }

    /* Inicializa el modo gr?fico */

    if ( scrbitmap )
    {
        bitmap_destroy( scrbitmap ) ;
        scrbitmap = NULL ;
    }

    /* Setup the SDL Video Mode */
    sdl_flags = SDL_HWPALETTE;
    if ( double_buffer ) sdl_flags |= SDL_DOUBLEBUF;
    if ( full_screen ) sdl_flags |= SDL_FULLSCREEN;
    if ( frameless ) sdl_flags |= SDL_NOFRAME;

    sdl_flags |= hardware_scr ? SDL_HWSURFACE : SDL_SWSURFACE;
    
    if ( scale_screen ) {
        SDL_FreeSurface( scale_screen ) ;
        scale_screen = NULL;
    }
    if ( screen ) {
        SDL_FreeSurface( screen ) ;
        screen = NULL;
    }

    if ( scale_resolution )
    {
        switch ( scale_resolution_orientation )
        {
            case    SRO_LEFT:
            case    SRO_RIGHT:
            {
                    int aux =  surface_width;
                    surface_width = surface_height;
                    surface_height = aux;
                    break;
            }
        }

        scale_screen = SDL_SetVideoMode( surface_width, surface_height, depth, sdl_flags );

        if ( !scale_screen ) return -1;
        screen = SDL_CreateRGBSurface( sdl_flags, surface_width, surface_height,
                                       scale_screen->format->BitsPerPixel,
                                       scale_screen->format->Rmask,
                                       scale_screen->format->Gmask,
                                       scale_screen->format->Bmask,
                                       scale_screen->format->Amask);

        /* scale tables */

        int     lim_w = 0, lim_h = 0, pitch_w = 0, pitch_h = 0;
        double  fw = 0.0, fh = 0.0, fx = 0.0, fy = 0.0;
        int     h, w;
        int     start_w = 0, start_h = 0, fix = 1;

        scale_resolution_aspectratio_offx = 0;
        scale_resolution_aspectratio_offy = 0;

        switch ( scale_resolution_orientation )
        {
            case    SRO_NORMAL:
            case    SRO_DOWN:
                    lim_w = screen->w;
                    lim_h = screen->h;

                    pitch_w = 1;
                    pitch_h = screen->pitch;

                    fw = (double)screen->w / (double)scale_screen->w;
                    fh = (double)screen->h / (double)scale_screen->h;
                    break;

            case    SRO_LEFT:
            case    SRO_RIGHT:
                    lim_w = screen->h;
                    lim_h = screen->w;

                    pitch_w = screen->pitch;
                    pitch_h = 1;

                    fh = (double)screen->w / (double)scale_screen->h;
                    fw = (double)screen->h / (double)scale_screen->w;
                    break;
        }

        switch ( scale_resolution_orientation )
        {
            case    SRO_NORMAL:
            case    SRO_LEFT:
                    start_w = 0;
                    start_h = 0;
                    fix = -1;
                    break;

            case    SRO_DOWN:
            case    SRO_RIGHT:
                    start_w = scale_screen->w - 1;
                    start_h = scale_screen->h - 1;
                    fix = 1;
                    break;
        }

        if ( scale_resolution_aspectratio == SRA_PRESERVE )
        {
            if ( scale_screen->w > scale_screen->h )
            {
                fw = fh;
                scale_resolution_aspectratio_offx = ( scale_screen->w - lim_w / fw ) / 2 ;
                scale_resolution_aspectratio_offy = 0;
            }
            else
            {
                fh = fw;
                scale_resolution_aspectratio_offx = 0;
                scale_resolution_aspectratio_offy = ( scale_screen->h - lim_h / fh ) / 2 ;
            }
        }

        if ( !( scale_resolution_table_w = malloc( surface_width  * sizeof( int ) ) ) ) return -1;
        if ( !( scale_resolution_table_h = malloc( surface_height * sizeof( int ) ) ) ) return -1;

        for ( w = 0; w < scale_screen->w; w++ )
        {
            if ( w < scale_resolution_aspectratio_offx )
                scale_resolution_table_w[ start_w - w * fix ] = -1;
            else
            {
                scale_resolution_table_w[ start_w - w * fix ] = ( fx < lim_w ) ? pitch_w * ( int ) fx : -1 ;
                fx += fw;
            }
        }

        for ( h = 0; h < scale_screen->h; h++ )
        {
            if ( h < scale_resolution_aspectratio_offy )
                scale_resolution_table_h[ start_h - h * fix ] = -1;
            else
            {
                scale_resolution_table_h[ start_h - h * fix ] = ( fy < lim_h ) ? pitch_h * ( int ) fy : -1 ;
                fy += fh;
            }
        }
    }
    else
    {
        screen = SDL_SetVideoMode( surface_width, surface_height, depth, sdl_flags );
    }
    
    if ( !screen ) return -1;

    SDL_WM_GrabInput( grab_input ? SDL_GRAB_ON : SDL_GRAB_OFF ) ;

#endif

    /* Set window title */
    gr_set_caption( apptitle ) ;

    if ( !sys_pixel_format )
    {
        sys_pixel_format = bitmap_create_format( depth );
    }
    else
    {
        PALETTE * p = sys_pixel_format->palette;

        free( sys_pixel_format );
        sys_pixel_format = bitmap_create_format( depth );

        if ( p )
        {
            sys_pixel_format->palette = p;
            pal_refresh( sys_pixel_format->palette ) ;
        }
    }

    if ( sys_pixel_format->depth == 16 )
    {
        for ( n = 0 ; n < 65536 ; n++ )
        {
            colorghost[ n ] =
                ((( n & screen->format->Rmask ) >> 1 ) & screen->format->Rmask ) +
                ((( n & screen->format->Gmask ) >> 1 ) & screen->format->Gmask ) +
                ((( n & screen->format->Bmask ) >> 1 ) & screen->format->Bmask ) ;
        }
//        bitmap_16bits_conversion();
    }

    scr_initialized = 1 ;

    SDL_ShowCursor( 0 ) ;

    pal_refresh( NULL ) ;
    palette_changed = 1 ;

//    gr_make_trans_table();

    /* Bitmaps de fondo */

    /* Only allow background with same properties that video mode */
    if (
        !background ||
        scr_width != width || scr_height != height ||
        sys_pixel_format->depth != background->format->depth )
    {
        if ( background ) bitmap_destroy( background );
        background = bitmap_new( 0, width, height, sys_pixel_format->depth ) ;
        if ( background )
        {
            gr_clear( background ) ;
            bitmap_add_cpoint( background, 0, 0 ) ;
        }
    }

    scr_width = screen->w ;
    scr_height = screen->h ;
    
    gr_draw_frame();

    regions[0].x  = 0 ;
    regions[0].y  = 0 ;
    regions[0].x2 = screen->w - 1 ;
    regions[0].y2 = screen->h - 1 ;

    // Finalmente seteamos icono de aplicacion
    // Necesitamos crear una surface a partir de un MAP generico de 16x16...
    gr_set_icon( icon );

    if ( background ) background->modified = 1;

//    gr_rects_clear();

    return 0;
}

/* --------------------------------------------------------------------------- */

int gr_init( int width, int height )
{
    return gr_set_mode( width, height, 0 );
}

/* --------------------------------------------------------------------------- */

void __bgdexport( libvideo, module_initialize )()
{
    char * e;

    if ( !SDL_WasInit( SDL_INIT_VIDEO ) ) SDL_InitSubSystem( SDL_INIT_VIDEO );

#ifdef _WIN32
    if ( !directdraw ) init_dx();
#endif
    apptitle = appname;

    if ( ( e = getenv( "VIDEO_WIDTH"  ) ) ) scr_width = atoi(e);
    if ( ( e = getenv( "VIDEO_HEIGHT" ) ) ) scr_height = atoi(e);
    if ( ( e = getenv( "VIDEO_DEPTH"  ) ) )
        GLODWORD( libvideo, GRAPH_MODE ) = atoi(e);
    else
        GLODWORD( libvideo, GRAPH_MODE ) = MODE_16BITS;

    // Don't autostart video
    //gr_init( scr_width, scr_height ) ;
}

/* --------------------------------------------------------------------------- */

void __bgdexport( libvideo, module_finalize )()
{
#ifdef _WIN32
    if ( directdraw )
    {
        /* set cooperative level back to normal */
        IDirectDraw2_SetCooperativeLevel( directdraw, NULL, DDSCL_NORMAL );

        /* release DirectDraw interface */
        IDirectDraw2_Release( directdraw );

        directdraw = NULL;
    }
#endif

#if SDL_VERSION_ATLEAST(1, 3, 0)
    if(window != NULL)
        SDL_DestroyWindow(window);
#endif

    if ( SDL_WasInit( SDL_INIT_VIDEO ) ) SDL_QuitSubSystem( SDL_INIT_VIDEO );
}

/* --------------------------------------------------------------------------- */

char * __bgdexport( libvideo, modules_dependency )[] =
{
    "libgrbase",
    NULL
};

/* --------------------------------------------------------------------------- */
