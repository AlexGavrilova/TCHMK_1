#include <stdio.h>
#include <string.h>
#include <math.h>
#include <fstream>
#include <iostream>

using namespace std;

//function to clear main from details of numerations
bool calculation(char* fName_In1, char operation, char* fName_In2, char* fNameRes, int base, char* modulFile, bool isModul);

//function to print initial dialogue
void printInitial()
{
    cout << "Long arithmetics. Realised operations are: ";
    cout << " addition - ""+"", substraction - ""-"", multiplication - ""m"", dividing - ""/"", involution - ""^"", taking the residue - ""r"".\n";
    cout << "Command line arguments must be entered this way: \n";
    cout << "[inFile1] [operationSign] [inFile2] [resultFile] [-b(if necessary)] [moduleFile (if necessary)]";
}

//function to check whether number of parameters coincide
bool argumentsOk(int argc, char* argv[])
{
    if (argc < 5)
    {
        cout << "Too few arguments. Try again.\n";
        return false;
    }
    
    if (argc > 7)
    {
        cout << "Too many arguments. Try again.\n";
        return false;
    }
    
    if (strlen(argv[2]) > 1)
    {
        cout << "Wrong operation.\n";
        return false;
    }
    
    return true;
}

//function to read bigNumber from file
bool readFile(int** numMass, char* fileName, int base, int& arrSize, char& numberSign)
{
    ifstream fp;
    
    if (base == 1000000000)
    {
        fp.open(fileName, ios::in);
    }
    else if (base == 256)
    {
        fp.open(fileName, ios::in | ios::binary);
    }
    
    if (fp.fail())
    {
        cout << "Unable to open the file. Error!\n";
        return false;
    }
    
    fp.seekg(0, fp.end);
    int fileSize = fp.tellg();
    int curPos = 0;
    
    fp.seekg(0, fp.beg);
    if (fp.get() == '-')
    {
        numberSign = '-';
        curPos = fileSize;
        fileSize -= 1;
    }
    else
    {
        numberSign = '+';
        curPos = fileSize;
    }
    
    if (base == 1000000000) //define array_size depending on num system base
    {
        if (!(fileSize % 9))
        {
            arrSize = fileSize / 9;
        }
        else
        {
            arrSize = fileSize / 9 + 1;
        }
    }
    else if (base == 256)
    {
        arrSize = fileSize;
    }
    
    (*numMass) = new int[arrSize];
    for (int i = 0; i < (arrSize); i++)
    {
        (*numMass)[i] = 0;
    }
    
    if (base == 1000000000) //not binary - 10^9 num system
    {
        
        for (int i = arrSize; i > 0; i--)
        {
            if ((curPos - 9) >= 0) //if not the last 9 symbols
            {
                curPos -= 9;
                fp.seekg(curPos, fp.beg);
                for (int j = 0; j < 9; j++) //to 10^9 num system (int border)
                {
                    (*numMass)[i - 1] += (fp.get() - 48)*(int)(pow((double)10, (9 - j - 1)));
                }
            }
            else //if the last 9 symbols -> check sign, read reminded
            {
                fp.seekg(0, fp.beg);
                if (fp.get() == '-')
                {
                    fp.seekg(1, fp.beg);
                    for (int j = 1; j < curPos; j++)
                    {
                        (*numMass)[i - 1] += (fp.get() - 48)*(int)(pow((double)10, (curPos - j - 1)));
                    }
                }
                else
                {
                    fp.seekg(0, ios::beg);
                    for (int j = 0; j < curPos; j++)
                    {
                        (*numMass)[i - 1] += (fp.get() - 48)*(int)(pow((double)10, (curPos - j - 1)));
                    }
                }
            }
        }
    }
    else if (base == 256) //if binary - read as it is
    {
        fp.seekg(0, ios::beg);
        for (int i = arrSize; i > 0; i--)
        {
            (*numMass)[i - 1] = fp.get();
        }
    }
    
    fp.close();
   cout << "The number was successfully read from input file.\n";
    return true;
}

//function to write bigNumber to file
bool writeFile(int* numMass, char numSign, int numMassSize, char* resFileName, int base)
{
    ofstream fp;
    if (base == 1000000000)
    {
        fp.open(resFileName, ios::out);
    }
    else if (base == 256)
    {
        fp.open(resFileName, ios::out | ios::binary);
    }
    
    if (fp.fail())
    {
        return false;
    }
    
    if (base == 1000000000) //not binary - 10^9 num system
    {
        if (numSign == '-')
        {
            fp << numSign;
        }
        for (int i = 0; i < numMassSize; i++)
        {
            if (i)
            {
                for (int j = 0; j < 9; j++)
                {
                    if (numMass[i] < (int)pow((double)10, j))
                    {
                        fp << 0;
                    }
                }
            }
            fp << numMass[i];
        }
    }
    
    else if (base == 256) //if binary
    {
        for (int i = numMassSize; i > 0; i--)
        {
            fp.put(numMass[i - 1]);
        }
    }
    
    fp.close();
    cout << "The number was successfully written to file.\n";
    return true;
}

//function to define the sign of the result
char resSign(char operation, char firstSign, char secSign, int* firstArr, int* secArr, int firstArrSize, int secArrSize)
{
    if (operation == '+')
    {
        if (firstArrSize > secArrSize) //if 1st num is longer - its sign
        {
            return firstSign;
        }
        else if (firstArrSize < secArrSize) //if 2nd num is longer - its sign
        {
            return secSign;
        }
        
        else //if 1st and 2nd lengths are equal - compare symbol by symbol
        {
            for (int i = 0; i < firstArrSize; i++)
            {
                if (firstArr[i] > secArr[i])
                {
                    return firstSign;
                }
                
                else if (firstArr[i] < secArr[i])
                {
                    return secSign;
                }
            }
        }
        
        return '=';
    }
    
    if (operation == '*' || operation == '/')
    {
        if (firstSign != secSign)
        {
            return '-';
        }
        else
        {
            return '+';
        }
    }
    
    return firstSign;
}

//function to  define what number is bigger
void toMaxMin(int* firstArr, int* secArr, int firstArrSize, int secArrSize, int** maxArr, int** minArr, int& maxArrSize, int& minArrSize)
{
    if (firstArrSize > secArrSize) //if 1st num is longer - 1st-max, 2nd-min
    {
        maxArrSize = firstArrSize;
        *maxArr = new int[maxArrSize];
        for (int i = 0; i < maxArrSize; i++)
        {
            (*maxArr)[i] = firstArr[i];
        }
        minArrSize = secArrSize;
        *minArr = new int[minArrSize];
        for (int i = 0; i < minArrSize; i++)
        {
            (*minArr)[i] = secArr[i];
        }
    }
    
    else if (firstArrSize < secArrSize) //if 2nd num is longer - 1st-min, 2nd-max
    {
        maxArrSize = secArrSize;
        *maxArr = new int[maxArrSize];
        for (int i = 0; i < maxArrSize; i++)
        {
            (*maxArr)[i] = secArr[i];
        }
        minArrSize = firstArrSize;
        (*minArr) = new int[minArrSize];
        for (int i = 0; i < minArrSize; i++)
        {
            (*minArr)[i] = firstArr[i];
        }
    }
    
    else //if lengths are equal - compare symbol by symbol
    {
        for (int i = 0; i < firstArrSize; i++)
        {
            if (firstArr[i] > secArr[i]) //if 1st num is longer - 1st-max, 2nd-min
            {
                maxArrSize = firstArrSize;
                *maxArr = new int[maxArrSize];
                for (int i = 0; i < maxArrSize; i++)
                {
                    (*maxArr)[i] = firstArr[i];
                }
                minArrSize = secArrSize;
                *minArr = new int[minArrSize];
                for (int i = 0; i < minArrSize; i++)
                {
                    (*minArr)[i] = secArr[i];
                }
                break;
            }
            
            else if (firstArr[i] < secArr[i]) //if 2nd num is longer - 1st-min, 2nd-max
            {
                maxArrSize = secArrSize;
                *maxArr = new int[maxArrSize];
                for (int i = 0; i < maxArrSize; i++)
                {
                    (*maxArr)[i] = secArr[i];
                }
                minArrSize = firstArrSize;
                *minArr = new int[minArrSize];
                for (int i = 0; i < minArrSize; i++)
                {
                    (*minArr)[i] = firstArr[i];
                }
                break;
            }
        }
    }
}

//function to cut head nulls
int* deleteNulls(int* arrYesNulls, int& arrSize)
{
    int nullsNum = 0;
    int* arrNoNulls = NULL;
    
    for (int i = 0; i < arrSize; i++)
    {
        if (arrYesNulls[i] == 0)
        {
            nullsNum++;
        }
        else
        {
            break;
        }
    }
    
    arrSize -= nullsNum;
    if (!arrSize)
    {
        arrSize = 1;
        arrNoNulls = new int[arrSize];
        arrNoNulls[0] = 0;
    }
    
    else
    {
        arrNoNulls = new int[arrSize];
        for (int i = 0; i < arrSize; i++)
        {
            arrNoNulls[i] = arrYesNulls[i + nullsNum];
        }
    }
    
    delete[] arrYesNulls;
    return arrNoNulls;
}

//function to convert long long num to 2 ints (integer and residue)
int* convertToInt(long long int ArrElem, int* longArr, int& longArrSize, int base)
{
    longArrSize = 2;
    longArr = new int[longArrSize];
    for (int i = 0; i < longArrSize; i++)
    {
        longArr[i] = 0;
    }
    
    longArr[0] = (int)(ArrElem / (long long int)base);
    longArr[1] = (int)(ArrElem % (long long int)base);
    
    return deleteNulls(longArr, longArrSize);
}

//function to copy arrays (nums)
int* copyMass(int* newMas, int* oldMas, int& newMasSize, int oldMasSize)
{
    delete[] newMas;
    newMasSize = oldMasSize;
    newMas = new int[newMasSize];
    for (int i = 0; i < newMasSize; i++)
    {
        newMas[i] = oldMas[i];
    }
    
    return deleteNulls(newMas, newMasSize);
}


int main(int argc, char* argv[])
{
    
    printInitial();
    
    if (!argumentsOk(argc, argv))
    {
        return 0;
    }
    
    char* inFIle1 = argv[1];
    char operation = argv[2][0];
    char* inFIle2 = argv[3];
    char* resFile = argv[4];
    char* bin = NULL;
    char* moduleFile = NULL;
    
    int base = 0;
    bool isModule = false;
    
    if (argc == 5) //not binary
    {
        base = 1000000000;
    }
    
    if (argc == 6) // binary or module
    {
        if (!strcmp(argv[5], "-b"))
        {
            bin = argv[5];
            base = 256;
        }
        else
        {
            base = 1000000000;
            moduleFile = argv[5];
            isModule = true;
        }
    }
    
    if (argc == 7) //binary AND module or mistake
    {
        if (!strcmp(argv[5], "-b"))
        {
            bin = argv[5];
            base = 256;
            moduleFile = argv[6];
            isModule = true;
        }
        else
        {
            cout << "Too many arguments. Error!\n";
            return 0;
        }
    }
    
    if (!calculation(inFIle1, operation, inFIle2, resFile, base, moduleFile, isModule))
    {
        printf("There is error in processing. \n");
    }
    
    return 0;
}

//function of summarising (+)
int* addition(int* firstArr, int* secArr, int* resArr, int firstArrSize, int secArrSize, int& resArrSize, int base)
{
    int* biggerNum = NULL;
    int* lessNum = NULL;
    int biggerNumSize = 0;
    int lessNumSize = 0;
    
    toMaxMin(firstArr, secArr, firstArrSize, secArrSize, &biggerNum, &lessNum, biggerNumSize, lessNumSize);
    resArrSize = biggerNumSize + 1;
    resArr = new int[resArrSize];
    
    for (int i = 0; i < resArrSize; i++)
    {
        resArr[i] = 0;
    }
    
    for (int i = resArrSize; i > 1; i--) //add starting from the end
    {
        if ((resArrSize - i) < lessNumSize) //if shorter num hasn't been completely added yet
        {
            resArr[i - 1] += (biggerNum[biggerNumSize - (resArrSize - i) - 1] + lessNum[lessNumSize - (resArrSize - i) - 1]);
        }
        
        else //if shorter number has already been completely added
        {
            resArr[i - 1] += biggerNum[biggerNumSize - (resArrSize - i) - 1];
        }
        
        resArr[i - 2] += resArr[i - 1] / base; //saving integer part and the residue
        resArr[i - 1] %= base;
    }
    
    delete[] biggerNum;
    delete[] lessNum;
    return deleteNulls(resArr, resArrSize);
}

//function sub (-)
int* substraction(int* firstArr, int* secArr, int* resArr, int firstArrSize, int secArrSize, int& resArrSize, int base)
{
    int* biggerNum = NULL;
    int* lessNum = NULL;
    int biggerNumSize = 0;
    int lessNumSize = 0;
    int credit = 0;
    int newSub = 0;
    
    toMaxMin(firstArr, secArr, firstArrSize, secArrSize, &biggerNum, &lessNum, biggerNumSize, lessNumSize);
    resArrSize = biggerNumSize;
    
    resArr = new int[resArrSize];
    
    for (int i = 0; i < resArrSize; i++)
    {
        resArr[i] = 0;
    }
    
    for (int i = resArrSize; i > 0; i--) //sub starting from the end
    {
        if ((resArrSize - i) < lessNumSize) //if shorter num hasn't been completely subbed yet
        {
            newSub = biggerNum[biggerNumSize - (resArrSize - i) - 1] - lessNum[lessNumSize - (resArrSize - i) - 1] - credit;
        }
        
        else //if shorter number has already been completely subbed
        {
            newSub = biggerNum[biggerNumSize - (resArrSize - i) - 1] - credit;
        }
        
        if (newSub < 0) //if there is overflow
        {
            resArr[i - 1] = newSub + base;
            credit = 1;
        }
        else
        {
            resArr[i - 1] = newSub;
            credit = 0;
        }
        
    }
    
    delete[] biggerNum;
    delete[] lessNum;
    return deleteNulls(resArr, resArrSize);
}

//function mul (*)
int* multiplication(int* firstArr, int* secArr, int* resArr, int firstArrSize, int secArrSize, int& resArrSize, int base)
{
    long long int* resArrLong = NULL;
    resArrSize = firstArrSize + secArrSize + 1;
    resArr = new int[resArrSize];
    resArrLong = new long long int[resArrSize];
    
    for (int i = 0; i < resArrSize; i++)
    {
        resArrLong[i] = 0;
        resArr[i] = 0;
    }
    
    for (int i = firstArrSize; i > 0; i--) //mul as by columns
    {
        for (int j = secArrSize; j > 0; j--)
        {
            resArrLong[i + j] += (long long int)(firstArr[i - 1]) * (long long int)(secArr[j - 1]);
        }
    }
    
    for (int i = resArrSize; i > 1; i--) //to int
    {
        resArrLong[i - 2] += resArrLong[i - 1] / base;
        resArrLong[i - 1] %= base;
    }
    
    for (int i = 0; i < resArrSize; i++)
    {
        resArr[i] = (int)resArrLong[i];
    }
    
    delete[] resArrLong;
    
    return deleteNulls(resArr, resArrSize);
}

//function divide bigNum on digit
int* divideDigit(int* firstArr, int digit, int* resArr, int firstArrSize, int& resArrSize, int& tail, bool isRem, int base)
{
    if (!digit)
    {
        printf("Dividing to the zero. Error!\n");
        return firstArr;
    }
    
    resArrSize = firstArrSize;
    resArr = new int[resArrSize];
    for (int i = 0; i < resArrSize; i++)
    {
        resArr[i] = 0;
    }
    
    int remain = 0;
    long long int curDiver = 0;
    for (int i = 0; i < firstArrSize; i++)
    {
        //current numerator
        curDiver = (long long int)remain * (long long int)base + (long long int)firstArr[i];
        resArr[i] = (int)(curDiver / (long long int)digit); //current result
        remain = (int)(curDiver % (long long int)digit); //current residue
    }
    
    tail = remain; //last residue
    return deleteNulls(resArr, resArrSize);
}

//function divide bigNum on bigNum (/)
int* dividing(int* firstArr, int* secArr, int* resArr, int firstArrSize, int secArrSize, int& resArrSize, bool isRem, int base)
{
    int remain = 0;
    if (secArrSize == 1) //if 2nd num is digit
    {
        resArr = divideDigit(firstArr, secArr[0], resArr, firstArrSize, resArrSize, remain, isRem, base);
        if (isRem) //if need residue
        {
            delete[] resArr;
            resArr = new int[1];
            resArr[0] = remain;
            return resArr;
        }
        else
        {
            return resArr;
        }
    }
    
    if (firstArrSize == 1) //if 1st num is digit
    {
        resArrSize = 1;
        resArr = new int[resArrSize];
        
        if (secArrSize == 1)
        {
            if (secArr[0])
            {
                resArr[0] = firstArr[0] / secArr[0];
                return resArr;
            }
            else
            {
                printf("Dividing to the zero. Error!\n");
                return firstArr;
            }
        }
        else
        {
            resArr[0] = 0;
            return resArr;
        }
    }
    
    resArrSize = firstArrSize - secArrSize + 1;
    resArr = new int[resArrSize];
    for (int i = 0; i < resArrSize; i++)
    {
        resArr[i] = 0;
    }
    
    
    int* rest = NULL;
    int* substr = NULL;
    int* submit = NULL;
    int* difference = NULL;
    long long int* newRem = NULL;
    long long int* NewSecArr = NULL;
    int restSize = 0;
    int substrSize = 0;
    int difSize = 0;
    restSize = firstArrSize + 1;
    rest = new int[restSize];
    submit = new int[restSize];
    newRem = new long long int[restSize];
    NewSecArr = new long long int[secArrSize];
    
    for (int i = 0; i < restSize; i++)
    {
        rest[i] = 0;
        newRem[i] = 0;
        submit[i] = 0;
    }
    
    for (int i = 0; i < secArrSize; i++)
    {
        NewSecArr[i] = 0;
    }
    
    int helpValue = 0; //dividing cuts to halfs
    if (!((base / 2) % secArr[0]))
    {
        helpValue = ((base / 2) / secArr[0]);
    }
    
    else
    {
        helpValue = ((base / 2) / secArr[0]) + 1;
    }
    
    for (int i = 1; i < restSize; i++)
    {
        rest[i] = firstArr[i - 1];
    }
    
    for (int i = 0; i < secArrSize; i++)
    {
        NewSecArr[i] = (long long int)secArr[i] * (long long int)helpValue;
    }
    
    int* newLimit = NULL;
    newLimit = new int[1];
    int* newRemInt = NULL;
    int* NewSecArrInt = NULL;
    int* mulArr = NULL;
    int* secMulArr = NULL;
    int* thirdMulArr = NULL;
    int* sumArr = NULL;
    int* baseArr = NULL;
    int* relation = NULL;
    baseArr = new int[1];
    baseArr[0] = base;
    int mulArrSize = 0;
    int secMulArrSize = 0;
    int thirdMulArrSize = 0;
    int sumArrSize = 0;
    int newRemIntSize = 0;
    int NewSecArrIntSize = 0;
    int relationSize = 0;
    
    // calculate new residue
    for (int i = 0; i < resArrSize; i++)
    {
        for (int j = 0; j < restSize; j++)
        {
            newRem[j] = (long long int)rest[j] * (long long int)helpValue;
        }
        
        newRemInt = convertToInt(newRem[i], newRemInt, newRemIntSize, base);
        mulArr = multiplication(newRemInt, baseArr, mulArr, newRemIntSize, 1, mulArrSize, base); //mulArr=residue*helpValue*base
        delete[] newRemInt;
        newRemInt = convertToInt(newRem[i + 1], newRemInt, newRemIntSize, base);
        sumArr = addition(mulArr, newRemInt, sumArr, mulArrSize, newRemIntSize, sumArrSize, base); //sumArr=residue*helpValue*base + newRem[i]
        delete[] newRemInt;
        relation = divideDigit(sumArr, (int)NewSecArr[0], relation, sumArrSize, relationSize, remain, false, base); //relation = residue*helpValue*base + newRem[i-1]*helpValue / secArr[i]*helpValue
        
        if (relationSize > 1)
        {
            newLimit[0] = (base - 1);
        }
        else
        {
            newLimit[0] = relation[0];
        }
        
        delete[] relation;
        
        do
        {
            delete[] mulArr;
            delete[] sumArr;
            
            NewSecArrInt = convertToInt(NewSecArr[1], NewSecArrInt, NewSecArrIntSize, base);
            mulArr = multiplication(NewSecArrInt, newLimit, mulArr, NewSecArrIntSize, 1, mulArrSize, base);
            delete[] NewSecArrInt;
            
            newRemInt = convertToInt(newRem[i], newRemInt, newRemIntSize, base);
            secMulArr = multiplication(newRemInt, baseArr, secMulArr, newRemIntSize, 1, secMulArrSize, base);
            delete[] newRemInt;
            newRemInt = convertToInt(newRem[i + 1], newRemInt, newRemIntSize, base);
            sumArr = addition(secMulArr, newRemInt, sumArr, secMulArrSize, newRemIntSize, sumArrSize, base); //newRem[i]*base+newRem[i]
            delete[] newRemInt;
            delete[] secMulArr;
            NewSecArrInt = convertToInt(NewSecArr[0], NewSecArrInt, NewSecArrIntSize, base);
            secMulArr = multiplication(NewSecArrInt, newLimit, secMulArr, NewSecArrIntSize, 1, secMulArrSize, base); //newSecArr[0]*newLimit
            delete[] NewSecArrInt;
            thirdMulArr = substraction(sumArr, secMulArr, thirdMulArr, sumArrSize, secMulArrSize, thirdMulArrSize, base); //newRem[i]*base+newRem[i] - newSecArr[0]*newLimit
            delete[] sumArr;
            delete[] secMulArr;
            secMulArr = multiplication(thirdMulArr, baseArr, secMulArr, thirdMulArrSize, 1, secMulArrSize, base); //(newRem[i]*base+newRem[i] - newSecArr[0]*newLimit) * base
            delete[] thirdMulArr;
            newRemInt = convertToInt(newRem[i + 2], newRemInt, newRemIntSize, base);
            sumArr = addition(secMulArr, newRemInt, sumArr, secMulArrSize, newRemIntSize, sumArrSize, base); //(newRem[i]*base+newRem[i] - newSecArr[0]*newLimit) * base + newRem[i+2]
            delete[] secMulArr;
            delete[] newRemInt;
            
            if (resSign('+', '+', '-', mulArr, sumArr, mulArrSize, sumArrSize) == '+')
            {
                newLimit[0]--; //decrease the limit while mulArr is bigger than SumArr
            }
        } while (resSign('+', '+', '-', mulArr, sumArr, mulArrSize, sumArrSize) == '+');
        
        delete[] mulArr;
        delete[] sumArr;
        
        substr = multiplication(secArr, newLimit, substr, secArrSize, 1, substrSize, base); //delitel*newLimit
        for (int j = 0; j < (i + 1 - (substrSize - secArrSize)); j++)
        {
            submit[j] = 0;
        }
        for (int j = (i + 1 - (substrSize - secArrSize)); j < (secArrSize + 1 + i); j++)
        {
            submit[j] = substr[j - i - 1 + (substrSize - secArrSize)]; //4islo proverka
        }
        for (int j = (secArrSize + 1 + i); j < restSize; j++)
        {
            submit[j] = 0;
        }
        
        if (resSign('+', '+', '-', rest, submit, restSize, restSize) == '-') //ostatok>proverki
        {
            newLimit[0]--; //decrease the limit
            
            delete[] substr;
            
            substr = multiplication(secArr, newLimit, substr, secArrSize, 1, substrSize, base); //secArr*newLimit
            
            for (int j = 0; j < (i + 1 - (substrSize - secArrSize)); j++)
            {
                submit[j] = 0;
            }
            for (int j = (i + 1 - (substrSize - secArrSize)); j < (secArrSize + 1 + i); j++)
            {
                submit[j] = substr[j - i - 1 + (substrSize - secArrSize)]; //4islo proverka
            }
            for (int j = (secArrSize + 1 + i); j < restSize; j++)
            {
                submit[j] = 0;
            }
        }
        
        difference = substraction(rest, submit, difference, restSize, restSize, difSize, base); //ostatok-proverka
        
        resArr[i] = newLimit[0]; //result[i]=newLimit
        for (int i = restSize; i >(restSize - difSize); i--)
        {
            rest[i - 1] = difference[difSize - (restSize - i) - 1]; //change the residue
        }
        for (int i = (restSize - difSize); i > 0; i--)
        {
            rest[i - 1] = 0;
        }
        delete[] difference;
        delete[] substr;
    }
    
    delete[] submit;
    delete[] newRem;
    delete[] NewSecArr;
    delete[] newLimit;
    delete[] baseArr;
    
    if (isRem)
    {
        delete[] resArr;
        resArr = deleteNulls(rest, restSize);
        resArrSize = restSize;
        return resArr;
    }
    else
    {
        delete[] rest;
        return deleteNulls(resArr, resArrSize);
    }
}

//function ^
int* involution(int* firstArr, int* secArr, int* resArr, int firstArrSize, int secArrSize, int& resArrSize, int base)
{
    resArr = new int[1];
    resArr[0] = 1;
    resArrSize = 1;
    int* oneArr = NULL;
    oneArr = new int[1];
    oneArr[0] = 1;
    int* tmpDived = NULL;
    int tmpDivedSize = 0;
    int* tmpMulArr = NULL;
    int tmpMulArrSize = 0;
    int* tmpSubbed = NULL;
    int tmpSubbedSize = 0;
    int* tmpFirst = NULL;
    int tmpFirstSize = 0;
    int* tmpSecond = NULL;
    int tmpSecondSize = 0;
    int remain = 0;
    
    tmpFirst = copyMass(tmpFirst, firstArr, tmpFirstSize, firstArrSize);
    tmpSecond = copyMass(tmpSecond, secArr, tmpSecondSize, secArrSize);
    
    while (!((tmpSecondSize == 1) && (tmpSecond[0] == 1)))
    {
        tmpDived = divideDigit(tmpSecond, 2, tmpDived, tmpSecondSize, tmpDivedSize, remain, false, base);
        if (!remain) //if delitsa na 2 umnozaem na tekushcii
        {
            tmpSecond = copyMass(tmpSecond, tmpDived, tmpSecondSize, tmpDivedSize);
            delete[] tmpDived;
            tmpMulArr = multiplication(tmpFirst, tmpFirst, tmpMulArr, tmpFirstSize, tmpFirstSize, tmpMulArrSize, base);
            tmpFirst = copyMass(tmpFirst, tmpMulArr, tmpFirstSize, tmpMulArrSize);
            delete tmpMulArr;
        }
        else //esli net umnozaem na pervoe chislo
        {
            tmpSubbed = substraction(tmpSecond, oneArr, tmpSubbed, tmpSecondSize, 1, tmpSubbedSize, base);
            tmpSecond = copyMass(tmpSecond, tmpSubbed, tmpSecondSize, tmpSubbedSize);
            delete[] tmpSubbed;
            tmpMulArr = multiplication(tmpFirst, resArr, tmpMulArr, tmpFirstSize, resArrSize, tmpMulArrSize, base);
            resArr = copyMass(resArr, tmpMulArr, resArrSize, tmpMulArrSize);
            delete[] tmpMulArr;
        }
    }
    
    tmpMulArr = multiplication(tmpFirst, resArr, tmpMulArr, tmpFirstSize, resArrSize, tmpMulArrSize, base);
    resArr = copyMass(resArr, tmpMulArr, resArrSize, tmpMulArrSize);
    delete[] tmpMulArr;
    
    delete[] oneArr;
    delete[] tmpSecond;
    delete[] tmpFirst;
    return resArr;
}

int* rest(int* firstArr, int* secArr, int* resArr, int firstArrSize, int secArrSize, int& resArrSize, int base)
{
    resArr = dividing(firstArr, secArr, resArr, firstArrSize, secArrSize, resArrSize, true, base);
    return resArr;
}

bool calculation(char* fName_In1, char operation, char* fName_In2, char* fNameRes, int base, char* modulFile, bool isModul)
{
    int* firstArr = NULL;
    int* secArr = NULL;
    int* resArr = NULL;
    int firstArrSize = 0;
    int secArrSize = 0;
    int resArrSize = 0;
    char firstSign = '\0';
    char secSign = '\0';
    char resNumSign = '\0';
    
    if (!readFile(&firstArr, fName_In1, base, firstArrSize, firstSign))
    {
        printf("An error occured in reading the first number!\n");
        return false;
    }
    
    if (!readFile(&secArr, fName_In2, base, secArrSize, secSign))
    {
        printf("An error occured in reading the second number!\n");
        return false;
    }
    
    char tempOper = '\0';
    
    if (operation == '-')
    {
        tempOper = '-';
        operation = '+';
        if (secSign == '-')
        {
            secSign = '+';
        }
        else
        {
            secSign = '-';
        }
    }
    
    resNumSign = resSign(operation, firstSign, secSign, firstArr, secArr, firstArrSize, secArrSize);
    
    if (tempOper != '\0')
    {
        operation = tempOper;
        if (secSign == '-')
        {
            secSign = '+';
        }
        else
        {
            secSign = '-';
        }
    }
    
    if (firstSign != secSign)
    {
        if (operation == '+')
        {
            operation = '-';
        }
        else if (operation == '-')
        {
            operation = '+';
        }
    }
    
    switch (operation)
    {
        case '+':
            resArr = addition(firstArr, secArr, resArr, firstArrSize, secArrSize, resArrSize, base);
            break;
        case '-':
            resArr = substraction(firstArr, secArr, resArr, firstArrSize, secArrSize, resArrSize, base);
            break;
        case 'm':
            resArr = multiplication(firstArr, secArr, resArr, firstArrSize, secArrSize, resArrSize, base);
            break;
        case '/':
            resArr = dividing(firstArr, secArr, resArr, firstArrSize, secArrSize, resArrSize, false, base);
            break;
        case '^':
            resArr = involution(firstArr, secArr, resArr, firstArrSize, secArrSize, resArrSize, base);
            break;
        case 'r':
            resArr = rest(firstArr, secArr, resArr, firstArrSize, secArrSize, resArrSize, base);
            break;
        default:
            printf("Wrong operation. Error.\n");
            return false;
            break;
    }
    
    if (isModul)
    {
        int* modNumArr = NULL;
        int* resNumModArr = NULL;
        int modNumArrSize = 0;
        int resNumModArrSize = 0;
        char modNumSign = '\0';
        char resNumModSign = '\0';
        
        if (!readFile(&modNumArr, modulFile, base, modNumArrSize, modNumSign))
        {
            printf("An error occured in reading the module!\n");
            return false;
        }
        
        resNumModArr = rest(resArr, modNumArr, resNumModArr, resArrSize, modNumArrSize, resNumModArrSize, base);
        
        delete[] resArr;
        resArrSize = resNumModArrSize;
        resArr = new int[resArrSize];
        for (int i = 0; i < resArrSize; i++)
        {
            resArr[i] = resNumModArr[i];
        }
        delete[] modNumArr;
        delete[] resNumModArr;
    }
    
    if (!writeFile(resArr, resNumSign, resArrSize, fNameRes, base))
    {
        printf("An error occured in writing the result\n");
    }
    
    delete[] firstArr;
    delete[] secArr;
    delete[] resArr;
    
    return true;
}
