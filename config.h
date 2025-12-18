/* See LICENSE file for copyright and license details.
 * This config requires Nerd Fonts to display the icons.
 *
 * Installed Patches
 * actualfullscreen-20211013-cb3f58a
 * full-gaps-toggle-20200830
 * (hide_vacant_tags-6.4) - sometimes I like it, sometimes I want to see my icons
 * pertag-20200914-61bb8b2
 * status2d-6.3
 * dwm-scratchpads-20200414-728d397b
 *
 * */

#include <X11/XF86keysym.h>
#define TERMINAL "st"
#define TERMCLASS "st"
#define MODKEY Mod4Mask
#define ALTKEY Mod1Mask

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const Gap default_gap        = {.isgap = 1, .realgap = 5, .gappx = 0};
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "HackNerdFontPropo:size=11:antialias=true:autohint=true", "NotoColorEmoji:size=12:antialias=true:autohint=true" };
static char norm_fg[]               = "#839496";
static char norm_bg[]               = "#002b36";
static char norm_border[]           = "#000000";
static char title_fg[]              = "#2aa198";
static char sel_bg[]                = "#c5c8c6";
static char sel_fg[]                = "#0d597f"; //
static char sel_border[]            = "#268bd2"; //
static const char *colors[][3] = {
//                      fg           bg           border
    [SchemeNorm]    = { norm_fg,    norm_bg,    norm_border },
    [SchemeSel]     = { sel_fg,     sel_bg,     sel_border },
	[SchemeTitle]   = { title_fg,   norm_bg,    norm_border },
};

/* PROGRAM SHORTHANDS */
static char dmenumon[2]         = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]   = { "dmenu_run", NULL };
static const char *termcmd[]    = { "st", NULL};
static const char *web[]        = { "librewolf", NULL };
// static const char *web[]        = { "chromium", NULL };
static const char *calc[]       = { "qalculate-qt", NULL };

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {"st", "-n", "spterm", "-g", "120x34", NULL };
const char *spcmd2[] = {"st", "-n", "spmusic", "-g", "144x41", "-e", "ncmpcpp", NULL };
const char *spcmd3[] = {"st", "-n", "spfm", "-g", "144x41", "-e", "lf", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",      spcmd1},
	{"spmusic",     spcmd2},
	{"spfm",        spcmd3},
};

/* Tags/Workspaces/Desktops/Groups */
static const char *tags[] = { "", "", "", "", "" };
// Neovim, Firefox, Pdf, GIMP, Dharmachakra, AlpineLinux

// static const char *tags[] = { "", "", "", "", "", "", "", "", "" };
// Linux Distros: void, qubes, trisquel, slackware, gentoo, debian, arch, fedora, Alpine

// static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class                instance        title       tags mask     isfloating   monitor */
	{ "Gimp",               NULL,           NULL,       1<<3,            1,           -1 },
	{ "Zathura",            NULL,           NULL,       1<<2,            0,           -1 },
	{ "Chromium",           NULL,           NULL,       1<<1,            0,           -1 },
	{ "Firefox",            NULL,           NULL,       1<<1,            0,           -1 },
	{ "librewolf",          NULL,           NULL,       1<<1,            0,           -1 },
    { "qalculate-qt",       NULL,           NULL,       0,               1,           -1 },
	{ "mpv",                NULL,           NULL,       0,               1,           -1 },
	{ "Qemu-system-x86_64", NULL,           NULL,       1<<4,            1,           -1 },
	{ NULL,		            "spterm",		NULL,		SPTAG(0),	 	 1,			  -1 },
	{ NULL,		            "spmusic",	    NULL,		SPTAG(1),		 1,			  -1 },
	{ NULL,		            "spfm",	        NULL,		SPTAG(2),		 1,			  -1 },
};

/* layout(s) */
static const float mfact        = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster        = 1;    /* number of clients in master area */
static const int resizehints    = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1;    /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "",      tile },    /* first entry is default  */
	{ " ",     NULL },    /* no layout function means floating behavior */
	{ "󰈈",      monocle }, /* Full-sized Windows stacked on top of each other */
};

/* KEY DEFINITIONS */

#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* add functionality for the volume controls, using WirePlumber/Pipewire */
static const char *upvol[]      = { "/usr/bin/wpctl",   "set-volume", "@DEFAULT_AUDIO_SINK@",      "2%+",    NULL };
static const char *downvol[]    = { "/usr/bin/wpctl",   "set-volume", "@DEFAULT_AUDIO_SINK@",      "2%-",    NULL };
static const char *maxvol[]     = { "/usr/bin/wpctl",   "set-volume", "@DEFAULT_AUDIO_SINK@",      "100%",   NULL };
static const char *vol75[]      = { "/usr/bin/wpctl",   "set-volume", "@DEFAULT_AUDIO_SINK@",      "75%",    NULL };
static const char *midvol[]     = { "/usr/bin/wpctl",   "set-volume", "@DEFAULT_AUDIO_SINK@",      "50%",    NULL };
static const char *vol25[]      = { "/usr/bin/wpctl",   "set-volume", "@DEFAULT_AUDIO_SINK@",      "25%",    NULL };
static const char *mutevol[]    = { "/usr/bin/wpctl",   "set-mute",   "@DEFAULT_AUDIO_SINK@",      "toggle", NULL };
static const char *mutemic[]    = { "/usr/bin/wpctl",   "set-mute",   "@DEFAULT_AUDIO_SOURCE@",    "toggle", NULL };

static const Key keys[] = {
	/* modifier                     key         function        argument */
// Program related KeyBindings
    { MODKEY|ShiftMask,          XK_BackSpace,  quit,           {0} }, // Close DWM / Exit back to TTY terminal
    { MODKEY,                       XK_Return,  spawn,          {.v = termcmd } }, // Terminal emulator of choice
	{ MODKEY,                       XK_r,       spawn,          {.v = dmenucmd } }, // dmenu
    { MODKEY,                       XK_c,       spawn,          {.v = calc } }, // Qaclculate
//    { MODKEY,                       XK_f,       spawn,          SHCMD( "" ) }, //
	{ MODKEY,                       XK_g,       spawn,          SHCMD( "gimp" ) }, // GNU Image Manipulation Program
	{ MODKEY,                       XK_q,       killclient,     {0} }, // close the window with focus
	{ MODKEY,                       XK_b,       togglebar,      {0} }, // turn the top bar on/off
	{ MODKEY,                       XK_w,       spawn,          {.v = web } }, // Web Browser of choice
    { 0,                            XK_F2,      spawn,          SHCMD( "slock" ) }, // Screen locker
	{ MODKEY,                       XK_Tab,     view,           {0} }, // Switch b/w last 2 focused windows
 	{ MODKEY,			            XK_m,       spawn,          {.v = (const char*[]){ "mounter", NULL } } },
	{ MODKEY|ALTKEY,  			    XK_m,       spawn,          {.v = (const char*[]){ "unmounter", NULL } } },
// Camera/Video
    { MODKEY,                       XK_Print,   spawn,          SHCMD( "maim -i $(xdotool getactivewindow) ~/Pictures/Screenshots/window-$(date '+%y%m%d-%H%M-%S').png && notify-send \"Screenshot Taken\"" ) },
    { 0,                            XK_Print,   spawn,          SHCMD( "maim ~/Pictures/Screenshots/screen-$(date '+%y%m%d-%H%M-%S').png && notify-send \"Screenshot Taken\"" ) },
//    { MODKEY|ControlMask,           XK_p,       spawn,          SHCMD( "ffmpeg -f video4linux2 -s 640x480 -i /dev/video0 -ss 0:0:2 -frames 1 ~/Pictures/Screenshots/selfie.jpg && notify-send \"Photo Taken\"" ) },
    { MODKEY,                       XK_v,       spawn,          {.v = (const char*[]){ "dmenurecord", NULL } } },// Luke Smith's screencasting dmenu script
    { MODKEY|ControlMask,           XK_v,       spawn,          {.v = (const char*[]){ "dmenurecord", "kill", NULL } } },
// Help Docs
    { 0,                            XK_F1,      spawn,          SHCMD( "zathura ~/help/KeyBindings.pdf" ) }, // dwm KeyBindings Cheatsheet
    { MODKEY|ControlMask,           XK_v,       spawn,          SHCMD( "zathura ~/help/VimShortcuts.pdf" ) }, // Neovim KeyBindings Cheatsheet
    { MODKEY|ControlMask,           XK_l,       spawn,          SHCMD( "zathura ~/help/lshort.pdf" ) }, // LaTeX Short Start Guide
    { MODKEY,                       XK_n,       spawn,          SHCMD( "zathura ~/Documents/MartialArts/Notebook/Notebook.pdf" ) },
// ScratchPads
	{ 0,            			    XK_F9,  	togglescratch,  {.ui = 0 } },
	{ 0,            			    XK_F10,	    togglescratch,  {.ui = 1 } },
	{ 0,            			    XK_F11,	    togglescratch,  {.ui = 2 } },
// Layout related KeyBindings
	{ ALTKEY,                       XK_t,       setlayout,      {.v = &layouts[0]} }, // Tile layout
	{ ALTKEY,                       XK_f,       setlayout,      {.v = &layouts[1]} }, // Floating layout
	{ ALTKEY,                       XK_m,       setlayout,      {.v = &layouts[2]} }, // Monocle layout, Windows-style
	{ MODKEY,                       XK_space,   setlayout,      {0} }, // switch between last 2 layouts
	{ MODKEY|ShiftMask,             XK_space,   togglefloating, {0} }, // makes current window floating
	{ MODKEY|ShiftMask,             XK_f,       togglefullscr,  {0} }, // make current window fullscreen
	{ MODKEY,                       XK_minus,   setgaps,        {.i = -5 } }, // Decrease gap size
	{ MODKEY|ShiftMask,             XK_minus,   setgaps,        {.i = GAP_RESET } },
	{ MODKEY,                       XK_equal,   setgaps,        {.i = +5 } }, // Increase gap size
	{ MODKEY|ShiftMask,             XK_equal,   setgaps,        {.i = GAP_TOGGLE} },
// Stack related KeyBindings
	{ MODKEY|ALTKEY,                XK_Return,  zoom,           {0} }, // pull the current window (in the stack) into the Master space
	{ MODKEY,                       XK_j,       focusstack,     {.i = +1 } }, // move focus up the stack
	{ MODKEY,                       XK_k,       focusstack,     {.i = -1 } }, // move focus down the stack
	{ MODKEY,                       XK_i,       incnmaster,     {.i = +1 } }, // Increase # of windows in Master area
	{ MODKEY,                       XK_d,       incnmaster,     {.i = -1 } }, // Decrease # of windows in Master area
	{ MODKEY,                       XK_h,       setmfact,       {.f = -0.05} }, // Decrease Master area size
	{ MODKEY,                       XK_l,       setmfact,       {.f = +0.05} }, // Increase Master area size
	{ MODKEY,                       XK_0,       view,           {.ui = ~0 } }, // View all open windows
	{ MODKEY|ShiftMask,             XK_0,       tag,            {.ui = ~0 } }, // Place current window in all tags

// Media Related Keybindings
    { 0,                            XF86XK_AudioLowerVolume,            spawn,      {.v = downvol } },
    { MODKEY,                       XF86XK_AudioLowerVolume,            spawn,      {.v = midvol } },
    { MODKEY|ALTKEY,                XF86XK_AudioLowerVolume,            spawn,      {.v = vol25 } },
	{ 0,                            XF86XK_AudioRaiseVolume,            spawn,      {.v = upvol   } },
	{ MODKEY,                       XF86XK_AudioRaiseVolume,            spawn,      {.v = maxvol   } },
	{ MODKEY|ALTKEY,                XF86XK_AudioRaiseVolume,            spawn,      {.v = vol75   } },
	{ 0,                            XF86XK_AudioMute,                   spawn,      {.v = mutevol } },
	{ 0,                            XF86XK_AudioMicMute,	            spawn,      {.v = mutemic } },
	{ 0,                            XF86XK_AudioPrev,                   spawn,      {.v = (const char*[]){ "rmpc", "prev", NULL } } },
	{ 0,                            XF86XK_AudioNext,                   spawn,      {.v = (const char*[]){ "rmpc",  "next", NULL } } },
	{ ShiftMask,                    XF86XK_AudioPlay,                   spawn,      {.v = (const char*[]){ "rmpc", "pause", NULL } } },
	{ 0,                            XF86XK_AudioPlay,                   spawn,      {.v = (const char*[]){ "rmpc", "play", NULL } } },
	{ 0,                            XF86XK_AudioStop,                   spawn,      {.v = (const char*[]){ "rmpc", "stop", NULL } } },
    { 0,                            XK_F7,                              spawn,      SHCMD( "feh --bg-fill --random ~/Pictures/Wallpapers/" ) }, // Set a new Random wallpaper
	TAGKEYS(                        XK_1,                       0)
	TAGKEYS(                        XK_2,                       1)
	TAGKEYS(                        XK_3,                       2)
	TAGKEYS(                        XK_4,                       3)
	TAGKEYS(                        XK_5,                       4)
//	TAGKEYS(                        XK_6,                       5)
//	TAGKEYS(                        XK_7,                       6)
//	TAGKEYS(                        XK_8,                       7)
//	TAGKEYS(                        XK_9,                       8)
	{ MODKEY,                       XK_comma,   focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,  focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,   tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,  tagmon,         {.i = +1 } },

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button1,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
