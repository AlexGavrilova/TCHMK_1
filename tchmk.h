#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BASE 4294967296

struct LongNumber
{
    unsigned int size;
    unsigned int* pointer;
};

struct LongNumber ReadNumber(unsigned long long int value);

struct LongNumber ReadString(const char* num);
char* PrintNumber(struct LongNumber number);

struct LongNumber ReadBinFile(const char* file);
void WriteBinFile(const char* file, struct LongNumber number);

struct LongNumber ReadTextFile(const char* file);
void WriteTextFile(const char* file, struct LongNumber number);

struct LongNumber ADD(struct LongNumber a, struct LongNumber b);
struct LongNumber SUB(struct LongNumber a, struct LongNumber b);
struct LongNumber MUL(struct LongNumber a, struct LongNumber b);

struct LongNumber DIV(struct LongNumber a, struct LongNumber b, int sumbols);
struct LongNumber SmallMul(struct LongNumber a, unsigned long long int b);
struct LongNumber shiftLeft(struct LongNumber a, unsigned int s);

struct LongNumber DEGREE(struct LongNumber a, struct LongNumber b, struct LongNumber c);
struct LongNumber SmallDIV(struct LongNumber a, unsigned long long int b);

struct LongNumber clear(struct LongNumber number);
struct LongNumber allocate(struct LongNumber number, unsigned int size);
struct LongNumber zero(struct LongNumber number, unsigned int size);

int compare(struct LongNumber a, struct LongNumber b);
struct LongNumber copy(struct LongNumber from);
struct LongNumber Normalize(struct LongNumber a);
