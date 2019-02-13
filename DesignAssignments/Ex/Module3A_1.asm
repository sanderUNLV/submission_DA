;
; Program2_0.asm: The first AVR Assembly program that 
; adds some data and stores SUM in SRAM location 0x300.
;
; Created: 07/20/2017 07:26:35 
; Author : 
;
.include<m328pdef.inc>
.cseg

.EQU SUM = 0x300	;SRAM loc $300 for SUM

.org 0x00			;start at address 0
	LDI R16, 0x25		;R16 = 0x25
	LDI R17, $34		;R17 = 0x34
	LDI R18, 0b00110001	;R18 = 0x31
	ADD R16, R17		;add R17 to R16
	ADD R16, R18		;add R18 to R16
	LDI R17, 11			;R17 = 0x0B
	ADD R16, R17		;add R17 to R16
	STS SUM, R16		;save the SUM in loc $300
HERE: JMP HERE			;stay here forever
