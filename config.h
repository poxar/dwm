/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 0;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int rmaster            = 0;        /* 1 means master-area is initially on the right */
static const unsigned int gappih    = 20;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 20;       /* vert inner gap between windows */
static const unsigned int gappoh    = 20;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 20;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 1;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "FiraCode Nerd Font Light:size=10" };

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
	{ "firefox",         "Toolkit",  "Picture-in-Picture", (1<<9)-1,  0,    1,           -1 },
	{ "steam",           NULL,       NULL,       1 << 6,       0,           0,           -1 },
	{ "Spotify",         NULL,       NULL,       1 << 7,       0,           0,           -1 },
	{ "discord",         NULL,       NULL,       1 << 8,       0,           0,           -1 },
	{ "TelegramDesktop", NULL,       NULL,       1 << 8,       0,           0,           -1 },
	{ "Signal",          NULL,       NULL,       1 << 8,       0,           0,           -1 },
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

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "rofi", "-show", "combi", NULL };
static const char *termcmd[]  = { "alacritty", NULL };

static const char *actcmd[] = { "rofi", "-show", "dwm", "-modes", "dwm:Code/dwm/actions.sh", NULL };
static const char *sesscmd[] = { "rofi", "-show", "sessions", "-modes", "sessions:Code/dwm/sessions.sh", NULL };
static const char *notecmd[] = { "alacritty", "--working-directory", "Notes", "-e", "nvim", "Eingang.md", NULL};
static const char *filecmd[] = { "rofi", "-show", "files", "-modes", "files:Code/dwm/files.sh", NULL};

#include "movestack.c"
static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_c,      spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_semicolon, spawn,       {.v = actcmd } },
	{ MODKEY,                       XK_s,      spawn,          {.v = sesscmd } },
	{ MODKEY,                       XK_n,      spawn,          {.v = notecmd } },
	{ MODKEY,                       XK_f,      spawn,          {.v = filecmd } },
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
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
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
	{ MODKEY,                       XK_p,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_p,      tag,            {.ui = ~0 } },

	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkClientWin,         Mod1Mask,       Button1,        movemouse,      {0} },
	{ ClkClientWin,         Mod1Mask,       Button2,        togglefloating, {0} },
	{ ClkClientWin,         Mod1Mask,       Button3,        resizemouse,    {0} },
	{ ClkClientWin,         Mod1Mask|ShiftMask, Button1,    resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

