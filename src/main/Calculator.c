#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char *operators;
	double *operands;	
	size_t oplen, finlen;
} bo;

char *get_expression(void);	//OBTAIN USER-INPUT MATHEMATICAL EXPRESSION; RETURN IN STRING FORMAT FOR PARSING BY
bo *parse_expression(char *buffer);	//GIVEN A M.E., PARSES DIGITS AND OPERATORS, SEPARATING THEM INTO THE SEPARATE ARRAYS OF FIN AND OPS, WHICH ARE THEN STORED IN A BO STRUCT FOR EXPORTINGTO
double calculate_expression(bo *bobo);	//GIVEN A BO STRUCT, CHECKS FOR THE NEED TO PERFORM PEMDAS CALCULATIONS, AND THEN CALCULATES THE EXPRESSION SPLIT INTO OPERATOR AND OPERAND ARRAYS; RETURNS FINAL SUM TO MAIN FUNCTION

int main(void) {

	while (1)
		printf("%.2f\n", calculate_expression(parse_expression(get_expression())));
	return 0;
}

char *get_expression(void) {

	char *buffer, permissable[15] = {'0','1','2','3','4','5','6','7','8','9','+','-','*','/', '.'};
	int n, c, count, bol;
	count = 0, bol = 0, n = 2;
	if ((buffer = malloc(sizeof(char) * n)) == NULL) {
		perror("malloc err");
		exit(1);
	}
	while ((c = fgetc(stdin)) != EOF && c != '\n') {
		if (count == n) {
			n *= 2;
			buffer = realloc(buffer, sizeof(char) * n);
		}
		for (int i = 0; i < sizeof(permissable); i++) 
			if (c == permissable[i]) {
				bol = 1;
				break;
			} else {
				bol = 0;
			}
		if (bol) {
			*(buffer + count) = c;
			count++;
		} else {
			printf("%s\n", "Calculator only accepts numbers and operators (+, -, *, /).");	
			exit(1);
		}
	}
	*(buffer + count) = '\0';
	return buffer;

}

bo *parse_expression(char *buf) {

	char c, *nums = NULL, *ops = NULL;		//CURRENT CHAR BOX, ARRAY OF OPERANDS AS CHARACTERS, ARRAY OF OPERATORS
	int mal_con = 1, num_count = 0, op_count = 0, fin_count = 0;		//REALLOC SIZE CONTROLLER, NUM LOCATION INCREMENETER, OPS LOCATION INCREMENTER,
	double *fin = NULL;		 //ARRAY OF OPERANDS AS doubleING-POINTS
	bo *bobo;
	buf[strlen(buf)] = '\0';		
	for (int i = 0; i <= strlen(buf); i++) {	//EFFECTIVELY PARSES AN ARBITRARILY LONG MATHEMATICAL ARGUMENT
		if (i + 1 == mal_con) {		//BY USING A SINGLE REALLOC CONTROLLER, WE ELIMINATE THE NEED FOR MULTIPLE VARIABLES AT THE COST OF STABILITY
			mal_con *= 2;
			nums = realloc(nums, sizeof(char) * mal_con);
			fin = realloc(fin, sizeof(double) * mal_con);
			ops = realloc(ops, sizeof(char) * mal_con);
		}
		if ((c = *(buf + i)) != '+' && c != '-' && c != '*' && c != '/' && c != '\0') {
			*(nums + num_count) = c;
			num_count++;
		} else if (c == '\0') {		//ENSURE THAT LAST NUMBER IS READ USING NULL CHARACTER AS TERMINATION OPERATOR
			*(fin + fin_count) = atof(nums);
			fin_count++;
		} else {
			*(fin + fin_count) = atof(nums);
			if (memset(nums, 0, strlen(nums)) == NULL) {
				perror("memset err");
				exit(1);
			}
			num_count = 0;
			*(ops + op_count) = c;
			fin_count++;
			op_count++;
		}	
	}
	if ((bobo = malloc(sizeof(bo))) == NULL) {
		perror("malloc err");
		exit(1);
	}
	bobo -> operators = ops;
	bobo -> operands = fin;
	bobo -> oplen = op_count;
	bobo -> finlen = fin_count;
	free(buf);
	free(nums);
	return bobo;

}

double calculate_expression(bo *bobo) {

	double sfin = *(bobo -> operands);
	int pos_box, do_order = 0;
	for (int i = 0; i < bobo -> oplen; i++)		//SHOULD BE VAR < OPLEN IF INTERACTING WITH OPERATORS; IF INTERACTING WITH OPERANDS SHOULD BE VAR <= OPLEN TO ACCOUNT FOR TRAILING OPERAND
		if (*(bobo -> operators + i) == '*' || *(bobo -> operators + i) == '/') {
			do_order = 1;
			pos_box = i;
			break;
		}
	while (do_order) {	//DOES WORK IN (PE)MDAS FASHION
		switch(*(bobo -> operators + pos_box)) {
			case '*':
				sfin = *(bobo -> operands + pos_box) * *(bobo -> operands + (pos_box + 1));
				break;
			case '/':
				sfin = *(bobo -> operands + pos_box) / *(bobo -> operands + (pos_box + 1));
				break;
		}
		for (int i = pos_box + 1; i < bobo -> oplen; i++) {	//ERASE THE RIGHTMOST OF THE TWIN UTILIZED OPERANDS, KEEPING ARRAY CONTENTS SAFE
			*(bobo -> operands + i) = *(bobo -> operands + (i + 1));
			*(bobo -> operators + (i - 1)) = *(bobo -> operators + i);
		}
		*(bobo -> operators + (bobo -> oplen - 1)) = '\0';
		*(bobo -> operands + (bobo -> finlen - 1)) = 0;
		*(bobo -> operands + pos_box) = sfin;	//REPLACE THE LEFTMOST OF THE TWIN OPERANDS WITH THEIR SUM
		do_order = 0;
		for (int i = 0; i < bobo -> oplen; i++)		
			if (*(bobo -> operators + i) == '*' || *(bobo -> operators + i) == '/') {
				do_order = 1;
				pos_box = i;
				break;
			}
	}
	sfin = *(bobo -> operands);	//SET SFIN BACK TO FIRST VALUE IN OPERANDS LIST
	for (int i = 0; i < bobo -> oplen; i++) {
		switch (*(bobo -> operators + i)) {
			case '+':
				sfin += *(bobo -> operands + (i + 1));
				break;
			case '-':
				sfin -= *(bobo -> operands + (i + 1));
				break;
		}
	}
	free(bobo);
	return sfin;

}

