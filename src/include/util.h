#ifndef SK_UTIL_H
#define SK_UTIL_H

#include "common.h"

// ByteBuffer
struct ByteBuffer_t {
	unsigned char* buffer;
	int length;
	int alloc_size;
};
typedef struct ByteBuffer_t ByteBuffer;

ByteBuffer* create_byte_buffer();
void load_byte_buffer(ByteBuffer* buf, unsigned char* buffer, int len);
void write_byte_to_buffer(ByteBuffer* buf, unsigned char n);
void write_short_to_buffer(ByteBuffer* buf, unsigned short n);
void write_int_to_buffer(ByteBuffer* buf, unsigned int n);
unsigned char read_byte_from_buffer(ByteBuffer* buf, int index);
unsigned int read_int_from_buffer(ByteBuffer* buf, int index);
void delete_byte_buffer(ByteBuffer* buf);

// Read text file
char* read_file_ascii(const char* filename);
void write_file_binary(const char* filename, ByteBuffer* buffer);
unsigned char* read_file_binary(const char* filename, int* len);

#endif // SK_UTIL_H