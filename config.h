/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>
#define TERMINAL "st"
#define TERMCLASS "St"
#define MODKEY Mod4Mask
#define ALTKEY Mod1Mask

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const Gap default_gap        = {.isgap = 0, .realgap = 0, .gappx = 0};
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Hack Regular:size=11", "Hack Nerd Font Mono:size=17" };
static char norm_fg[]           = "#eee8d5";
static char norm_bg[]           = "#000000"; // 2e3440
static char norm_border[]       = "#6c7c80";
static char sel_fg[]            = "#eee8d5";
static char sel_bg[]            = "#5e81ac";
static char sel_border[]        = "#bf616a";
static const char *colors[][3] = {
//                      fg           bg           border
    [SchemeNorm] = { norm_fg, norm_bg, norm_border },
    [SchemeSel]  = { sel_fg,  sel_bg,  sel_border },
	[SchemeTitle] = { sel_fg, norm_bg, norm_bg },
};

/* tagging */
static const char *tags[] = { "", "󱍢", "", "", "󰣙", "" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class                instance        title       tags mask     isfloating   monitor */
	{ "Gimp",               NULL,           NULL,       1<<3,            1,           -1 },
	{ "Firefox",            NULL,           NULL,       1<<1,            0,           -1 },
	{ "Chromium",           NULL,           NULL,       1<<1,            0,           -1 },
    { "qalculate-qt",       NULL,           NULL,       0,               1,           -1 },
	{ "vlc",                NULL,           NULL,       1<<5,            1,           -1 },
	{  "libreoffice-startcenter",           NULL,       NULL,      1<<2,            0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "",      tile },    /* first entry is default */
	{ "󱏿",      NULL },    /* no layout function means floating behavior */
	{ "󰈈",      monocle },
};

/* key definitions */

#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", NULL };
static const char *termcmd[] = { "st", NULL};
static const char *web[] = { "chromium", NULL };
static const char *altweb[] = { "firefox", NULL };
static const char *calc[] = { "qalculate-qt", NULL };

/* add functionality for the volume controls */
static const char *upvol[]      = { "/usr/bin/wpctl",   "set-volume", "@DEFAULT_AUDIO_SINK@",      "2%+",    NULL };
static const char *downvol[]    = { "/usr/bin/wpctl",   "set-volume", "@DEFAULT_AUDIO_SINK@",      "2%-",    NULL };
static const char *mutevol[]    = { "/usr/bin/wpctl",   "set-mute",   "@DEFAULT_AUDIO_SINK@",      "toggle", NULL };
static const char *mutemic[]    = { "/usr/bin/wpctl",   "set-mute",   "@DEFAULT_AUDIO_SOURCE@",    "toggle", NULL };

static const Key keys[] = {
	/* modifier                     key         function        argument */
	{ MODKEY,                       XK_Return,  spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,       togglebar,      {0} }, // turn the top bar on/off
    { MODKEY,                       XK_c,       spawn,          {.v = calc } },
	{ MODKEY,                       XK_g,       spawn,          SHCMD( "gimp" ) }, // GNU Image Manipulation Program
    { MODKEY,                       XK_o,       spawn,          SHCMD( "libreoffice" ) },
	{ MODKEY,                       XK_q,       killclient,     {0} }, // close the window with focus
	{ MODKEY,                       XK_r,       spawn,          {.v = dmenucmd } },
    { MODKEY,                       XK_v,       spawn,          SHCMD( "vlc" ) }, // VLC media player
    { MODKEY|ControlMask,           XK_v,       spawn,          SHCMD( "zathura ~/help/VimShortcuts.pdf" ) },
	{ MODKEY,                       XK_w,       spawn,          {.v = web } },
	{ MODKEY|ShiftMask,             XK_w,       spawn,          {.v = altweb } },
    { MODKEY,                       XK_x,       spawn,          SHCMD( "slock" ) }, // Screen locker
	{ MODKEY,                       XK_Tab,     view,           {0} }, //
// Layout related KeyBindings
	{ ALTKEY,                       XK_t,       setlayout,      {.v = &layouts[0]} }, // Tile layout
	{ ALTKEY,                       XK_f,       setlayout,      {.v = &layouts[1]} }, // Floating layout
	{ ALTKEY,                       XK_m,       setlayout,      {.v = &layouts[2]} }, // Monocle layout, essentially Windows-style
	{ MODKEY,                       XK_space,   setlayout,      {0} }, // switch between last 2 layouts
	{ MODKEY|ShiftMask,             XK_space,   togglefloating, {0} }, // makes current window floating
// Stack related KeyBindings
	{ MODKEY|ALTKEY,                XK_Return,  zoom,           {0} }, // pull the current window (in the stack) into the Master space
	{ MODKEY,                       XK_j,       focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,       focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,       incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,       incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,       setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,       setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_0,       view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,       tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_minus,   setgaps,        {.i = -5 } },
	{ MODKEY|ShiftMask,             XK_minus,   setgaps,        {.i = GAP_RESET } },
	{ MODKEY,                       XK_equal,   setgaps,        {.i = +5 } },
	{ MODKEY|ShiftMask,             XK_equal,   setgaps,        {.i = GAP_TOGGLE} },
    { MODKEY|ShiftMask,             XK_BackSpace,   quit,       {0} },
 	{ MODKEY,			            XK_F9,          spawn,      {.v = (const char*[]){ "mounter", NULL } } },
	{ MODKEY|ALTKEY,			    XK_F9,          spawn,      {.v = (const char*[]){ "unmounter", NULL } } },
    { MODKEY,                       XK_p,           spawn,      SHCMD( "maim -i $(xdotool getactivewindow) ~/Pictures/Screenshots/window-$(date '+%y%m%d-%H%M-%S').png && notify-send \"Screenshot Taken\"" ) },
    { MODKEY|ShiftMask,             XK_p,           spawn,      SHCMD( "maim ~/Pictures/Screenshots/screen-$(date '+%y%m%d-%H%M-%S').png && notify-send \"Screenshot Taken\"" ) },
    { MODKEY|ControlMask,           XK_p,           spawn,      SHCMD( "ffmpeg -f video4linux2 -s 640x480 -i /dev/video0 -ss 0:0:2 -frames 1 ~/Pictures/Screenshots/selfie.jpg && notify-send \"Photo Taken\"" ) },
    { 0, XF86XK_Launch1,                            spawn,      SHCMD( "zathura ~/help/KeyBindings.pdf" ) },
	{ 0, XF86XK_AudioLowerVolume,                   spawn,      {.v = downvol } },
	{ 0, XF86XK_AudioMute,                          spawn,      {.v = mutevol } },
	{ 0, XF86XK_AudioRaiseVolume,                   spawn,      {.v = upvol   } },
	{ 0, XF86XK_AudioMicMute,	                    spawn,      {.v = mutemic } },
	{ 0, XF86XK_AudioPrev,                          spawn,      {.v = (const char*[]){ "rmpc", "prev", NULL } } },
	{ 0, XF86XK_AudioNext,                          spawn,      {.v = (const char*[]){ "rmpc",  "next", NULL } } },
	{ ShiftMask, XF86XK_AudioPlay,                  spawn,      {.v = (const char*[]){ "rmpc", "pause", NULL } } },
	{ 0, XF86XK_AudioPlay,                          spawn,      {.v = (const char*[]){ "rmpc", "play", NULL } } },
	{ 0, XF86XK_AudioStop,                          spawn,      {.v = (const char*[]){ "rmpc", "stop", NULL } } },
	TAGKEYS(                        XK_1,                       0)
	TAGKEYS(                        XK_2,                       1)
	TAGKEYS(                        XK_3,                       2)
	TAGKEYS(                        XK_4,                       3)
	TAGKEYS(                        XK_5,                       4)
	TAGKEYS(                        XK_6,                       5)
	TAGKEYS(                        XK_7,                       6)
	TAGKEYS(                        XK_8,                       7)
	TAGKEYS(                        XK_9,                       8)
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
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
