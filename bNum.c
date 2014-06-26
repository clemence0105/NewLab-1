#include "bNum.h"

struct bNum StructFromStr(char* string)
{
	int strSize = strlen(string);
	int strSign = 0;
	if (string[0] == '-')
	{
		strSize--;
		strSign = 1;
	}
	char* pStr = string + strSize + strSign;
	struct bNum res;
	res.base = 1000000000;
	res.size = ((strSize + strSign + 8) / 9);
	res.sign = strSign;
	res.nums = (unsigned int*)malloc((res.size)*sizeof(unsigned int));

	int i;
	for (i = 0; i < (strSize + strSign) / 9; i++)
	{
		pStr -= 9;
		char splStr[10];
		memcpy(splStr, pStr, 9);
		splStr[9] = '\0';
		unsigned int digit = atol(splStr);

		res.nums[i] = digit;
	}
	char ost[10];
	memset(ost, 0, 10);
	memcpy(ost, string + strSign, pStr - string - strSign);
	if (strlen(ost) > 0)
	{
		unsigned int lastDigit = atol(ost);
		res.nums[res.size - 1] = lastDigit;
	}
	res = DelZeros(res);
	return res;
}

struct bNum fromInt(long long int val, int base)
{
	struct bNum res;
	res.nums = (unsigned int*)malloc((100)*sizeof(unsigned int));
	res.size = 0;
	res.sign = 0;

	if (val < 0)
	{
		res.sign = 1;
		val = -val;
	}
	do
	{
		res.size++;
		res.nums[res.size - 1] = val % base;
		val = val / base;
	} while (val);
	res.base = base;
	return res;
}

char* getString(struct bNum a)
{
	if (a.base != 1000000000)
	{
		return NULL;
	}
	char* strBuf = (char*)malloc((a.size * 9 + 1 + a.sign) * sizeof(char));
	char* pString = strBuf + a.size * 9 + a.sign;
	*pString = '\0';

	int i;
	for (i = 0; i < a.size; i++)
	{
		char splStr[10];
		sprintf(splStr, "%09u", a.nums[i]);

		pString -= 9;
		memcpy(pString, splStr, 9);
	}

	while (*pString == '0' && *(pString + 1))
		pString++;

	if (a.sign)
	{
		pString--;
		*pString = '-';
	}

	char* string = (char*)malloc((strlen(pString) + 1) * sizeof(char));
	strcpy(string, pString);
	free(strBuf);

	return string;
}

struct bNum copy(struct bNum from)
{
	struct bNum cpy;
	cpy.base = from.base;
	cpy.size = from.size;
	cpy.nums = (unsigned int*)malloc((cpy.size)*sizeof(unsigned int));
	cpy.sign = from.sign;
	memcpy(cpy.nums, from.nums, cpy.size * sizeof(unsigned int));
	return cpy;
}

struct bNum DelZeros(struct bNum a)
{
	while ((a.size - 1) && a.nums && a.nums[a.size - 1] == 0)
		a.size--;

	if (a.size == 1 && a.nums[0] == 0)
		a.sign = 0;

	return a;
}

long long int compare(struct bNum a, struct bNum b)
{
	if (a.base != b.base)
	{
		return 0;
	}
	int aSign = 1;
	if (a.sign == 1)
		aSign = -1;

	if (a.sign != b.sign)
		return aSign;

	if (a.size > b.size)
		return aSign;

	if (a.size < b.size)
		return -aSign;

	int i = a.size - 1;

	while (a.nums[i] == b.nums[i] && i > 0)
	{
		i--;
	}
	return ((long long int) a.nums[i] - (long long int)b.nums[i]) * aSign;
}

struct bNum shiftLeft(struct bNum a, int s)
{
	unsigned int* newDig = (unsigned int*)malloc((a.size + s)*sizeof(unsigned int));

	memset(newDig, 0, s * sizeof(unsigned int));

	memcpy(newDig + s, a.nums, a.size * sizeof(unsigned int));
	free(a.nums);
	a.nums = newDig;
	a.size += s;

	return a;
}

struct bNum Sum1(struct bNum left, struct bNum right)
{
	if (left.base != right.base)
	{
		struct bNum res;
		res.sign = 0;
		res.size = 0;
		res.base = 0;
		res.nums = NULL;
		return res;
	}
	struct bNum a = left, b = right;
	a.sign = 0;
	b.sign = 0;
	if (compare(a, b) > 0)
	{
		a.sign = left.sign;
		b.sign = right.sign;
	}
	else
	{
		a = right;
		b = left;
	}

	if (a.sign == b.sign)
		return Sum2(a, b);
	else
		return sub(a, b);
}

struct bNum Sum2(struct bNum A, struct bNum B)
{
	struct bNum res;
	res.base = A.base;
	res.sign = 0;
	res.size = A.size + 1;
	res.nums = (unsigned int*)malloc((A.size + 1)*sizeof(unsigned int));

	unsigned int carry = 0;
	int i;
	for (i = 0; i < A.size; i++)
	{
		unsigned int tmp = A.nums[i] + carry;
		if (i < B.size)
			tmp += B.nums[i];

		res.nums[i] = tmp % res.base;
		carry = tmp / res.base;
	}

	res.nums[A.size] = carry;
	res.sign = A.sign;
	res = DelZeros(res);
	return res;
}

struct bNum sub(struct bNum A, struct bNum B)
{
	struct bNum res;
	res.base = A.base;
	res.sign = 0;
	res.size = A.size;
	res.nums = (unsigned int*)malloc((A.size)*sizeof(unsigned int));

	unsigned int carry = 0;
	int i;
	for (i = 0; i < A.size; i++)
	{
		int tmp = A.nums[i] - carry;
		if (i < B.size)
			tmp -= B.nums[i];

		carry = 0;
		if (tmp < 0)
		{
			carry = 1;
			tmp += res.base;
		}
		res.nums[i] = tmp;
	}

	res.sign = A.sign;
	res = DelZeros(res);
	return res;
}

struct bNum mins(struct bNum a)
{
	struct bNum res = copy(a);
	res.sign = !a.sign;
	return res;
}

struct bNum mul(struct bNum a, struct bNum b)
{
	if (a.base != b.base)
	{
		struct bNum res;
		res.sign = 0;
		res.size = 0;
		res.base = 0;
		res.nums = NULL;
		return res;
	}
	struct bNum res;
	res.base = a.base;
	res.size = a.size + b.size;
	res.nums = (unsigned int*)malloc((res.size)*sizeof(unsigned int));
	memset(res.nums, 0, res.size * sizeof(unsigned int));
	unsigned int carry = 0;
	int i;
	for (i = 0; i < b.size; i++)
	{
		carry = 0;
		int j;
		for (j = 0; j < a.size; j++)
		{
			unsigned long long int tmp = (unsigned long long int) b.nums[i] * (unsigned long long int) a.nums[j]
			+ carry + (unsigned long long int) res.nums[i + j];
			carry = tmp / res.base;
			res.nums[i + j] = tmp % res.base;
		}
		res.nums[i + a.size] = carry;
	}

	res.sign = (a.sign != b.sign);
	res = DelZeros(res);
	return res;
}

struct bNum dividing(struct bNum a, struct bNum b, struct bNum* remainder)
{
	if (a.base != b.base)
	{
		struct bNum res;
		res.sign = 0;
		res.size = 0;
		res.base = 0;
		res.nums = NULL;
		return res;
	}
	*remainder = copy(a);
	remainder->sign = 0;

	struct bNum divider = copy(b);
	divider.sign = 0;

	if (1 == divider.size)
	{
		if (divider.nums[0] == 0)
		{
			remainder->base = 0;
			remainder->size = 0;
			remainder->sign = 0;
			remainder->nums = NULL;

			struct bNum a;
			a.base = 0;
			a.size = 0;
			a.sign = 0;
			a.nums = NULL;

			free(divider.nums);
			return a;
		}
	}

	if (compare(*remainder, divider) < 0)
	{
		remainder->sign = a.sign;
		struct bNum res;
		res.base = a.base;
		res.sign = 0;
		res.size = 1;
		res.nums = (unsigned int*)malloc((res.size)*sizeof(unsigned int));
		res.nums[0] = 0;
		free(divider.nums);
		return res;
	}

	struct bNum res;
	res.base = a.base;
	res.sign = 0;
	res.size = a.size - b.size + 1;
	res.nums = (unsigned int*)malloc((res.size)*sizeof(unsigned int));

	int i;
	for (i = a.size - b.size + 1; i != 0; i--)
	{
		long long int qGuessMax = res.base;
		long long int qGuessMin = 0;
		long long int qGuess = qGuessMax;

		while (qGuessMax - qGuessMin > 1)
		{
			qGuess = (qGuessMax + qGuessMin) / 2;

			struct bNum qGuessBigInt = fromInt(qGuess, res.base);
			struct bNum tmp = mul(divider, qGuessBigInt);
			free(qGuessBigInt.nums);
			tmp = shiftLeft(tmp, i - 1);

			if (compare(tmp, *remainder) > 0)
				qGuessMax = qGuess;
			else
				qGuessMin = qGuess;

			free(tmp.nums);
		}
		struct bNum qGuessMinBigInt = fromInt(qGuessMin, res.base);
		struct bNum tmp = mul(divider, qGuessMinBigInt);
		free(qGuessMinBigInt.nums);
		tmp = shiftLeft(tmp, i - 1);

		struct bNum tmpRem = copy(*remainder);
		struct bNum minusTmp = mins(tmp);
		free(remainder->nums);
		*remainder = Sum1(tmpRem, minusTmp);
		free(tmpRem.nums);
		free(minusTmp.nums);
		free(tmp.nums);

		res.nums[i - 1] = qGuessMin;
	}

	res.sign = (a.sign != b.sign);
	remainder->sign = (a.sign != b.sign);
	*remainder = DelZeros(*remainder);
	res = DelZeros(res);

	free(divider.nums);
	return res;
}

struct bNum pPow(struct bNum a, struct bNum exp)
{
	if (a.base != exp.base)
	{
		struct bNum res;
		res.sign = 0;
		res.size = 0;
		res.base = 0;
		res.nums = NULL;
		return res;
	}
	struct bNum res = fromInt(1, a.base);
	struct bNum zero = fromInt(0, a.base);
	struct bNum i = copy(exp);
	struct bNum minusOne = fromInt(-1, a.base);

	while (compare(i, zero) > 0)
	{
		struct bNum tmp = mul(res, a);
		free(res.nums);
		res = tmp;

		tmp = Sum1(i, minusOne);
		free(i.nums);
		i = tmp;
	}

	free(i.nums);
	free(zero.nums);
	free(minusOne.nums);

	return res;
}

struct bNum powMod(struct bNum a, struct bNum exp, struct bNum modulus)
{
	if (a.base != exp.base || a.base != modulus.base)
	{
		struct bNum res;
		res.sign = 0;
		res.size = 0;
		res.base = 0;
		res.nums = NULL;
		return res;
	}
	struct bNum res = fromInt(1, a.base);
	struct bNum zero = fromInt(0, a.base);
	struct bNum i = copy(exp);
	struct bNum minusOne = fromInt(-1, a.base);

	while (compare(i, zero) > 0)
	{
		struct bNum tmp = mul(res, a);

		struct bNum modRem;
		struct bNum tmp2 = dividing(tmp, modulus, &modRem);
		free(tmp2.nums);
		free(tmp.nums);
		free(res.nums);
		res = modRem;

		tmp = Sum1(i, minusOne);
		free(i.nums);
		i = tmp;
	}
	free(i.nums);
	free(zero.nums);
	free(minusOne.nums);

	return res;
}

struct bNum ReadFromBFile(char* filename)
{
	FILE* fp = fopen(filename, "r+b");
	if (!fp)
	{
		struct bNum res;
		res.base = 0;
		res.size = 0;
		res.sign = 0;
		res.nums = NULL;
		return res;
	}

	fseek(fp, 0, SEEK_END);
	int fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	unsigned char* fileContent = (unsigned char*)malloc(fileSize);
	fread((char*) fileContent, sizeof(unsigned char), fileSize, fp);
	fclose(fp);

	struct bNum res;
	res.base = 256;
	res.size = fileSize;
	res.sign = 0;
	res.nums = (unsigned int*)malloc(res.size * sizeof(unsigned int));
	int i;
	for (i = 0; i < fileSize; i++)
	{
		res.nums[i] = fileContent[i];
	}

	free(fileContent);
	return res;
}

int WriteToBFile(char* filename, struct bNum a)
{
	unsigned char *fileContent = (unsigned char*)malloc(a.size);
	int i;
	for (i = 0; i < a.size; i++)
	{
		fileContent[i] = a.nums[i];
	}

	FILE* fp = fopen(filename, "w+b");
	if (!fp)
		return 1;

	fwrite((char*)fileContent, a.size, 1, fp);
	free(fileContent);
	fclose(fp);
	return 0;
}

struct bNum ReadFromTFile(char *fileName)
{
	FILE *fp = fopen(fileName, "r");
	if (!fp)
	{
		struct bNum res;
		res.size = 0;
		res.sign = 0;
		res.nums = NULL;
		return res;
	}

	fseek(fp, 0, SEEK_END);
	int fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char* fileContent = (char*)malloc(fileSize + 1);
	fscanf(fp, "%s", fileContent);
	fileContent[fileSize] = '\0';
	fclose(fp);
	struct bNum res = StructFromStr(fileContent);
	free(fileContent);
	return res;
}

int WriteToTFile(char *fileName, struct bNum a)
{
	FILE *fp = fopen(fileName, "w");
	if (!fp)
		return 1;

	char *res = getString(a);

	fprintf(fp, "%s", res);
	fclose(fp);
	free(res);

	return 0;
}