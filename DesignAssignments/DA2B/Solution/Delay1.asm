; Delay 20 000 000 cycles
; 1s 250ms at 16.0 MHz

.ORG 0x000100

Delay1:
    ldi  r18, 102
    ldi  r19, 118
    ldi  r20, 194
L1: dec  r20
    brne L1
    dec  r19
    brne L1
    dec  r18
    brne L1
	RET