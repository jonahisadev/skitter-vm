#include "include/util.h"

ByteBuffer* create_byte_buffer() {
	ByteBuffer* buf = (ByteBuffer*) malloc(sizeof(ByteBuffer));
	buf->buffer = (unsigned char*) malloc(sizeof(unsigned char));
	buf->length = 0;
	buf->alloc_size = 1;
	return buf;
}

void load_byte_buffer(ByteBuffer* buf, unsigned char* buffer, int len) {
	for (int i = 0; i < len; i++) {
		write_byte_to_buffer(buf, buffer[i]);
	}
}

void write_byte_to_buffer(ByteBuffer* buf, unsigned char n) {
	if (buf->length >= buf->alloc_size) {
		buf->alloc_size *= 2;
		buf->buffer = realloc(buf->buffer, sizeof(unsigned char) * buf->alloc_size);
	}

	buf->buffer[buf->length++] = n;
}

void write_short_to_buffer(ByteBuffer* buf, unsigned short n) {
	write_byte_to_buffer(buf, n & 0xFF00);
	write_byte_to_buffer(buf, n & 0x00FF);
}

void write_int_to_buffer(ByteBuffer* buf, unsigned int n) {
	write_byte_to_buffer(buf, n & 0xFF000000);
	write_byte_to_buffer(buf, n & 0x00FF0000);
	write_byte_to_buffer(buf, n & 0x0000FF00);
	write_byte_to_buffer(buf, n & 0x000000FF);
}

unsigned char read_byte_from_buffer(ByteBuffer* buf, int index) {
	return buf->buffer[index];
}

unsigned int read_int_from_buffer(ByteBuffer* buf, int index) {
	unsigned char a = read_byte_from_buffer(buf, index);
	unsigned char b = read_byte_from_buffer(buf, index + 1);
	unsigned char c = read_byte_from_buffer(buf, index + 2);
	unsigned char d = read_byte_from_buffer(buf, index + 3);
	return (a << 24) | (b << 16) | (c << 8) | d;
}

void delete_byte_buffer(ByteBuffer* buf) {
	free(buf->buffer);
	free(buf);
}

// Read text file
char* read_file_ascii(const char* filename) {
	FILE* file = fopen(filename, "r");
	if (!file) {
		panic("Could not open file '%s'\n", filename);
	}

	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	fseek(file, 0, SEEK_SET);

	char* bytes = malloc(sizeof(char) * (size + 1));
	fread(bytes, 1, size, file);
	bytes[size] = '\0';
	fclose(file);

	return bytes;
}

void write_file_binary(const char* filename, ByteBuffer* buffer) {
	FILE* file = fopen(filename, "wb");
	if (!file) {
		printf("Could not write to file '%s'\n", filename);
		exit(EXIT_FAILURE);
	}

	fwrite(buffer->buffer, 1, buffer->length, file);
	fclose(file);
}

unsigned char* read_file_binary(const char* filename, int* len) {
	FILE* file = fopen(filename, "rb");
	if (!file) {
		printf("Could not open file '%s'\n", filename);
		exit(EXIT_FAILURE);
	}

	fseek(file, 0, SEEK_END);
	*len = ftell(file);
	fseek(file, 0, SEEK_SET);

	unsigned char* bytes = malloc(sizeof(unsigned char) * (*len));
	fread(bytes, 1, *len, file);
	fclose(file);

	return bytes;
}