/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const Gap default_gap        = {.isgap = 1, .realgap = 10, .gappx = 10};
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Hack Nerd Font:size=10", "Ubuntu Nerd Font:size=10", "Font Awesome 6 Free:size=10", "Font Awesome 6 Free Solid:size=10" };
static const char dmenufont[]       = "Ubuntu Mono:size=10";
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#ff033e";
static char selbgcolor[]            = "#005577";
static const char *colors[][3] = {
       /*               fg           bg           border   */
    [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
    [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
	[SchemeTitle]  = { "#ffffff", "#1973d1",  "#000000"  },
};

/* tagging */
/* static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7" }; */
  static const char *tags[] = { "", "", "", "","", "", "" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class                instance        title       tags mask     isfloating   monitor */
	{ "Gimp",               NULL,           NULL,       6,            1,           -1 },
	{ "Brave-browser",      NULL,           NULL,       2,            0,           -1 },
    { "qalculate-qt",       NULL,           NULL,       0,            1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "~*~",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define ALTKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define TERMINAL "st"
#define TERMCLASS "St"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selfgcolor, "-sf", normbordercolor, NULL };
static const char *termcmd[] = { "st", NULL };
static const char *alttermcmd[] = { "kitty", NULL};
static const char *web[] = { "brave", NULL };
static const char *files[] = { "thunar", NULL };
static const char *calc[] = { "qalculate-qt", NULL };
static const char *office[] = { "libreoffice", NULL };
static const char *vlc[] = { "vlc", NULL };

/* add functionality for the volume controls */
static const char *upvol[]      = { "/usr/bin/wpctl",   "set-volume", "@DEFAULT_AUDIO_SINK@",      "5%+",      NULL };
static const char *downvol[]    = { "/usr/bin/wpctl",   "set-volume", "@DEFAULT_AUDIO_SINK@",      "5%-",      NULL };
static const char *mutevol[]    = { "/usr/bin/wpctl",   "set-mute",   "@DEFAULT_AUDIO_SINK@",      "toggle",   NULL };
static const char *mutemic[]    = { "/usr/bin/wpctl",   "set-mute",   "@DEFAULT_AUDIO_SOURCE@",     "toggle",  NULL };

static const Key keys[] = {
	/* modifier                     key         function        argument */
	{ MODKEY,                       XK_r,       spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return,  spawn,          {.v = termcmd } },
    { MODKEY|ALTKEY,                XK_Return,  spawn,          {.v = alttermcmd } },
	{ MODKEY,                       XK_w,       spawn,          {.v = web } },
	{ MODKEY,                       XK_t,       spawn,          {.v = files } },
    { MODKEY,                       XK_o,       spawn,          {.v = office } },
    { MODKEY,                       XK_v,       spawn,          {.v = vlc } },
    { MODKEY|ControlMask,           XK_v,       spawn,          SHCMD( "zathura ~/help/VimShortcuts.pdf" ) },
    { MODKEY,                       XK_c,       spawn,          {.v = calc } },
	{ MODKEY,                       XK_x,       spawn,          SHCMD( "slock" ) },
	{ MODKEY,                       XK_b,       togglebar,      {0} },
	{ MODKEY,                       XK_j,       focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,       focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,       incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,       incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,       setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,       setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return,  zoom,           {0} },
	{ MODKEY,                       XK_Tab,     view,           {0} },
	{ MODKEY,                       XK_q,       killclient,     {0} },
	{ ALTKEY,                       XK_t,       setlayout,      {.v = &layouts[0]} },
	{ ALTKEY,                       XK_f,       setlayout,      {.v = &layouts[1]} },
	{ ALTKEY,                       XK_m,       setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,   setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,   togglefloating, {0} },
	{ MODKEY,                       XK_0,       view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,       tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,   focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,  focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,   tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,  tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_minus,   setgaps,        {.i = -5 } },
	{ MODKEY,                       XK_equal,   setgaps,        {.i = +5 } },
	{ MODKEY|ShiftMask,             XK_minus,   setgaps,        {.i = GAP_RESET } },
	{ MODKEY|ShiftMask,             XK_equal,   setgaps,        {.i = GAP_TOGGLE} },
	TAGKEYS(                        XK_1,                       0)
	TAGKEYS(                        XK_2,                       1)
	TAGKEYS(                        XK_3,                       2)
	TAGKEYS(                        XK_4,                       3)
	TAGKEYS(                        XK_5,                       4)
	TAGKEYS(                        XK_6,                       5)
	TAGKEYS(                        XK_7,                       6)
	TAGKEYS(                        XK_8,                       7)
	TAGKEYS(                        XK_9,                       8)
	{ MODKEY|ShiftMask,             XK_q,                       quit,           {0} },
    { 0,                            XK_Print,                   spawn,  SHCMD( "maim -i $(xdotool getactivewindow) ~/Pictures/Screenshots/window-$(date '+%y%m%d-%H%M-%S').png" ) },
    { ShiftMask,                    XK_Print,                   spawn,  SHCMD( "maim ~/Pictures/Screenshots/screen-$(date '+%y%m%d-%H%M-%S').png" ) },
    { ControlMask,                  XK_Print,                   spawn,  SHCMD( "ffmpeg -f video4linux2 -s 640x480 -i /dev/video0 -ss 0:0:2 -frames 1 ~/Pictures/Screenshots/out.jpg" ) },
    { 0,                            XF86XK_Launch1,             spawn,  SHCMD( "zathura ~/help/KeyBindings.pdf" ) },
	{ 0,                            XF86XK_AudioLowerVolume,    spawn,  {.v = downvol } },
	{ 0,                            XF86XK_AudioMute,           spawn,  {.v = mutevol } },
	{ 0,                            XF86XK_AudioRaiseVolume,    spawn,  {.v = upvol   } },
	{ 0,				            XF86XK_AudioMicMute,	    spawn,  {.v = mutemic } },
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
