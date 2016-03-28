#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#include "myfilesystem.h"
#include "bmsearch.h"

int main() {
	int i = 1;
	char filename[100];
	char number[10];
	while(i<2) {
		filename[0] = '\0';
		sprintf(number, "%d", i++);
		strcat(filename, "../youtubeData/youtube_");
		strcat(filename, number);
		strcat(filename, ".gais");
		printf("%s\n", filename);
		myread(filename, "Subaru");
	}
}
