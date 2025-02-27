#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *parse_step1(void);
double parse_step2(char *buffer);

int main(void) {

	return parse_step2(parse_step1());

}

char *parse_step1(void) {

	char *buffer;
	int n, c, rep, count;
	count = 0, n = 2;
	if ((buffer = malloc(sizeof(char) * n)) == NULL) {
		perror("malloc err");
		exit(1);
	}
	while ((c = fgetc(stdin)) != EOF && c != '\n') {
		if (count == n) {
			n *= 2;
			buffer = realloc(buffer, sizeof(char) * n);
			printf("%s\n", "Reallocated!");
		}
		if ((rep = c - '0') == 0 || rep == 1 || rep == 2 || rep == 3 || rep == 4 || rep == 5 || rep == 6 || rep == 7 || rep == 8 || rep == 9) {
			*(buffer + count) = c;
			count++;	
		} else if (c == '+' || c == '-' || c == '*' || c == '/') {
			*(buffer + count) = c;
			count++;
		}
	}
	*(buffer + count) = '\0';
	return buffer;

}

double parse_step2(char *buf) {

	char c, *nums, *ops;
	int n = 2, *fin = NULL, dfin;
	if ((nums = malloc(sizeof(char) * n)) == NULL || (ops = malloc(sizeof(char) * (n / 2))) == NULL) {
		perror("malloc err");
		exit(1);
	}
	for (int i = 0, ti = 0, oi = 0; i < strlen(buf); i++) {
		if ((c = *(buf + i)) != '+' && c != '-' && c != '*' && c != '/') {
			*(nums + ti) = c;
			ti++;
		} else {
			fin = realloc(fin, strlen(nums) + 1);
			for (int i2 = 0; i2 < strlen(nums); i2++)
				*(fin + i2) = atoi(nums);
			memset(nums, 0, strlen(nums));
			ti = 0;
			*(ops + oi) = c;
			oi++;
		}	
	}
	printf("%d\n", *fin);
	free(nums);
	free(ops);
	free(fin);
	return 1;		

}
