#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BASE 4294967296

struct LongNum
{
	unsigned int size;
	unsigned int* index;
};


LongNum InNum(unsigned long long int value);
LongNum InStr(const char* num);
char* OutNum(LongNum number);

LongNum InBinFile(const char* file);
void OutBinFile(const char* file, LongNum number);

LongNum InFile(const char* file);
void OutFile(const char* file, LongNum number);

LongNum add(LongNum a, LongNum b);
LongNum sub(LongNum a, LongNum b);
LongNum multiply(LongNum a, LongNum b);

LongNum divide(LongNum a, LongNum b, int sumbols);
LongNum mul(LongNum a, unsigned long long int b);
LongNum shift(LongNum a, unsigned int s);

LongNum pow(LongNum a, LongNum b, LongNum c);
LongNum div(LongNum a, unsigned long long int b);

LongNum clear(LongNum number);
LongNum take(LongNum number, unsigned int size);
LongNum zero(LongNum number, unsigned int size);

int compare(LongNum a, LongNum b);
LongNum copy(LongNum from);
LongNum Norm(LongNum a);

int main(int argc, const char * argv[])
{
	if (argc < 5)
	{
		printf("Too few arguments! \n");
		return 0;
	}

	if (argc > 7)
	{
		printf("Too many arguments! \n");
		return 0;
	}

	FILE* fileA = fopen(argv[1], "r");
	if (!fileA)
	{
		printf("Can't open the first file!\n");
		return 0;
	}
	fclose(fileA);

	const char* choice = argv[2];

	FILE* fileB = fopen(argv[3], "r");
	if (!fileB)
	{
		printf("Can't open the second file!\n");
		return 0;
	}
	fclose(fileB);

	FILE* outputFile = fopen(argv[4], "r");
	if (!outputFile)
	{
		printf("Can't open output file!\n");
		return 0;
	}
	fclose(outputFile);



	int bin = 0;
	if (argc == 6)
	{
		if (argv[2][0] == '^')
		{
			FILE* ModFile = fopen(argv[5], "r");
			if (!ModFile)
			{
				printf("Can't open module  file!\n");
				return 0;
			}
			fclose(ModFile);
		}
		else
		{
			if (strcmp(argv[5], "-b"))
			{
				printf("Invalid flag!\n");
				return 0;
			}
			bin = 1;
		}

	}

	if (argc == 7)
	{
		FILE* ModFile = fopen(argv[5], "r");
		if (!ModFile)
		{
			printf("Can't open module file!\n");
			return 0;
		}
		fclose(ModFile);

		if (strcmp(argv[6], "-b"))
		{
			printf("Invalid flag!\n");
			return 0;
		}
		bin = 1;
	}

	LongNum a, b;

	
	if (bin == 1)
		a = InBinFile(argv[1]);
	else
		a = InFile(argv[1]);

	
	if (bin == 1)
		b = InBinFile(argv[3]);
	else
		b = InFile(argv[3]);

	

	LongNum result;

	switch (choice[0]) {
	case '+':
	{
				result = add(a, b);
				break;
	}
	case '-':
	{
				result = sub(a, b);
				break;
	}
	case '*':
	{
				result = multiply(a, b);
				break;
	}
	case '/':
	{
				result = divide(a, b, 1);
				break;
	}
	case '%':
	{
				result = divide(a, b, 2);
				break;
	}
	case '^':
	{
				LongNum c;
				if (bin == 1)
					c = InBinFile(argv[5]);
				else
					c = InFile(argv[5]);

				result = pow(a, b, c);
				c = clear(c);
				break;
	}
	default:
		break;
	}

	
	if (bin == 1)
		OutBinFile(argv[4], result);
	else
		OutFile(argv[4], result);

	
	a = clear(a);
	b = clear(b);
	result = clear(result);

	return 0;
}



char* OutNum(struct LongNum number)
{
	LongNum dec;

	dec.size = number.size * 10;

	dec = take(dec, dec.size);
	dec = zero(dec, dec.size);

	unsigned int a, j = 0;
	long long int tmp, i = number.size - 1;
	char move = 0;

	while (number.size != 1 || number.index[0] != 0)
	{
		move = 0;

		for (i = number.size - 1; i >= 0; i--)
		{
			tmp = move * BASE + number.index[i];
			number.index[i] = tmp / 10;
			move = tmp - (long long int) number.index[i] * 10;
		}

		move += '0';

		dec.index[j] = move;
		j++;

		number = Norm(number);
	}

	dec = Norm(dec);

	char *string = NULL;
	j = 0;

	string = (char*)malloc(sizeof(char)*(1));
	string[0] = '\0';

	for (i = dec.size - 1; i > -1; i--)
		printf("%c", dec.index[i]);

	free(dec.index);
	dec.size = 0;

	return string;
}

LongNum InNum(unsigned long long int value)
{
	LongNum num;

	num.index = (unsigned int*)malloc(sizeof(unsigned int)*(3));
	num.size = 0;

	unsigned long long int move = value;

	do
	{
		num.size++;
		num.index[num.size - 1] = move % BASE;
		move = move / BASE;
	} while (move);

	return num;
}

LongNum InStr(const char* num)
{
	LongNum bin, number;
	char ch;
	long long int i = 0;

	for (; i < strlen(num); i++)
	if (num[i] < '0' || num[i] > '9')
	{
		printf("Error: Wrong string: %s \n", num);
		bin.index = (unsigned int*)malloc(sizeof(unsigned int)*(1));
		bin.index[0] = 0;
		bin.size = 1;
		return bin;
	}

	number.size = strlen(num);

	number = take(number, number.size);
	number = zero(number, number.size);

	bin.size = number.size / 9 + 1;

	bin = take(bin, bin.size);
	bin = zero(bin, bin.size);

	unsigned int a, move = 0, tmp, current, j, x;

	i = number.size - 1;

	for (; i > -1; --i)
		number.index[i] = num[number.size - i - 1] - '0';

	current = 1;
	j = 0;
	x = 0;

	while (number.size != 1 || number.index[0] != 0)
	{
		move = 0;

		for (i = number.size - 1; i >= 0; i--)
		{
			tmp = move * 10 + number.index[i];
			number.index[i] = tmp / 2;
			move = tmp - number.index[i] * 2;
		}

		number = Norm(number);

		bin.index[j] = ((current << x) * move) | bin.index[j];

		x++;

		if (x == 32)
		{
			x = 0;
			j++;
		}
	}

	number = clear(number);

	bin = Norm(bin);

	return bin;
}

LongNum InBinFile(const char* file)
{
	FILE* f = fopen(file, "r");
	if (!f)
	{
		printf("Can't open file!\n");
		LongNum number;
		number.size = 1;
		number.index = (unsigned int*)malloc(sizeof(unsigned int)*(1));
		number.index[0] = 0;
		fclose(f);
		return number;
	}
	fclose(f);

	FILE* in = fopen(file, "rb");
	LongNum number;
	number.size = 0;

	fseek(in, 0, SEEK_END);
	unsigned int quot, remainder, size = ftell(in);

	quot = size / sizeof(unsigned int);
	remainder = size % sizeof(unsigned int);

	if (remainder == 0)
		number = take(number, quot);
	else
		number = take(number, quot + 1);

	fseek(in, SEEK_SET, 0);  //переход на начало строки

	int i, j;

	for (i = 0; i < quot; i++)
		fread(&number.index[i], sizeof(unsigned int), 1, in);

	number.size = quot;

	if (remainder != 0)
	{
		unsigned char ch;
		number.size++;
		number.index[quot] = 0;

		for (j = 0; j<remainder; j++)
		{
			fread(&ch, sizeof(unsigned char), 1, in);
			number.index[quot] |= (ch << j * 8);
		}
	}

	fclose(in);

	return number;
}

void OutBinFile(const char* file, struct LongNum number)
{
	FILE* out = fopen(file, "wb");

	int i;

	for (i = 0; i < number.size; i++)
		fwrite(&number.index[i], sizeof(unsigned int), 1, out);

	fclose(out);
}

LongNum InFile(const char* file)
{
	FILE* f = fopen(file, "r");
	if (!f)
	{
		printf("Can't open file!\n");
		LongNum number;
		number.size = 1;
		number.index = (unsigned int*)malloc(sizeof(unsigned int)*(1));
		number.index[0] = 0;
		fclose(f);
		return number;
	}
	fclose(f);

	LongNum bin, number;
	char ch;

	number.size = 0;

	FILE *in = fopen(file, "r");

	fseek(in, 0, SEEK_END);
	number.size = ftell(in);

	fseek(in, SEEK_SET, 0);

	number = take(number, number.size);
	number = zero(number, number.size);

	bin.size = number.size / 9 + 1;

	bin = take(bin, bin.size);
	bin = zero(bin, bin.size);

	unsigned int a, move = 0, tmp, current, j, x;

	long long int i = number.size - 1;

	while ((ch = getc(in)) != EOF)
		number.index[i--] = ch - '0';

	fclose(in);

	current = 1;
	j = 0;
	x = 0;

	while (number.size != 1 || number.index[0] != 0)
	{
		move = 0;

		for (i = number.size - 1; i >= 0; i--)
		{
			tmp = move * 10 + number.index[i];
			number.index[i] = tmp / 2;
			move = tmp - number.index[i] * 2;
		}

		number = Norm(number);

		bin.index[j] = ((current << x) * move) | bin.index[j];

		x++;

		if (x == 32)
		{
			x = 0;
			j++;
		}
	}

	number = clear(number);

	bin = Norm(bin);

	return bin;
}

void OutFile(const char* file, LongNum number)
{
	FILE* out = fopen(file, "w");

	LongNum dec;

	dec.size = number.size * 10;

	dec = take(dec, dec.size);
	dec = zero(dec, dec.size);

	unsigned int a, j = 0;
	long long int tmp, i = number.size - 1;
	char move = 0;

	while (number.size != 1 || number.index[0] != 0)
	{
		move = 0;

		for (i = number.size - 1; i >= 0; i--)
		{
			tmp = move * BASE + number.index[i];
			number.index[i] = tmp / 10;
			move = tmp - (long long int) number.index[i] * 10;
		}

		move += '0';

		dec.index[j] = move;
		j++;

		number = Norm(number);
	}

	dec = Norm(dec);

	for (i = dec.size - 1; i > -1; i--)
		fprintf(out, "%c", dec.index[i]);

	fclose(out);
}

LongNum clear(LongNum number)
{
	free(number.index);
	return number;
}

LongNum take(LongNum number, unsigned int size)
{
	number.index = (unsigned int*)malloc(sizeof(unsigned int)*(size));
	return number;
}

LongNum zero(LongNum number, unsigned int size)
{
	memset(number.index, 0, size * sizeof(unsigned int));
	return number;
}

LongNum copy(LongNum from)
{
	LongNum cpy;

	cpy.size = from.size;
	cpy = take(cpy, cpy.size);
	memcpy(cpy.index, from.index, cpy.size * sizeof(unsigned int));

	return cpy;
}

int compare(LongNum a, LongNum b)
{
	if (a.size > b.size)
		return 1;

	if (a.size < b.size)
		return -1;

	int i = a.size - 1;

	while (a.index[i] == b.index[i] && i > 0)
		i--;

	if (a.index[i] > b.index[i])
		return 1;

	if (a.index[i] < b.index[i])
		return -1;

	if (a.index[i] == b.index[i])
		return 0;
}

LongNum add(LongNum a, LongNum b)
{
	unsigned long long int tmp;
	unsigned int move = 0, i;
	LongNum res;

	if (a.size < b.size)
	{
		return add(b, a);
	}

	res.size = a.size + 1;
	res = take(res, res.size);
	res = zero(res, res.size);

	for (i = 0; i<b.size; i++)
	{
		tmp = (unsigned long long int) a.index[i] + (unsigned long long int) b.index[i] + (unsigned long long int) move;
		if (tmp >= BASE)
		{
			res.index[i] = tmp - BASE;
			move = 1;
		}
		else
		{
			res.index[i] = tmp;
			move = 0;
		}
	}

	for (; i < a.size; i++)
	{
		tmp = (unsigned long long int) a.index[i] + (unsigned long long int) move;

		if (tmp >= BASE)
		{
			res.index[i] = tmp - BASE;
			move = 1;
		}
		else
		{
			res.index[i] = tmp;
			move = 0;
		}
	}

	if (move == 1)
	{
		res.index[i] = move;
		res.size = a.size + 1;
	}
	else
		res.size = a.size;

	return res;
}

LongNum sub(LongNum a, LongNum b)
{
	long long int tmp;
	unsigned int move = 0, i;
	LongNum res;

	if (compare(a, b) < 0)
	{
		res = sub(b, a);
		return res;
	}

	res.size = a.size;
	res = take(res, res.size);
	res = zero(res, res.size);

	for (i = 0; i<b.size; i++)
	{
		tmp = (long long int)a.index[i] - (long long int) b.index[i] - (long long int) move;

		if (tmp < 0)
		{
			res.index[i] = tmp + BASE;
			move = 1;
		}
		else
		{
			res.index[i] = tmp;
			move = 0;
		}
	}

	for (; i<a.size; i++)
	{
		tmp = a.index[i] - move;

		if (tmp < 0)
		{
			res.index[i] = tmp + BASE;
			move = 1;
		}
		else
		{
			res.index[i] = tmp;
			move = 0;
		}
	}

	res = Norm(res);

	return res;
}

LongNum multiply(LongNum a, LongNum b)
{
	unsigned long long int tmp;
	unsigned int move = 0, i, j;

	LongNum res;

	res.size = a.size + b.size;
	res = take(res, res.size);
	res = zero(res, res.size);

	for (i = 0; i < b.size; i++)
	{
		move = 0;

		for (j = 0; j < a.size; j++)
		{
			tmp = (unsigned long long int) b.index[i] * (unsigned long long int) a.index[j] + move + res.index[i + j];
			move = tmp / BASE;
			res.index[i + j] = tmp % BASE;
		}
		res.index[i + a.size] = move;
	}

	res = Norm(res);

	return res;
}

LongNum divide(LongNum a, LongNum b, int sumbols)
{
	LongNum remainder;

	if (compare(a, b) < 0)
	{
		LongNum res;
		res.size = 1;
		res = take(res, res.size);
		res.index[0] = 0;

		if (sumbols == 1)
			return res;
		else
			return a;
	}

	if (b.size == 1)
	{
		if (b.index[0] == 0)
		{
			remainder.size = 0;
			remainder.index = NULL;

			return remainder;
		}

		LongNum res = div(a, b.index[0]);

		remainder = sub(a, mul(res, b.index[0]));

		if (sumbols == 1)
			return res;
		else
			return remainder;
	}

	remainder = copy(a);

	LongNum res;
	res.size = a.size - b.size + 1;
	res = take(res, res.size);
	res = zero(res, res.size);

	unsigned int i;

	for (i = a.size - b.size + 1; i != 0; i--)
	{
		unsigned long long int qGuessMax = BASE, qGuessMin = 0, qGuess;

		while (qGuessMax - qGuessMin > 1)
		{
			qGuess = (qGuessMax + qGuessMin) / 2;

			LongNum tmp = mul(b, qGuess);
			tmp = shift(tmp, i - 1);
			if (compare(tmp, remainder) > 0)
				qGuessMax = qGuess;
			else
				qGuessMin = qGuess;

			tmp = clear(tmp);
		}

		LongNum tmp = mul(b, qGuessMin);
		tmp = shift(tmp, i - 1);

		remainder = sub(remainder, tmp);

		tmp = clear(tmp);

		res.index[i - 1] = qGuessMin;
	}

	remainder = Norm(remainder);

	res = Norm(res);

	if (sumbols == 1)
		return res;
	else
		return remainder;
}

LongNum mul(LongNum a, unsigned long long int b)
{
	unsigned long long int tmp, move = 0;
	unsigned int i, j;

	LongNum res;

	res.size = a.size + 1;
	res = take(res, res.size);
	res = zero(res, res.size);

	move = 0;

	for (j = 0; j < a.size; j++)
	{
		tmp = b * (unsigned long long int) a.index[j] + move + res.index[j];
		move = tmp / BASE;
		res.index[j] = tmp % BASE;
	}

	res.index[a.size] = move;

	res = Norm(res);

	return res;
}

LongNum shift(LongNum a, unsigned int s)
{
	LongNum current;

	current.size = a.size + s;
	current = take(current, current.size);
	current = zero(current, s);

	int i;

	for (i = s; i < a.size + s; i++)
		current.index[i] = a.index[i - s];

	return current;
}

LongNum pow(LongNum a, LongNum b, LongNum c)
{
	LongNum res;
	if (a.size < c.size)
		res.size = c.size + c.size;
	else
		res.size = a.size + a.size;

	res = take(res, res.size);
	res = zero(res, res.size);
	res.index[0] = 1;

	LongNum com;
	com.size = 1;
	com = take(com, com.size);
	com.index[0] = 0;

	LongNum value;
	value.size = res.size;
	value = take(value, value.size);
	value = zero(value, value.size);

	memcpy(value.index, a.index, a.size * sizeof(unsigned int));

	LongNum pow = copy(b);

	while ((compare(pow, com)) > 0)
	{
		if ((pow.index[0] & 1) == 1)
		{
			res = multiply(res, value);
			res = divide(res, c, 2);
		}

		value = multiply(value, value);
		value = divide(value, c, 2);
		pow = div(pow, 2);
	}

	com = clear(com);
	pow = clear(pow);
	value = clear(value);

	return res;
}

LongNum div(LongNum a, unsigned long long int b)
{
	unsigned long long int tmp;
	unsigned int move = 0;
	int i;

	LongNum res;
	res.size = a.size;
	res = take(res, res.size);
	res = zero(res, res.size);

	for (i = a.size - 1; i > -1; i--)
	{
		tmp = (unsigned long long int) move * BASE + a.index[i];
		res.index[i] = tmp / b;
		move = tmp - (unsigned long long int) res.index[i] * (unsigned long long int) b;
	}

	res = Norm(res);

	return res;
}

LongNum Norm(LongNum a)
{
	unsigned int i = a.size - 1;

	while ((i>0) && (a.index[i] == 0))
		i--;

	a.size = i + 1;

	return a;
}
