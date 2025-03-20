;
; PatternLight.asm
;
; Created: 16-3-2021 11:13:19
; Author : hajlinde
;
.include "m128def.inc"

.def	delayReg = r19		; Register for delay time [ms]
.def	ledReg = r20		; Register for LED pattern
.def	entriesReg = r22	; Register that counts down lookup table entries

.cseg
.org	$0000
; Reset vector, execution starts here after reset
start:
	rjmp init		; Jump to initialization

; This is the space in program memory where the interrupt vector table would go

.cseg
.org	$0200
init:
; Initialize the stack pointer at high end of RAM so we can call subroutines
	ldi	r16, low(RAMEND)
	out SPL, r16
	ldi r16, high(RAMEND)
	out SPH, r16

main:
; Main entry point (after initialization)
; Jump to the section that we want


; Patternlight
patternlight_init:
; Set port A to all output
	ldi r16, $FF
	out DDRA, r16

patternlight_start:
	ldi entriesReg, lookup_table_entries	; Use this register to count down the entries
; Let the Z register low + high (= R30 and R31) point to the lookup table in program memory
; The * 2 converts the program memory address into a byte address
; Remember that program memory in the AVR is organized as a list of 16 words
	ldi ZH, high(lookup_table * 2)
	ldi ZL, low(lookup_table * 2)

patternlight_next:
; Load LED value from the lookup table and display on port A
	lpm ledReg, Z+		; Autoincrement the Z registers
	out PORTA, ledReg
; Load the delay time from the lookup table
	lpm delayReg, Z+	; Autoincrement the Z registers

patternlight_delay:
	rcall delay_1ms
	dec delayReg
	brne patternlight_delay		; Jump back until delayReg is zero
	dec entriesReg
	breq patternlight_start		; Start from the beginning of the lookup table
	rjmp patternlight_next		; Continue in the lookup table


; End of program (never reached)
loop_forever:
	rjmp loop_forever

; === Subroutines ===

delay_1ms:
; Delay 1 millisecond subroutine
; Uses R17 and R18 (so don't use those registers elsewhere)
; Values for r18 and r17 empirically determined (based on 8 MHz clock)
	ldi r18, 9
delay_1ms_loop1:
	ldi r17, 222
delay_1ms_loop2:
	nop						; No operation, just takes up some time
	dec r17					; Decrement r17
	brne delay_1ms_loop2	; Jump back to delay_1ms_loop2 if not yet zero
	dec r18					; Decrement r18
	brne delay_1ms_loop1	; Jump back to delay_1ms_loop1 if not yet zero
	ret						; Both r17 and r18 are zero, return from the subroutine

; === Data tables in program memory ===

; Define constants in program memory
; Each entry consists of two bytes:
;   LED pattern, time delay value [ms]
lookup_table:
.db 0b10000001, 200
.db 0b01000010, 150
.db 0b00100100, 100
.db 0b00011000, 50
.db 0, 150
.db $FF, 50
.db 0, 50
.db $FF, 50
.db 0, 250
.equ	lookup_table_entries = 9
