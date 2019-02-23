.ORG  0			;burn into ROM starting at 0
	LDI	ZL, LOW(MYDATA<<1)	;R30 = 00 low-byte addr 
	LDI	ZH, HIGH(MYDATA<<1) 	;R31 = 05, high-byte addr 
	LDI	XL, LOW(0x140)		;R26 = 40, low-byte RAM address
	LDI	XH, HIGH(0x140) 	;R27 = 1, high-byte RAM address
AGAIN:  LPM 	R16, Z+		;read the table, then increment Z
	CPI 	R16,0		;compare R16 with 0
	BREQ 	END		;exit if end of string
	ST 	X+, R16		;store R16 in RAM and inc X
	RJMP	AGAIN
END:	RJMP	END	
.ORG	0x500		;data burned starting at 0x500
MYDATA: .DB "CpE 3z1 ESD",0
