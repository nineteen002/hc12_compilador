	ORG $4000
	LDAA #20
E1 	ORG $4010
	LDAA @20
	LDAA 16,SP
	LDAA -16,PC
	BNE E1
	JMP E1
	LBNE E1
	IBNE A,E1
E2 	START
E3 	LDAA -10,SP
	LDAA [10,SP]
	LDAA [D,SP]
	LDAA [A,SP]
E4 	END