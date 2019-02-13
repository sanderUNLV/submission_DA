.ORG 0
        LDI R16, 15
        LDI R17, 5
        JMP LBL_NAME
        LDI R18, 4
        ADD R18, R17
LBL_NAME:
        ADD R16,R17
        JMP LBL_NAME
