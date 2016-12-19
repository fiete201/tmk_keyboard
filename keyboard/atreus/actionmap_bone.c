#include "actionmap.h"
#include "action_code.h"
#include "actionmap_common.h"
#include "print.h"
#include "bootloader.h"

/* id for user defined functions */
enum function_id {
  BOOTLOADER,
};

/*
 * Actions
 */
#define AC_FN0 ACTION_LAYER_MOMENTARY(1)  // to Fn overlay
#define AC_FN1 ACTION_LAYER_ON(2, 1)  // switch to layer 2
#define AC_FN2 ACTION_LAYER_OFF(2, 1)  // switch back to layer 0
#define AC_FN3 ACTION_FUNCTION(BOOTLOADER)
#define AC_FN4 ACTION_LAYER_MOMENTARY(3)
#define AC_FN5 ACTION_LAYER_MOMENTARY(4)

const action_t PROGMEM actionmaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = ACTIONMAP( \
		  J, D, U, A, X, P, H, L, M, W, \
		  C, T, I, E, O, B, N, R, S, G, \
		  F, V, LBRACKET, QUOTE, SCOLON, Z, Y, COMM, DOT, K, \
		  FN4, TAB, LGUI, LSFT, FN5, LCTL, LALT, \
		  SPC, FN0, MINUS, Q, ENT),
  /* 1: punctuation and numbers */
  [1] = ACTIONMAP( \
		  s(1), ra(Q), UP, ra(7), ra(0), \
		  PGUP, 7, 8, 9, s(RBRACKET), \
		  BSLASH, LEFT, DOWN, RIGHT, s(4), \
		  PGDN, 4, 5, 6, RBRACKET, \
		  ra(8), ra(9), s(8), s(9), s(6), \
		  s(EQUAL), 1, 2, 3, ra(MINUS),	\
		  FN1, INS, LGUI, LSFT, BSPC, LCTL, \
		  LALT, SPC, FN0, DOT, 0, s(0)), 
  /* 2: arrows and function keys */
  [2] = ACTIONMAP( \
		  INS, HOME, UP, END, PGUP, \
		  UP, F7, F8, F9, F10, \
		  DEL, LEFT, DOWN, RIGHT, PGDN,	\
		  DOWN, F4, F5, F6, F11, \
		  NO, _VOLUP, NO, NO, FN3, \
		  NO, F1, F2, F3, F12, \
		  NO, _VOLDOWN, LGUI, LSFT, BSPC, LCTL,	\
		  LALT, SPC, FN2, PSCREEN, SLCK, PAUSE),
  /* 3: Bone level 3 */
  [3] = ACTIONMAP( \
		  NO, SLSH, s(8), s(9), GRV, \
		  s(1), NUBS, s(NUBS), s(0), s(6), \
		  ra(MINUS), s(7), ra(7), ra(0), s(RBRACKET), \
		  s(MINUS), s(8), s(9), SLASH, s(DOT), \
		  BSLASH, s(4), ra(NUBS), ra(RBRACKET), s(EQUAL), \
		  RBRACKET, s(5), s(2), s(BSLASH), s(COMMA), \
		  TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, \
		  TRNS, TRNS, TRNS, TRNS, COMMA, TRNS),
  /* 4: Neo Level 4 */
  [4] = ACTIONMAP( \
		  s(INS), BSPC, UP, DEL, NO, \
		  PGUP, F7, F8, F9, F10, \
		  NO, LEFT, DOWN, RIGHT, NO, \
		  PGDN, F4, F5, F6, F11, \
		  ESC, TAB, NO, NO, NO, \
		  NO, F1, F2, F3, F12, \
		  TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, \
		  TRNS, TRNS, TRNS, TRNS, TRNS, TRNS)

};

void bootloader() {
  clear_keyboard();
  print("\n\nJump to bootloader... ");
  _delay_ms(250);
#ifdef TEENSY
  cli();
  UDCON = 1;
  USBCON = (1<<FRZCLK);  // disable USB
  UCSR1B = 0;
  _delay_ms(5);
  EIMSK = 0; PCICR = 0; SPCR = 0; ACSR = 0; EECR = 0; ADCSRA = 0;
  TIMSK0 = 0; TIMSK1 = 0; TIMSK3 = 0; TIMSK4 = 0; UCSR1B = 0; TWCR = 0;
  DDRB = 0; DDRC = 0; DDRD = 0; DDRE = 0; DDRF = 0; TWCR = 0;
  PORTB = 0; PORTC = 0; PORTD = 0; PORTE = 0; PORTF = 0;
  asm volatile("jmp 0x7E00");
#else
  *(uint16_t *)0x0800 = 0x7777; // these two are a-star-specific
  bootloader_jump();
  print("not supported.\n");
#endif
}

void action_function(keyrecord_t *record, uint8_t id, uint8_t opt)
{
  if (id == BOOTLOADER) {
    bootloader();
  }
}
