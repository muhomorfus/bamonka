#ifndef LAB_02_PCBC_H
#define LAB_02_PCBC_H

#include "des.h"

unsigned char* pcbc_encrypt(unsigned char* buf, int len, block_t key, block_t iv, int *new_len);
unsigned char* pcbc_decrypt(unsigned char* buf, int len, block_t key, block_t iv, int *new_len);

block_t* pcbc_encrypt_blocks(block_t* p, int len, block_t key, block_t iv);
block_t* pcbc_decrypt_blocks(block_t* c, int len, block_t key, block_t iv);

#endif //LAB_02_PCBC_H
