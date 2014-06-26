#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bNum.c"
#include "bNum.h"

int main(int argc, char* argv[])
{
	char *file1 = argv[1];
	char *file2 = argv[3];
	char *file3 = argv[4];
	char *file4 = NULL;
	char op = argv[2][0];
	int bin = 0;
	struct bNum n, m, md;

	if (argc == 6)
	{
		if (!strcmp(argv[5], "-b"))
			bin = 1;
		else
			file4 = argv[5];
	}

	if (argc == 7)
	{
		bin = 1;
		file4 = argv[6];
	}

	if (bin == 1)
	{
		n = ReadFromBFile(file1);
		m = ReadFromBFile(file2);
		if (file4 != NULL)
			md = ReadFromBFile(file4);
	}
	else
	{
		n = ReadFromTFile(file1);
		m = ReadFromTFile(file2);
		if (file4 != NULL)
			md = ReadFromTFile(file4);
	}

	struct bNum k;
	k.size = 0;
	k.sign = 0;
	k.nums = NULL;
	if (argv[2][0] == '+')
		k = Sum1(n, m);

	if (argv[2][0] == '-')
	{
		struct bNum minusB = mins(m);
		k = Sum1(n, minusB);
		free(minusB.nums);
	}

	if (argv[2][0] == '*')
		k = mul(n, m);

	if (argv[2][0] == '/')
	{
		struct bNum q;
		k = dividing(n, m, &q);
		free(q.nums);
	}

	if (argv[2][0] == '%')
	{
		struct bNum q = dividing(n, m, &k);
		free(q.nums);
	}

	if (file4 != NULL)
	{
		struct bNum tmp;
		struct bNum q = dividing(k, md, &tmp);
		free(q.nums);
		free(k.nums);
		k = tmp;
	}

	if (argv[2][0] == '^')
	{
		if (file4 == NULL)
			k = pPow(n, m);
		else
			k = powMod(n, m, md);
	}

	if (bin == 1)
		WriteToBFile(file3, k);
	else
		WriteToTFile(file3, k);

	free(n.nums);
	free(m.nums);
	free(k.nums);
	if (file4 != NULL)
		free(md.nums);

	return 0;
}
