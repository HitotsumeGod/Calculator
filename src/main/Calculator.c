#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char *operators;
	int *operands;
} bo;

char *parse_step1(void);
bo *parse_step2(char *buffer);
int parse_step3(bo *bobo);

int main(void) {

	bo *bing = parse_step3(parse_step2(parse_step1()));
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
	int mal_con = 1, num_count = 0, op_count = 0, *fin = NULL;		//REALLOC SIZE CONTROLLER, NUM LOCATION INCREMENETER, OPS LOCATION INCREMENTER, ARRAY OF OPERANDS AS INTEGERS
	bo *bobo;
	buf[strlen(buf)] = '\0';		
	for (int i = 0; i <= strlen(buf); i++) {	//EFFECTIVELY PARSES AN ARBITRARILY LONG MATHEMATICAL ARGUMENT
		if (i + 1 == mal_con) {		//BY USING A SINGLE REALLOC CONTROLLER, WE ELIMINATE THE NEED FOR MULTIPLE VARIABLES AT THE COST OF STABILITY
			mal_con *= 2;
			nums = realloc(nums, sizeof(char) * mal_con);
			fin = realloc(fin, sizeof(int) * mal_con);
			ops = realloc(ops, sizeof(char) * mal_con);
		}
		if ((c = *(buf + i)) != '+' && c != '-' && c != '*' && c != '/' && c != '\0') {
			*(nums + num_count) = c;
			num_count++;
		} else if (c == '\0') {		//ENSURE THAT LAST NUMBER IS READ USING NULL CHARACTER AS TERMINATION OPERATOR
			*(fin + op_count) = atoi(nums);
		} else {
			if ((*(fin + op_count) = atoi(nums)) == 0) {
				perror("atoi err");
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
	for (int i = 0; i <= op_count; i++) {		//OPS AND FIN ARE EQUAL IN LENGTH 
		printf("%d%c", *(fin + i), *(ops + i));
	}
	printf("\n");
	if ((bobo = malloc(sizeof(bo))) == NULL) {
		perror("malloc err");
		exit(1);
	}
	bobo -> operators = ops;
	bobo -> operands = fin;
	free(nums);
	free(ops);
	free(fin);
	return bobo;

}

int parse_step3(bo *bobo) {

	int *arr = bobo -> operands;	
	int fin = *arr;
	printf("%d\n", fin);

}
