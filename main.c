#include "tchmk.h"

int main(int argc, const char * argv[])
{
    if (argc < 5 || argc > 7)
    {
        printf("You must be enter more than 5 and fewer than 7 arguments. For example:\n");
	printf("<file_a> <[ + | - | x | / | % | ^ ]> <file_b> <file_result> <file_module>\n");
	printf("or:\n");
	printf("<file_a> <[ + | - | x | / | % | ^ ]> <file_b> <file_result> <file_module> <-b>\n");
	return 0;
    }
    int bin = 0;
    if (argc == 5)
    {
        if (argv[2][0] == '^')
        {
            printf("You must be enter module file\n");
            return 0;
        }
    }
    if (argc == 6)
    {
        if (strcmp(argv[5], "-b") == 0)
            bin = 1;
        else
            bin = 0;
        if (bin == 0)
        {
            if (argv[2][0] != '^')
            {
                printf("Bad argument %s!\n", argv[5]);
                return 0;
            }
            else
            {
                FILE* Module_File = fopen(argv[5], "r");
                if (!Module_File)
                {
                    printf("Module file %s not found!\n",argv[5]);
                    return 0;
                }
                fclose(Module_File);
            }
        }
    }
    if (argc == 7)
    {
        if (strcmp(argv[6], "-b") == 0)
            bin = 1;
        else
        {
            printf("What this %s?\n",argv[6]);
            return 0;
        }
        if (argv[2][0] != '^')
        {
            printf("Bad argument %s!\n", argv[5]);
            return 0;
        }
        else
        {
            FILE* Module_File = fopen(argv[5], "r");
            if (!Module_File)
            {
                printf("Module file %s not found!\n",argv[5]);
                return 0;
            }
            fclose(Module_File);
        }
    }
    FILE* Long_A_File = fopen(argv[1], "r");
    if (!Long_A_File)
    {
        printf("File %s not found!\n", argv[1]);
        return 0;
    }
    fclose(Long_A_File);
    FILE* Long_B_File = fopen(argv[3], "r");
    if (!Long_B_File)
    {
        printf("File %s not found!\n", argv[3]);
        return 0;
    }
    fclose(Long_B_File);
    FILE* Result_File = fopen(argv[4], "w");
    fclose(Result_File);
    const char* op = argv[2];
    if ((strlen(op) > 1 || op[0] == '\0') || op[0] != '+' && op[0] != '-' && op[0] != 'x' && op[0] != '/' && op[0] != '%' && op[0] != '^')
    {
        printf("Bad operation: %s \n", op);
        return 0;
    }
    struct LongNumber a,b;
    if  (bin == 1)
    {
        a = ReadBinFile(argv[1]);
        b = ReadBinFile(argv[3]);  
    }
    else
    {
        a = ReadTextFile(argv[1]);
        b = ReadTextFile(argv[3]);
    }
    struct LongNumber result;
    switch (op[0]) 
    {
        case '+':
        {
            result = ADD(a, b);
            break;
        }
        case '-':
        {
            result = SUB(a, b);
            break;
        }
        case 'x':
        {
            result = MUL(a, b);
            break;
        }
        case '/':
        {
            result = DIV(a, b, 1);
            break;
        }
        case '%':
        {
            result = DIV(a, b, 2);
            break;
        }
        case '^':
        {
            struct LongNumber c;
            if(bin == 1)
                c = ReadBinFile(argv[5]);
            else
                c = ReadTextFile(argv[5]);
            result = DEGREE(a, b, c);
            c = clear(c);
            break;
        }
        default:
            break;
    }
    if (bin == 1)
        WriteBinFile(argv[4], result);
    else
        WriteTextFile(argv[4], result);
    a = clear(a);
    b = clear(b);
    result = clear(result);
    return 0;           
}  
