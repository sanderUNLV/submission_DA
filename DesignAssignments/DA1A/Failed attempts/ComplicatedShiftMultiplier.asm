; Author : Robert Sander

.include <m328pdef.inc>

.ORG 0		
.SET	A = 7
.SET	B = 255
	
	LDI R16, 8			;DECREMENT COUNT 'FOR 8 BITS TO SHIFT OVER'
	LDI R17, B			;LOAD IN B 'BOTTOM NUMBER'
	LDI R18, A			;LOAD IN A 'TOP NUMBER'
	LDI R19, A			;INITIAL A VALUE (BEFORE SHIFT) 'MULTIPLIED BY 1', RUNNING SUM
	LDI R21, 1			;LOAD 1 IN R21 FOR BASE OF 16BIT CARRY DURING ADD AND SHIFT
L1:	LSR R17				;RIGHT SHIFT BOTTOM NUMBER TO CHECK CARRY BIT
	BRCS BCARRY			;BRANCH FOR 0 OR 1 CASE
	DEC R16				;DECREMENT AMOUNT OF SHIFT ADDS FOR 0 CASE
	BRCC NOCARRY		;BRANCH IF THE CARRY BIT IS NOT SET		LINE 14
BCARRY: 
	DEC R16				;DECREMENT AMOUNT OF SHIFT ADDS FOR 1 CASE
NOCARRY:
	LSL R18				;SHIFT TOP NUMBER BEING ADDED LEFT BY 1 BIT
	ADD R19, R18		;RUNNING SUM, LINE 19
	BRCS CARRY_16BIT	;IF CARRY BIT IS HIGH JUMP TO CARRY_16BIT
	CPI R17, 0			;COMPARE IF ALL BITS HAVE BEEN SHIFTED AND ACCOUNTED FOR
	BRNE L1				;IF NOT THROUGH ALL THE BOTTOM BITS, EVALUATE OF THE BOTTOM LSB IS A 0 OR 1
CARRY_16BIT:			;IF R19'S CARRY BIT IS HIGH DO THIS LOOP
	CPI R20, 0
	BREQ L2				;IF R20 EQUALS 0 THEN MOVE TO L2:
	LSL R20				;IF R20 DOES NOT EQUAL 0 SHIFT ZERO INTO R20 FROM THE LEFT
	
	ADD R20, R21		;ADD 1 TO R20 - '16BIT LOW'
	
	CPI R20, 0			
	BRNE L3				;IF R20 IS NOT EQUAL TO 0 GO TO L3:
L2:	
	ADD R20, R21		;IF R20 EQUALS 0 ADD 1 TO R20, START OF '16BIT LOW' CARRY FROM SHIFT AND ADD
L3:
	CPI R17, 0			;COMPARE IF ALL BITS HAVE BEEN SHIFTED AND ACCOUNTED FOR
	BRNE L1				;IF NOT THROUGH ALL THE BOTTOM BITS, EVALUATE OF THE BOTTOM LSB IS A 0 OR 1
	NOP
	;L2: RJMP L2		;STAY HERE FOREVER
	
	
	;LDI R18, RESULT_1
	;LDI R19, RESULT_2
	;LDI R20, RESULT_3
	;LDI R22, MULTIPLICAND_8
	;LDI R24, MULTIPLICAND_16_HIGH
	;LDI R25, MULTIPLICAND_16_LOW
	
