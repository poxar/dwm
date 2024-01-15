/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 4;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int rmaster            = 1;        /* 1 means master-area is initially on the right */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 1;    /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static const unsigned int gappih    = 20;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 20;       /* vert inner gap between windows */
static const unsigned int gappoh    = 20;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 20;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 1;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "FiraCode Nerd Font Light:size=10" };
static const char dmenufont[]       = "FiraCode Nerd Font Light:size=10";

static const char col_fg[] = "#f4e4e1";
static const char col_bg[] = "#000000";
static const char col_hi[] = "#e3bcb5";

static const char *colors[][3]      = {
	/*               fg      bg      border   */
	[SchemeNorm] = { col_fg, col_bg, col_fg },
	[SchemeSel]  = { col_bg, col_hi, col_hi },
};

/* tagging */
static const char *tags[] = { "q", "w", "e", "r", "t", "y", "u", "i", "o" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class             instance    title       tags mask     iscentered   isfloating   monitor */
	{ "skim",            NULL,       NULL,       0,            1,           1,           -1 },
	{ "steam",           NULL,       NULL,       1 << 5,       0,           0,           -1 },
	{ "discord",         NULL,       NULL,       1 << 6,       0,           0,           -1 },
	{ "TelegramDesktop", NULL,       NULL,       1 << 7,       0,           0,           -1 },
	{ "Signal",          NULL,       NULL,       1 << 7,       0,           0,           -1 },
	{ "firefox",         "Navigator",NULL,       1 << 8,       0,           0,           -1 },
	{ "firefox",         "Toolkit",  "Picture-in-Picture", (1<<9)-1,  0,    0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* helper to update the statusbar after running the command */
#define SCMD(cmd) { "/bin/sh", "-c", cmd "&& pkill --signal USR1 dwmstatus.sh", NULL }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_bg, "-nf", col_fg, "-sb", col_hi, "-sf", col_bg, NULL };
static const char *termcmd[]  = { "alacritty", NULL };

#define FLOAT_TERM "alacritty", "--class", "skim", "--option", "window.opacity=1"

static const char *clipcmd[] = { FLOAT_TERM, "--command", "clipcat-menu", NULL };
static const char *actcmd[] = { FLOAT_TERM, "--command", "Code/dwm/actions.sh", NULL };

static const char *scrotcmd[] = {
  "sh", "-c", "cd ~/Pictures/Screenshots && scrot -fs "
    "-e 'xclip -selection clipboard -t image/png -i $f'",
  NULL
};

static const char *scrotscreencmd[] = {
  "sh", "-c", "cd ~/Pictures/Screenshots && scrot "
    "-e 'xclip -selection clipboard -t image/png -i $f'",
  NULL
};

static const char *cmd_media_next[] = { "playerctl", "-i", "firefox", "next", NULL };
static const char *cmd_media_prev[] = { "playerctl", "-i", "firefox", "previous", NULL };
static const char *cmd_media_play[] = { "playerctl", "-i", "firefox", "play-pause", NULL };

static const char *cmd_mic_mute[] = SCMD("pamixer --default-source --toggle-mute");
static const char *cmd_vol_mute[] = SCMD("pamixer --toggle-mute");
static const char *cmd_vol_ndec[] = SCMD("pamixer --decrease 5");
static const char *cmd_vol_sdec[] = SCMD("pamixer --decrease 1");
static const char *cmd_vol_ninc[] = SCMD("pamixer --increase 5");
static const char *cmd_vol_sinc[] = SCMD("pamixer --increase 1");

static const char *cmd_bright_up[]   = { "brightnessctl", "set", "+10%", NULL };
static const char *cmd_bright_down[] = { "brightnessctl", "set", "10%-", NULL };

static const char *cmd_calender[] = {
  FLOAT_TERM,
  "--option", "window.dimensions.columns=64",
  "--option", "window.dimensions.lines=10",
  "--command", "sh", "-c",
  "echo '' && cal -n3 && read -n1",
  NULL
};

#include "movestack.c"
static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_p,      spawn,          {.v = clipcmd } },
	{ MODKEY,                       XK_v,      spawn,          {.v = clipcmd } },
	{ MODKEY,                       XK_g,      spawn,          {.v = actcmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_c,      spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_h,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_l,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
        { Mod1Mask,                     XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_Tab,    tag,            {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_m,      togglermaster,  {0} },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_q,                      0)
	TAGKEYS(                        XK_w,                      1)
	TAGKEYS(                        XK_e,                      2)
	TAGKEYS(                        XK_r,                      3)
	TAGKEYS(                        XK_t,                      4)
	TAGKEYS(                        XK_y,                      5)
	TAGKEYS(                        XK_u,                      6)
	TAGKEYS(                        XK_i,                      7)
	TAGKEYS(                        XK_o,                      8)

        { 0,                            XK_Print,  spawn,          {.v = scrotcmd } },
        { ShiftMask,                    XK_Print,  spawn,          {.v = scrotscreencmd } },

	{ 0,                     XF86XK_AudioPrev, spawn,          {.v = cmd_media_prev } },
	{ 0,                     XF86XK_AudioNext, spawn,          {.v = cmd_media_next } },
	{ 0,                     XF86XK_AudioPlay, spawn,          {.v = cmd_media_play } },

	{ 0,                  XF86XK_AudioMicMute, spawn,          {.v = cmd_mic_mute } },
	{ 0,                     XF86XK_AudioMute, spawn,          {.v = cmd_vol_mute } },
	{ 0,              XF86XK_AudioLowerVolume, spawn,          {.v = cmd_vol_ndec } },
	{ ShiftMask,      XF86XK_AudioLowerVolume, spawn,          {.v = cmd_vol_sdec } },
	{ 0,              XF86XK_AudioRaiseVolume, spawn,          {.v = cmd_vol_ninc } },
	{ ShiftMask,      XF86XK_AudioRaiseVolume, spawn,          {.v = cmd_vol_sinc } },

	{ 0,               XF86XK_MonBrightnessUp, spawn,          {.v = cmd_bright_up } },
	{ 0,             XF86XK_MonBrightnessDown, spawn,          {.v = cmd_bright_down } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        spawn,          {.v = cmd_calender } },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkClientWin,         Mod1Mask,       Button1,        movemouse,      {0} },
	{ ClkClientWin,         Mod1Mask,       Button2,        togglefloating, {0} },
	{ ClkClientWin,         Mod1Mask,       Button3,        resizemouse,    {0} },
	{ ClkClientWin,         Mod1Mask|ShiftMask,       Button1,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

