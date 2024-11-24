#ifndef LAB_04_IO_H
#define LAB_04_IO_H

int ReadFile(char* filename, uint8_t** buf, int* size);
int WriteFile(char* filename, uint8_t* buf, int size);

#endif //LAB_04_IO_H
