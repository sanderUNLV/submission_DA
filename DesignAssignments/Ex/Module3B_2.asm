.include<m328pdef.inc>

.cseg
.org 0x00

   LDI R20,0
   LDI R16,1
L1:ADD R20,R16
   LDI R17,2
   ADD R16,R17	;R16 = R16 + 2
   LDI R17,27	;R17 = 27
   SUB R17,R16	
   BRCC L1	;if R16 <= 27 jump L1

end: rjmp end