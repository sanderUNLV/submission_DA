// DA2B_T1_A
// Author : Robert Sander

.include <m328pdef.inc>
.include "Delay1.asm"

.ORG 0	;location for reset
	JMP	MAIN
.ORG 0x02	 ;location for EXT_INT0
	JMP	EX1_ISR		
MAIN: 
	LDI	R20,HIGH(RAMEND);Initialize the stack
	OUT	SPH,R20			;Initialize the stack
	LDI	R20,LOW(RAMEND)	;Initialize the stack
	OUT	SPL,R20			;Initialize the stack
	LDI R16, 0xFF	;load 0xFF into R16
	OUT DDRB, R16	;SET PORTB TO OUTPUT
	LDI R16, 0x3C	;load 0x3C into R16
	OUT PORTB, R16	;'Turn off' by setting PB.2-5 high
	SBI	PORTD,2     ;pull-up activated, PD.2
	LDI	R20, (0<<ISC01)|(0<<ISC00) ;Low level interrupt 
	STS	EICRA,R20	;Low level interrupt 
	LDI	R20,1<<INT0 ;Enable INT0
	OUT	EIMSK,R20	;Enable INT0
	SEI				;Set I (Enable Interrupts)
HERE:
	JMP HERE	;Stay here indefinitely
EX1_ISR:
	LDI R20, 1<<INTF0
	OUT	EIFR, R20 ; clear flag, EXOR...LOADS 1, 1*1=0
	LDI R16, 0x38 ;LOAD THE VALUE 0x38 INTO R16
	OUT PORTB, R16	;'TURNS ON' PORTB...REVERSE LOGIC
	RCALL Delay1
	LDI R16, 0x3C ;LOAD THE VALUE 0x3C INTO R16
	OUT PORTB, R16	;'TURNS OFF' PORTB...REVERSE LOGIC
	RETI