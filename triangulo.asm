TRIANGULO: EQU 1 ;isto � uma constante
SECTION TEXT
INPUT		B
INPUT		H
LOAD		B
MULT		H
IF TRIANGULO
DIV		 DOIS
STORE		R
OUTPUT	R
STOP
SECTION DATA
B:		SPACE
H:		SPACE
R:		SPACE
DOIS:	CONST		2

