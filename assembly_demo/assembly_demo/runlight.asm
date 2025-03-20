;
; RunLight.asm
;
; Created: 16-3-2021 10:52:45
; Author : hajlinde
;
.include "m128def.inc"

.def	delayReg = r19		; Register for delay time [ms]
.def	ledReg = r20		; Register for LED pattern
.def	dirReg = r21		; Bit 0 indicates direction, 0 = left, 1 = right

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


; Runlight
runlight_init:
; Set port A to all output
	ldi r16, $FF
	out DDRA, r16
; Initialize LED register and direction register
	ldi ledReg, $01		; Start pattern for the runlight
	clr dirReg			; Clears the register so direction is left
; Show LED value on port A
	out PORTA, ledReg

runlight:
; Delay 200 ms
	ldi delayReg, 200

runlight_loop:
	rcall delay_1ms
	dec delayReg
	brne runlight_loop	; Jump back to runlight_loop if delay is not zero yet

shift_leds:
; Shift the LED value 1 bit left or right
	sbrs dirReg, 0		; Skip next instruction if bit 0 is set
	breq shift_left		; Bit 0 is clear so shift left
shift_right:
	lsr ledReg			; Shift 1 bit to the right
	out PORTA, ledReg
	sbrs ledReg, 0		; If bit 0 is 1 then reverse direction
	rjmp runlight		; No reverse, jump back to delay
reverse_to_left:
	cbr dirReg, 1		; Clear bit 0 in dirReg to indicate direction left
	rjmp runlight		; Done, back to delay

shift_left:
	lsl ledReg			; Shift 1 bit to the left
	out PORTA, ledReg
	sbrs ledReg, 7		; If bit 7 is 1 then reverse direction
	rjmp runlight		; No reverse, jump back to delay
reverse_to_right:
	sbr dirReg, 1		; Set bit 0 in dirReg to indicate direction right
	rjmp runLight		; Done, back to delay


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
