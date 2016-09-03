#include <stdio.h>
#include <stdlib.h>

int main()
{

	int N = 2;
	int *ptrs[N];
	for (int x = 0; x < N; x++) {

		ptrs[x] = (int *)malloc(10 * sizeof(int));
		for (int i = 0; i < 10; i++) {
			ptrs[x][i] = i * 2;
		}

	}

	int x = N;
	while (x) {

		for (int i = 0; i < 10; i++) {
			printf("%d ", ptrs[x - 1][i]);
		}
		printf("\n");
		free(ptrs[x - 1]);
		x--;
	}
	return 0;
}
