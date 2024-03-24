#include <stdlib.h>
#include "stdio.h"
void decodeFile(const char* inputFile, const char* outputFile) {
    char command[256];
    sprintf(command, "certutil -decode %s %s", inputFile, outputFile);
    system(command);
}

int main() {
    const char* inputFileName = "C:\\Users\\beta\\Desktop\\Apps\\coding_notes\\output7.txt";
    const char* outputFileName = "C:\\Users\\beta\\Downloads\\image_url\\image71.jpg";

    decodeFile(inputFileName, outputFileName);

    return 0;
}
