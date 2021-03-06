; Author : ert

.include <m328pdef.inc>

.ORG 0		
.SET	A = 7
.SET	B = 255
						;LINE 8
	LDI R23, A			;LOAD IN A
	LDI R17, B			;LOAD IN B
	LDI R21, 0			;ZERO FOR DECREMENTER REFERENCE
	LDI R16, 8			;DECREMENT COUNT 'FOR 8 BITS TO SHIFT OVER'
L1:	LSR R17				;RIGHT SHIFT BOTTOM NUMBER TO CHECK CARRY BIT
	BRCS BOTTOMCARRY	;BRANCH FOR 0 OR 1 CASE
	DEC R16				;DECREMENT AMOUNT OF SHIFT ADDS FOR 0 CASE
	BRCC NOCARRY		;BRANCH IF THE CARRY BIT IS NOT SET			
BOTTOMCARRY: 
	DEC R16				;DECREMENT AMOUNT OF SHIFT ADDS FOR 1 CASE
NOCARRY:
	LSL R23				;SHIFT TOP NUMBER BEING ADDED LEFT BY 1 BIT
	ADD R22, R23		;RUNNING SUM, LINE 18
	CPI R17, 0			;COMPARE IF ALL BITS HAVE BEEN SHIFTED AND ACCOUNTED FOR
	BRNE L1				;IF NOT THROUGH ALL THE BOTTOM BITS, EVALUATE OF THE BOTTOM LSB IS A 0 OR 1
	NOP