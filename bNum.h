#ifndef _BIGINT_H_
#define _BIGINT_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

struct bNum
{
	int size;
	int sign;
	int base;
	unsigned int* nums;
};

struct bNum StructFromStr(char* string);
struct bNum fromInt(long long int val, int base);
char* getString(struct bNum a);
struct bNum copy(struct bNum from);
struct bNum DelZeros(struct bNum a);
long long int compare(struct bNum A, struct bNum B);
struct bNum Sum1(struct bNum left, struct bNum right);
struct bNum Sum2(struct bNum A, struct bNum B);
struct bNum sub(struct bNum A, struct bNum B);
struct bNum mins(struct bNum a);
struct bNum mul(struct bNum A, struct bNum B);
struct bNum dividing(struct bNum A, struct bNum B, struct bNum* remainder);
struct bNum shiftLeft(struct bNum a, int s);
struct bNum pPow(struct bNum base, struct bNum exp);
struct bNum powMod(struct bNum base, struct bNum exp, struct bNum modulus);
struct bNum ReadFromTFile(char *fileName);
int WriteToTFile(char *fileName, struct bNum A);
struct bNum ReadFromBFile(char* filename);
int WriteToBFile(char* filename, struct bNum A);

#endif
