#ifndef LAB_02_IO_H
#define LAB_02_IO_H

int ReadFile(char* filename, unsigned char** buf, int* size);
int WriteFile(char* filename, unsigned char* buf, int size);

#endif //LAB_02_IO_H
