; Author : ert

.include <m328pdef.inc>

.ORG 0		
.SET	MULTIPLICAND_8 = 50
.SET	MULTIPLICAND_8_B = 3

	LDI R18, 0					;FIRST 'MULTIPLICATION', "NUMBER x 1"
	LDI R19, MULTIPLICAND_8		;THE NUMBER TO BE MULTIPLIED
	LDI R20, MULTIPLICAND_8_B	;HOW MANY TIMES THE MULTIPLAND WILL ADD TO ITSELF
L1:	ADD R18, R19				;THE SECOND DECREMENT SHOULD HAPPEN AFTER THIS, "NUMBER x 2"
	DEC R20						;DECREMENT THE NUMBER OF TIMES THE MULTIPLICAND WILL BE ADDED
	BRNE L1						;LOOP BACK TO L1 UNTIL THE MULTIPLICAND_8_B IS 0
L2: RJMP L2						;STAY HERE FOREVER
