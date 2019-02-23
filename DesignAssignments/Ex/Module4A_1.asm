.include<m328pdef.inc>

.cseg
.org 0x00

  LDI	R19,0x5		;R19 = 5 (R19 for counter)
  LDI	R16,0x55	;load R16 with value 0x55 (value to be copied)
  LDI	YL,LOW($140)	;load the low byte of Y with value 0x40
  LDI	YH,HIGH($140)	;load the high byte of Y with value 0x1
L1:
	ST Y+,R16		;copy R16 to memory location Y
	DEC R19		;decrement the counter
	BRNE L1		;loop until counter = zero

end: rjmp end