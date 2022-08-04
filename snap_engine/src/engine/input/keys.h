#pragma once

#include <SDL2/SDL.h>

enum ButtonState : uint8_t
{
	ButtonReleased = SDL_RELEASED,
	ButtonPressed = SDL_PRESSED
};

enum MouseButton : uint8_t
{
	MouseButtonLeft = SDL_BUTTON_LEFT,
	MouseButtonRight = SDL_BUTTON_RIGHT,
	MouseButtonMiddle = SDL_BUTTON_MIDDLE,
	MouseButtonX1 = SDL_BUTTON_X1,
	MouseButtonX2 = SDL_BUTTON_X2,
};

enum KeyState : uint8_t
{
	KeyReleased = SDL_RELEASED,
	KeyPressed = SDL_PRESSED
};

enum Key : int32_t
{
	/* The unknown key */
	Unknown = SDLK_UNKNOWN,

	/* Printable keys */
	Space =             SDLK_SPACE,
	Apostrophe =        SDLK_QUOTE, /* ' */
	Comma =             SDLK_COMMA, /* , */
	Minus =             SDLK_MINUS, /* - */
	Period =            SDLK_PERIOD, /* . */
	Slash =             SDLK_SLASH, /* / */
	KB_0 =              SDLK_0,
	KB_1 =              SDLK_1,
	KB_2 =              SDLK_2,
	KB_3 =              SDLK_3,
	KB_4 =              SDLK_4,
	KB_5 =              SDLK_5,
	KB_6 =              SDLK_6,
	KB_7 =              SDLK_7,
	KB_8 =              SDLK_8,
	KB_9 =              SDLK_9,
	Semicolon =         SDLK_SEMICOLON, /* ; */
	Equals =            SDLK_EQUALS, /* = */
	A =                 SDLK_a,
	B =                 SDLK_b,
	C =                 SDLK_c,
	D =                 SDLK_d,
	E =                 SDLK_e,
	F =                 SDLK_f,
	G =                 SDLK_g,
	H =                 SDLK_h,
	I =                 SDLK_i,
	J =                 SDLK_j,
	K =                 SDLK_k,
	L =                 SDLK_l,
	M =                 SDLK_n,
	N =                 SDLK_n,
	O =                 SDLK_o,
	P =                 SDLK_p,
	Q =                 SDLK_q,
	R =                 SDLK_r,
	S =                 SDLK_s,
	T =                 SDLK_t,
	U =                 SDLK_u,
	V =                 SDLK_v,
	W =                 SDLK_w,
	X =                 SDLK_x,
	Y =                 SDLK_y,
	Z =                 SDLK_z,
	LeftBracket =       SDLK_LEFTBRACKET, /* [ */
	Backslash =         SDLK_BACKSLASH, /* \ */
	RightBracket =      SDLK_RIGHTBRACKET, /* ] */
	GraveAccent =       SDLK_BACKQUOTE, /* ` */

	/* Function keys */
	EscapeKey =         SDLK_ESCAPE,
	Enter =             SDLK_RETURN,
	Tab =               SDLK_TAB,
	Backspace =         SDLK_BACKSPACE,
	Insert =            SDLK_INSERT,
	Delete =            SDLK_DELETE,
	Right =             SDLK_RIGHT,
	Left =              SDLK_LEFT,
	Down =              SDLK_DOWN,
	Up =                SDLK_UP,
	PageUp =            SDLK_PAGEUP,
	PageDown =          SDLK_PAGEDOWN,
	Home =              SDLK_HOME,
	End =               SDLK_END,
	CapsLock =          SDLK_CAPSLOCK,
	ScrollLock =        SDLK_SCROLLLOCK,
	NumLock =           SDLK_NUMLOCKCLEAR,
	PrintScreen =       SDLK_PRINTSCREEN,
	Pause =             SDLK_PAUSE,
	F1 =                SDLK_F1,
	F2 =                SDLK_F2,
	F3 =                SDLK_F3,
	F4 =                SDLK_F4,
	F5 =                SDLK_F5,
	F6 =                SDLK_F6,
	F7 =                SDLK_F7,
	F8 =                SDLK_F8,
	F9 =                SDLK_F9,
	F10 =               SDLK_F10,
	F11 =               SDLK_F11,
	F12 =               SDLK_F12,
	F13 =               SDLK_F13,
	F14 =               SDLK_F14,
	F15 =               SDLK_F15,
	F16 =               SDLK_F16,
	F17 =               SDLK_F17,
	F18 =               SDLK_F18,
	F19 =               SDLK_F19,
	F20 =               SDLK_F20,
	F21 =               SDLK_F21,
	F22 =               SDLK_F22,
	F23 =               SDLK_F23,
	F24 =               SDLK_F24,
	KP_0 =              SDLK_KP_0,
	KP_1 =              SDLK_KP_1,
	KP_2 =              SDLK_KP_2,
	KP_3 =              SDLK_KP_3,
	KP_4 =              SDLK_KP_4,
	KP_5 =              SDLK_KP_5,
	KP_6 =              SDLK_KP_6,
	KP_7 =              SDLK_KP_7,
	KP_8 =              SDLK_KP_8,
	KP_9 =              SDLK_KP_9,
	KP_Decimal =        SDLK_KP_DECIMAL,
	KP_Divide =         SDLK_KP_DIVIDE,
	KP_Multiply =       SDLK_KP_MULTIPLY,
	KP_Subtract =       SDLK_KP_MEMSUBTRACT,
	KP_Add =            SDLK_KP_MEMADD,
	KP_Enter =          SDLK_KP_ENTER,
	KP_Equals =         SDLK_KP_EQUALS,
	LeftShift =         SDLK_LSHIFT,
	LeftControl =       SDLK_LCTRL,
	LeftAlt =           SDLK_LALT,
	//LeftSuper =       SDLK_LSUPER,
	RightShift =        SDLK_RSHIFT,
	RightControl =      SDLK_RCTRL,
	RightAlt =          SDLK_RALT,
	//RightSuper =      SDLK_RSUPER,
	Menu =              SDLK_MENU
};