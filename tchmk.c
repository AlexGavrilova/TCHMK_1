#include "tchmk.h"

struct LongNumber ReadNumber(unsigned long long int value)
{
	struct LongNumber num;

	num.pointer = (unsigned int*)malloc(sizeof(unsigned int)*(3));
	num.size = 0;

	unsigned long long int carry = value;

	do
	{
		num.size++;
		num.pointer[num.size - 1] = carry % BASE;
		carry = carry / BASE;
	} while (carry);

	return num;
}


char* PrintNumber(struct LongNumber number)
{
	struct LongNumber decimal;

	decimal.size = number.size*10;

	decimal = allocate(decimal, decimal.size);
	decimal = zero(decimal, decimal.size);

	unsigned int a, j = 0;
	long long int tmp, i = number.size-1;
	char carry = 0;

	while (number.size != 1 || number.pointer[0]!=0)
	{
		carry = 0;

		for (i = number.size - 1; i >= 0; i--)
		{
			tmp = carry * BASE + number.pointer[i];
			number.pointer[i] = tmp / 10;
			carry = tmp - (long long int) number.pointer[i] * 10;
		}

		carry += '0';

		decimal.pointer[j] = carry;
		j++;

		number = Normalize(number);
	}

	//decimal = Normalize(decimal);

	char *string = NULL;
	j = 0;

	string = (char*)malloc(sizeof(char)*(1));
	string[0] = '\0';

	for (i = decimal.size - 1 ; i > -1; i--)
		printf("%c", decimal.pointer[i]);

	free(decimal.pointer);
	decimal.size = 0;

	return string;
}

struct LongNumber ReadString(const char* num)
{
	struct LongNumber bin, number;
	char ch;
	long long int i = 0;

	for (; i < strlen(num); i++)
		if (num[i] < '0' || num[i] > '9')
		{
			printf("Error: Wrong string: %s \n", num);
			bin.pointer = (unsigned int*)malloc(sizeof(unsigned int)*(1));
			bin.pointer[0] = 0;
			bin.size = 1;
			return bin;
		}

	number.size = strlen(num);

	number = allocate(number, number.size);
	number = zero(number, number.size);

	bin.size = number.size / 9 + 1;

	bin = allocate(bin, bin.size);
	bin = zero(bin, bin.size);

	unsigned int a, carry = 0, tmp, current, j, x;

	i = number.size-1;

	for(; i > -1; --i)
		number.pointer[i] = num[number.size - i - 1 ] - '0';

	current = 1;
	j = 0;
	x = 0;

	while (number.size != 1 || number.pointer[0]!=0)
	{
		carry = 0;

		for (i = number.size - 1; i >= 0; i--)
		{
			tmp = carry * 10 + number.pointer[i];
			number.pointer[i] = tmp / 2;
			carry = tmp - number.pointer[i] * 2;
		}

	//	number = Normalize(number);

		bin.pointer[j] = ((current << x) * carry) | bin.pointer[j];

		x++;

		if(x == 32)
		{
			x = 0;
			j++;
		}
	}

	number = clear(number);

//	bin = Normalize(bin);

	return bin;
}

struct LongNumber ReadBinFile(const char* file)
{
	FILE* f = fopen(file, "r");
    if (!f) 
	{
        printf("Error: Unable to open file: %s \n", file);
		struct LongNumber number;
		number.size = 1;
		number.pointer = (unsigned int*)malloc(sizeof(unsigned int)*(1));
		number.pointer[0] = 0;
		fclose(f);
        return number;
    }
	fclose(f);

	FILE* in = fopen(file, "rb");
	struct LongNumber number;
	number.size = 0;

	fseek(in, 0, SEEK_END);
	unsigned int quotient, remainder, size = ftell(in);

	quotient = size / sizeof(unsigned int);
	remainder = size % sizeof(unsigned int);

	if(remainder==0)
		number = allocate(number, quotient);
	else
		number = allocate(number, quotient+1);

	fseek(in, SEEK_SET, 0);  //ïåðåõîä íà íà÷àëî ñòðîêè

	int i, j;

	for(i = 0; i < quotient; i++)
		fread(&number.pointer[i], sizeof(unsigned int), 1, in);

	number.size = quotient;

	if (remainder != 0)
	{
		unsigned char ch;
		number.size++;
		number.pointer[quotient] = 0;

		for(j = 0; j<remainder; j++)
		{
			fread(&ch, sizeof(unsigned char), 1, in);
			number.pointer[quotient] |= (ch << j*8);
		}
	}

	fclose(in);		

	return number;
}

void WriteBinFile(const char* file, struct LongNumber number)
{
	FILE* out = fopen(file,"wb");

	int i;

	for (i = 0 ; i < number.size; i++)
		fwrite(&number.pointer[i], sizeof(unsigned int), 1, out);

	fclose(out);
}

struct LongNumber ReadTextFile(const char* file)
{
	FILE* f = fopen(file, "r");
    if (!f) 
	{
        printf("Error: Unable to open file: %s \n", file);
		struct LongNumber number;
		number.size = 1;
		number.pointer = (unsigned int*)malloc(sizeof(unsigned int)*(1));
		number.pointer[0] = 0;
		fclose(f);
        return number;
    }
	fclose(f);

	struct LongNumber bin, number;
	char ch;

	number.size = 0;

	FILE *in = fopen(file, "r");

	fseek(in, 0, SEEK_END);
	number.size = ftell(in);

	fseek(in, SEEK_SET, 0);  

	number = allocate(number, number.size);
	number = zero(number, number.size);

	bin.size = number.size / 9 + 1;

	bin = allocate(bin, bin.size);
	bin = zero(bin, bin.size);

	unsigned int a, carry = 0, tmp, current, j, x;

	long long int i = number.size-1;

	while((ch = getc(in))!=EOF)
		number.pointer[i--] = ch - '0';

	fclose(in);
	
	current = 1;
	j = 0;
	x = 0;

	while (number.size != 1 || number.pointer[0]!=0)
	{
		carry = 0;

		for (i = number.size - 1; i >= 0; i--)
		{
			tmp = carry * 10 + number.pointer[i];
			number.pointer[i] = tmp / 2;
			carry = tmp - number.pointer[i] * 2;
		}

	//	number = Normalize(number);

		bin.pointer[j] = ((current << x) * carry) | bin.pointer[j];

		x++;

		if(x == 32)
		{
			x = 0;
			j++;
		}
	}

	number = clear(number);

//	bin = Normalize(bin);

	return bin;
}

void WriteTextFile(const char* file, struct LongNumber number)
{
	FILE* out = fopen(file, "w");

	struct LongNumber decimal;

	decimal.size = number.size*10;

	decimal = allocate(decimal, decimal.size);
	decimal = zero(decimal, decimal.size);

	unsigned int a, j = 0;
	long long int tmp, i = number.size-1;
	char carry =0;

	while (number.size != 1 || number.pointer[0]!=0)
	{
		carry = 0;

		for (i = number.size - 1; i >= 0; i--)
		{
			tmp = carry * BASE + number.pointer[i];
			number.pointer[i] = tmp / 10;
			carry = tmp - (long long int) number.pointer[i] * 10;
		}

		carry += '0';

		decimal.pointer[j] = carry;
		j++;

		number = Normalize(number);
	}

//	decimal = Normalize(decimal);

	for (i = decimal.size - 1 ; i > -1; i--)
			fprintf(out, "%c", decimal.pointer[i]);

	fclose(out);
}

struct LongNumber clear(struct LongNumber number)
{
	free(number.pointer);
	return number;
}

struct LongNumber allocate(struct LongNumber number, unsigned int size)
{
	number.pointer = (unsigned int*)malloc(sizeof(unsigned int)*(size));
	return number;
}

struct LongNumber zero(struct LongNumber number, unsigned int size)
{
	memset(number.pointer, 0, size * sizeof(unsigned int));
	return number;
}

struct LongNumber copy(struct LongNumber from)
{
    struct LongNumber cpy;

    cpy.size = from.size;
    cpy = allocate(cpy, cpy.size);
    memcpy(cpy.pointer, from.pointer, cpy.size * sizeof(unsigned int));

    return cpy;
}

int compare(struct LongNumber a, struct LongNumber b)
{	
	if(a.size > b.size)
		return 1;

	if(a.size < b.size)
		return -1;

	int i = a.size - 1;

	while (a.pointer[i] == b.pointer[i] && i > 0)
		i--;
	
	if(a.pointer[i] > b.pointer[i])
		return 1;

	if(a.pointer[i] < b.pointer[i])
		return -1;

	if(a.pointer[i] == b.pointer[i])
		return 0;
}

struct LongNumber ADD(struct LongNumber a, struct LongNumber b)
{
	unsigned long long int tmp;
	unsigned int carry = 0, i;
	struct LongNumber res;

	if (a.size < b.size)
	{
		return ADD(b,a);
	}

	res.size = a.size + 1;
	res = allocate(res, res.size);
	res = zero(res, res.size);

	for (i=0; i<b.size; i++) 
	{
		tmp = (unsigned long long int) a.pointer[i] + (unsigned long long int) b.pointer[i] + (unsigned long long int) carry;
		if (tmp >= BASE) 
		{ 
			res.pointer[i] = tmp - BASE;
			carry = 1;
		} 
		else 
		{
			res.pointer[i] = tmp;
			carry = 0;
		}
	}

	for (; i < a.size; i++) 
	{
		tmp = (unsigned long long int) a.pointer[i] + (unsigned long long int) carry;

		if (tmp >= BASE) 
		{
			res.pointer[i] = tmp - BASE;
			carry = 1;
		} 
		else 
		{
			res.pointer[i] = tmp;
			carry = 0;
		}
	}

	if (carry==1) 
	{
		res.pointer[i] = carry;
		res.size = a.size + 1;
	}
	else
		res.size = a.size;

	return res;
}

struct LongNumber SUB(struct LongNumber a, struct LongNumber b)
{
	long long int tmp;
	unsigned int carry = 0, i;
	struct LongNumber res;

	if (compare(a, b) < 0)
	{
		res = SUB(b,a);
		return res;
	}

	res.size = a.size;
	res = allocate(res, res.size);
	res = zero(res, res.size);

	for (i=0; i<b.size; i++) 
	{
		tmp = (long long int)a.pointer[i] - (long long int) b.pointer[i] - (long long int) carry;

		if (tmp < 0) 
		{
			res.pointer[i] = tmp + BASE;
			carry = 1;
		} 
		else 
		{
			res.pointer[i] = tmp;
			carry = 0;
		}
	}

	for (; i<a.size; i++) 
	{
		tmp = a.pointer[i] - carry;

		if (tmp < 0) 
		{
			res.pointer[i] = tmp + BASE;
			carry = 1;
		} 
		else 
		{
			res.pointer[i] = tmp;
			carry = 0;
		}
	}

	res = Normalize(res);

	return res;
}

struct LongNumber MUL(struct LongNumber a, struct LongNumber b)
{
	unsigned long long int tmp;
	unsigned int carry = 0, i, j;

	struct LongNumber res;

    res.size = a.size + b.size;
    res = allocate(res, res.size);
    res = zero(res, res.size); 

    for (i = 0; i < b.size; i++)
    {
        carry = 0;

        for (j = 0; j < a.size; j++)
        {
            tmp = (unsigned long long int) b.pointer[i] * (unsigned long long int) a.pointer[j] + carry + res.pointer[i + j];
            carry = tmp / BASE;
            res.pointer[i + j] = tmp % BASE;
        }
        res.pointer[i + a.size] = carry;
    }

    res = Normalize(res);

    return res;
}

struct LongNumber DIV(struct LongNumber a, struct LongNumber b, int sumbols)
{
	struct LongNumber remainder; 

    if (compare(a, b) < 0)
    {
        struct LongNumber res;
        res.size = 1;
        res = allocate(res, res.size);
        res.pointer[0] = 0;

		if(sumbols == 1)
			return res;
		else
			return a;
    }
	if (compare(a,b)==0)
	{
		struct LongNumber res;
		res.size = 1;
		res = allocate(res, res.size);
		res.pointer[0] = 1;		
			return res;
	}
    if (b.size == 1)
    {
		if (b.pointer[0] == 0)
        {
            remainder.size = 0;
            remainder.pointer = NULL;

            return remainder;
        }

		struct LongNumber res = SmallDIV(a, b.pointer[0]);

        remainder = SUB(a, SmallMul(res,b.pointer[0]));

        if(sumbols == 1)
			return res;
		else
			return remainder;
    }
	
	remainder = copy(a);

    struct LongNumber res;
    res.size = a.size - b.size + 1;
    res = allocate(res, res.size);
	res = zero(res, res.size); 

    unsigned int i;
	
    for (i = a.size - b.size + 1; i != 0; i--)
    {
        unsigned long long int qGuessMax = BASE, qGuessMin = 0, qGuess; 
		
        while (qGuessMax - qGuessMin > 1)
        {
            qGuess = (qGuessMax + qGuessMin) / 2;

            struct LongNumber tmp = SmallMul(b, qGuess);
            tmp = shiftLeft(tmp, i - 1);   
            if (compare(tmp, remainder) > 0)
                qGuessMax = qGuess;
            else
                qGuessMin = qGuess;
            
            tmp = clear(tmp);
        }

        struct LongNumber tmp = SmallMul(b, qGuessMin);
        tmp = shiftLeft(tmp, i - 1);

        remainder = SUB(remainder, tmp);

		tmp = clear(tmp);
        
        res.pointer[i - 1] = qGuessMin;
    }

	remainder = Normalize(remainder);

	res = Normalize(res);

    if(sumbols == 1)
		return res;
	else
		return remainder;
}

struct LongNumber SmallMul(struct LongNumber a, unsigned long long int b)
{
	unsigned long long int tmp, carry = 0;
	unsigned int i, j;

	struct LongNumber res;

    res.size = a.size + 1;
    res = allocate(res, res.size);
    res = zero(res, res.size); 

	carry = 0;

	for (j = 0; j < a.size; j++)
	{
		tmp = b * (unsigned long long int) a.pointer[j] + carry + res.pointer[j];
		carry = tmp / BASE;
        res.pointer[j] = tmp % BASE;
    }

    res.pointer[a.size] = carry;

    res = Normalize(res);

    return res;
}

struct LongNumber shiftLeft(struct LongNumber a, unsigned int s) 
{
    struct LongNumber current;
	
	current.size = a.size + s;
	current = allocate(current, current.size);
	current = zero(current, s);

	int i;

	for(i = s; i < a.size + s; i++)
		current.pointer[i] = a.pointer[i-s];

    return current;
}

struct LongNumber DEGREE(struct LongNumber a, struct LongNumber b, struct LongNumber c)
{ 
	struct LongNumber res;
	if(a.size < c.size)
		res.size = c.size + c.size;
	else
		res.size = a.size + a.size;

	res = allocate(res, res.size);
    res = zero(res, res.size); 
	res.pointer[0] = 1;

	struct LongNumber com;
	com.size = 1;
	com = allocate(com, com.size);
	com.pointer[0] = 0;

	struct LongNumber value;
	value.size = res.size;
	value = allocate(value, value.size);
    value = zero(value, value.size);

	memcpy(value.pointer, a.pointer, a.size * sizeof(unsigned int));

	struct LongNumber pow = copy(b);

	while((compare(pow,com)) > 0)
	{
		if((pow.pointer[0] & 1)==1)
		{
			res = MUL(res, value);
			res = DIV(res, c, 2);
		}

		value = MUL(value, value);
		value = DIV(value, c, 2);
		pow = SmallDIV(pow, 2);
	}

	com = clear(com);
	pow = clear(pow);
	value = clear(value);

	return res;
}

struct LongNumber SmallDIV(struct LongNumber a, unsigned long long int b)
{
		unsigned long long int tmp;
		unsigned int carry = 0;
		int i;

        struct LongNumber res;
        res.size = a.size;
        res = allocate(res, res.size);
		res = zero(res, res.size); 

        for (i = a.size - 1; i > -1; i--)
        {	
            tmp = (unsigned long long int) carry * BASE + a.pointer[i];
            res.pointer[i] = tmp / b;
            carry = tmp - (unsigned long long int) res.pointer[i] * (unsigned long long int) b;
        }

        res = Normalize(res);

		return res;
}

struct LongNumber Normalize(struct LongNumber a)
{
/*	unsigned int i = a.size-1;

	while ( (i>0) && (a.pointer[i]==0) ) 
		i--;

	a.size = i+1;

	return a; */
} 
