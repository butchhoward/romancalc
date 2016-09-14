#include "romanmath.h"
#include <string.h>
#include <stdbool.h>

char *strdup(const char *s);

const int INVALID_ROMAN_NUMERAL = 0;

int romanSymbolToInt( const char RomanSymbol )
{
	int value = INVALID_ROMAN_NUMERAL;

	switch (RomanSymbol)
	{
	case 'I':
		value = 1;
		break;
	case 'V':
		value = 5;
		break;
	case 'X':
		value = 10;
		break;
	case 'L':
		value = 50;
		break;
	case 'C':
		value = 100;
		break;
	case 'D':
		value = 500;
		break;
	case 'M':
		value = 1000;
		break;
	default:
		value = INVALID_ROMAN_NUMERAL;
		break;
	};

	return value;
}

int romanAllowedSymbolRepeat(int value)
{
	int repeatCountAllowed = 0;
	switch(value)
	{
	case 1:
	case 10:
	case 100:
		repeatCountAllowed = 3;
		break;
	case 5:
	case 50:
	case 500:
		repeatCountAllowed = 1;
		break;
	case 1000:
		repeatCountAllowed = 4;
		break;
	default: 
		repeatCountAllowed = 0;
	}

	return repeatCountAllowed;
}

bool isAllowedSubtractiveForValue(int possibleSubtractve, int symbolValue)
{
	bool isAllowed = false;

	switch (symbolValue)
	{
	case 5:
	case 10:
		isAllowed = (possibleSubtractve == 1);
		break;
	case 50:
	case 100:
		isAllowed = (possibleSubtractve == 10);
		break;
	case 500:
	case 1000:
		isAllowed = (possibleSubtractve == 100);
		break;
	case 1:
	default:
		isAllowed = false;
	}

	return isAllowed;
}

bool doesSubtractivePreventMoreBase(int baseValue)
{
	bool isPrevented = true;

	switch (baseValue)
	{
	case 10:
	case 100:
	case 1000:
		isPrevented = false;
	}

	return isPrevented;
}

bool isValidRomanNumber(const char* Roman)
{

	printf("====\nisvalidRomanNumber(\"%s\")\n", Roman);

	bool isValid = true;

	int previousSymbolValue = 0;
	int largestSymbolValue = 0;
	int repeatCount = 0;
	
	int index = 0;
	for (index = strlen(Roman)-1; index >=0; --index)
	{
		int symbolValue = romanSymbolToInt(Roman[index]);

		printf("%c %d I=%d P=%d L=%d R=%d\n", 
			Roman[index], 
			symbolValue,
			index, 
			previousSymbolValue, 
			largestSymbolValue,
			repeatCount);

		if (symbolValue == INVALID_ROMAN_NUMERAL)
		{
			isValid = false;
			break;
		}

		if (symbolValue == previousSymbolValue)
		{
			++repeatCount;
			if (repeatCount > romanAllowedSymbolRepeat(symbolValue))
			{
				isValid = false;
				break;
			}
		}
		else
		{
			repeatCount = 1;
		}

		if (isAllowedSubtractiveForValue(symbolValue, previousSymbolValue))
		{
			if( doesSubtractivePreventMoreBase(previousSymbolValue))
			{
				largestSymbolValue = previousSymbolValue + 1;	
			}
		}
		else
		{
			if (symbolValue < largestSymbolValue)
			{
				isValid = false;
				break;
			}
			largestSymbolValue = symbolValue;
		}

		previousSymbolValue = symbolValue;
	}

	printf("%s\n", (isValid?"true":"false"));
	return isValid;
}

int romanToInt( const char* Roman)
{
	if (!isValidRomanNumber(Roman))
	{
		return INVALID_ROMAN_NUMERAL;
	}

	int value = INVALID_ROMAN_NUMERAL;
	int previousSymbolValue = 0;
	
	int index = 0;
	for (index = strlen(Roman)-1; index >=0; --index)
	{
		int symbolValue = romanSymbolToInt(Roman[index]);

		if (isAllowedSubtractiveForValue(symbolValue, previousSymbolValue))
		{
			value -= symbolValue;
		}
		else
		{
			value += symbolValue;
		}

		previousSymbolValue = symbolValue;
	}

	return value;
}

int romanSumInt(const char* Augend, const char* Addend)
{
	int augend = romanToInt(Augend);
	int addend = romanToInt(Addend);
	if (INVALID_ROMAN_NUMERAL == augend || INVALID_ROMAN_NUMERAL == addend)
	{
		return INVALID_ROMAN_NUMERAL;
	}
	return augend + addend;
}

char* romanSum(const char* Augend, const char* Addend)
{
	int sum = romanSumInt(Augend, Addend);
	if (INVALID_ROMAN_NUMERAL == sum)
	{
		return 0;
	}

	return intToRoman(sum);
}

int romanSubtractInt(const char* Menuend, const char* Subtrahend)
{
	int menuend = romanToInt(Menuend);
	int subtrahend = romanToInt(Subtrahend);
	if (INVALID_ROMAN_NUMERAL == menuend || INVALID_ROMAN_NUMERAL == subtrahend || subtrahend > menuend)
	{
		return INVALID_ROMAN_NUMERAL;
	}

	return menuend - subtrahend;
}

char* romanSubtract(const char* Menuend, const char* Subtrahend)
{
	int difference = romanSubtractInt(Menuend, Subtrahend);
	if (INVALID_ROMAN_NUMERAL == difference)
	{
		return 0;
	}

	return intToRoman(difference);
}

char* intToRoman(int Value)
{
	if (Value >= 5000)
	{
		return 0;
	}

	char romanNumeral[64];
	memset(romanNumeral, 0, 64);

	int symbolValues[] =             {1000, 900,  500, 400,  100, 90,   50,  40,   10,   9,    5,   4,    1};
	const char* symbolsForValues[] = {"M",  "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
	int symbolValueCount = sizeof(symbolValues) / sizeof(symbolValues[0]);

	int index = 0;
	for (index = 0; index < symbolValueCount; ++index)
	{
		for (;Value >= symbolValues[index]; Value -= symbolValues[index])
		{
			strcat(romanNumeral, symbolsForValues[index]);
		}
	}

	return strdup(romanNumeral);
}

