;
; assembly_demo.asm
;
; Created: 20-3-2025 10:54:51
; Author : tieme
;

;.include "m128def.inc"

.cseg
.org	$0000
	rjmp	init

.cseg
.org	$0200
init:
	ldi r16, low(RAMEND)
	out SPL, r16
	ldi r16, high(RAMEND)
	out SPH, r16

main:
	ldi r16, $FF
	out DDRA, r16
	ldi r16, $01
	out PORTA, r16

loop:
	ldi r19, 200
	rcall delay
	inc r16
	out PORTA, r16
	rjmp loop


;r19 contains delay in milliseconds	
delay:
delay_1ms:
	ldi r18,9
delay_1ms_loop1:
	ldi r17,222
delay_1ms_loop2:
	nop
	dec r17
	brne delay_1ms_loop2
	dec r18
	brne delay_1ms_loop1
	dec r19
	brne delay
	ret