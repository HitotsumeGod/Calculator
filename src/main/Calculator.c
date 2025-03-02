#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char *operators;
	float *operands;	
	size_t oplen;
} bo;

char *parse_step1(void);
bo *parse_step2(char *buffer);
int parse_step3(bo *bobo);

int main(void) {

	while (1)
		parse_step3(parse_step2(parse_step1()));
	return 0;
}

char *parse_step1(void) {

	char *buffer, permissable[14] = {'0','1','2','3','4','5','6','7','8','9','+','-','*','/'};
	int n, c, count, bol = 0;
	count = 0, n = 2;
	if ((buffer = malloc(sizeof(char) * n)) == NULL) {
		perror("malloc err");
		exit(1);
	}
	while ((c = fgetc(stdin)) != EOF && c != '\n') {
		if (count == n) {
			n *= 2;
			buffer = realloc(buffer, sizeof(char) * n);
		}
		for (int i = 0; i < 14; i++)
			if (c == permissable[i]) {
				bol = 1;
				break;
			}
		if (bol == 1) {
			*(buffer + count) = c;
			count++;
		}	
	}
	*(buffer + count) = '\0';
	return buffer;

}

bo *parse_step2(char *buf) {

	char c, *nums = NULL, *ops = NULL;		//CURRENT CHAR BOX, ARRAY OF OPERANDS AS CHARACTERS, ARRAY OF OPERATORS
	int mal_con = 1, num_count = 0, op_count = 0;		//REALLOC SIZE CONTROLLER, NUM LOCATION INCREMENETER, OPS LOCATION INCREMENTER,
	float *fin = NULL;		 //ARRAY OF OPERANDS AS FLOATING-POINTS
	bo *bobo;
	buf[strlen(buf)] = '\0';		
	for (int i = 0; i <= strlen(buf); i++) {	//EFFECTIVELY PARSES AN ARBITRARILY LONG MATHEMATICAL ARGUMENT
		if (i + 1 == mal_con) {		//BY USING A SINGLE REALLOC CONTROLLER, WE ELIMINATE THE NEED FOR MULTIPLE VARIABLES AT THE COST OF STABILITY
			mal_con *= 2;
			nums = realloc(nums, sizeof(char) * mal_con);
			fin = realloc(fin, sizeof(float) * mal_con);
			ops = realloc(ops, sizeof(char) * mal_con);
		}
		if ((c = *(buf + i)) != '+' && c != '-' && c != '*' && c != '/' && c != '\0') {
			*(nums + num_count) = c;
			num_count++;
		} else if (c == '\0') {		//ENSURE THAT LAST NUMBER IS READ USING NULL CHARACTER AS TERMINATION OPERATOR
			*(fin + op_count) = atoi(nums);
		} else {
			if ((*(fin + op_count) = atof(nums)) == 0) {
				perror("atof err");
				exit(1);
			}
			if (memset(nums, 0, strlen(nums)) == NULL) {
				perror("memset err");
				exit(1);
			}
			num_count = 0;
			*(ops + op_count) = c;
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
	free(nums);
	return bobo;

}

int parse_step3(bo *bobo) {

	float sfin = *(bobo -> operands);
	int pos_box, do_order = 0;
	for (int i = 0; i < bobo -> oplen; i++)		//SHOULD BE VAR < OPLEN IF INTERACTING WITH OPERATORS; IF INTERACTING WITH OPERANDS SHOULD BE VAR <= OPLEN TO ACCOUNT FOR TRAILING OPERAND
		if (*(bobo -> operators + i) == '*' || *(bobo -> operators + i) == '/') {
			do_order = 1;
			pos_box = i;
			break;
		}
	if (do_order) {
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
		*(bobo -> operands + (bobo -> oplen)) = 0;
		*(bobo -> operands + pos_box) = sfin;	//REPLACE THE LEFTMOST OF THE TWIN OPERANDS WITH THEIR SUM
	}
	sfin = *(bobo -> operands);
	for (int i = 0; i <= bobo -> oplen; i++) {
		switch(*(bobo -> operators + i)) {
			case '+':
				sfin += *(bobo -> operands + (i + 1));
				break;
			case '-':
				sfin -= *(bobo -> operands + (i + 1));
				break;
		}
	}
	free(bobo);
	printf("%.4f\n", sfin);
	return 0;

}

