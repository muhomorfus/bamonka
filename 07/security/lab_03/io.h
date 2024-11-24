#ifndef LAB_03_IO_H
#define LAB_03_IO_H

int ReadFile(char* filename, uint8_t** buf, int* size);
int WriteFile(char* filename, uint8_t* buf, int size);

#endif //LAB_03_IO_H
