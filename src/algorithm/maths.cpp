#include "maths.h"
#include <math.h>		// system <maths.h>
#include "macrodef.h"
#include "myassert.h"
#include <iostream>
using std::swap;

namespace algorithm {

	//ÅÐ¶ÏÒ»¸öintÊÇ·ñÎªËØÊý
	bool isPrime(uint n)
	{
		int find = static_cast<int>(sqrt(static_cast<float>(n)));
		MyAssert(find > 0);
		for (int i = 2; i <= find; ++i)
		{
			if (n % i == 0)
			{
				return false;
			}
		}
		return true;
	}

	//ÕÒ³öÐ¡ÓÚnµÄ×î´óËØÊý
	uint minPrime(uint n)
	{
		while (n)
		{
			if (isPrime(n))
			{
				return n;
			}
			n--;
		}
		return 0;
	}

	//ÇóbaseµÄtimes´Î·½
	int pow(int base, int times)
	{
		int result = base;
		while (--times) 
		{
			result *= base;
		}
		return result;
	}

    uint	rand_uint() 
    {
		srand((uint)time(NULL));
        uint inta = (rand() & 0x00000FFF);
        uint intb = ((rand() & 0x00000FFF) << 12);
        uint intc = ((rand() & 0x000000FF) << 24);
        uint result = inta + intb + intc;
        return result;
    }

    uint	get_rand(int start, int end)
    {
        return (rand_uint()) % (end - start + 1) + (start);
    }

    std::string		get_randstr(int length, char* prefix, bool letters, 
        bool numbers, bool symbols) 
    {
        // the shortest way to do this is to create a string, containing
        // all possible values. Then, simply add a random value from that string
        // to our return value
        std::string allpossible; // this will contain all necessary characters
        std::string str = prefix; // the random string

        if (letters) 
        { // if you passed true for letters, we'll add letters to the possibilities
            for (int i = 65; i <= 90; i++) 
            {
                allpossible += static_cast<char>(i);
                allpossible += static_cast<char>(i + 32); // add a lower case letter, too!
            }
        } 
        if (numbers) 
        { // if you wanted numbers, we'll add numbers
            for (int i = 48; i <= 57; i++) 
            {
                allpossible += static_cast<char>(i);
            }
        } 
        if (symbols) 
        { // if you want symbols, we'll add symbols (note, their ASCII values are scattered)
            for (int i = 33; i <= 47; i++)
            {
                allpossible += static_cast<char>(i);
            } 
            for (int i = 58; i <= 64; i++) 
            {
                allpossible += static_cast<char>(i);
            } 
            for (int i = 91; i <= 96; i++) 
            {
                allpossible += static_cast<char>(i);
            } 
            for (int i = 123; i <= 126; i++) 
            {
                allpossible += static_cast<char>(i);
            }
        }
        // get the number of characters to use (used for rand())
        int number_of_possibilities = allpossible.length();
        for (int i = 0; i < length; i++) 
        {
            str += allpossible[rand() % number_of_possibilities];
        }
        return str;
    }

    int		select_rand_weight(int* weight, int n)
    {
		struct Weight
		{
			Weight() : pos(0), value(0) {}
			int pos;
			int value;

            void swap(Weight& rhs)
            {
                int pos = rhs.pos;
                int value = rhs.value;
                rhs.pos = this->pos;
                rhs.value = this->value;
                this->pos = pos;
                this->value = value;
            }
		};
        /*template<>  // ¿¿¿
        void swap<Widget>(Weight& a, Weight& b)
        {
           a.swap(b);
        }*/
        int total_weight = 0;
        Weight* temp = new Weight[n];
        for (int i = 0; i < n; ++i)
        {
            temp[i].pos = i;
            temp[i].value = weight[i];
            total_weight += weight[i];
        }
        //ÈÅÂÒË³Ðò
        for (int i = n - 1; i > 0; --i)
        {
            int pos = get_rand(0, i);
            //swap<Weight>(temp[pos], temp[i]);
            temp[pos].swap(temp[i]);
        }

        int rand_value = get_rand(1, total_weight);
        int sum = 0;
        for (int i = 0; i < n; ++i)
        {
            sum += temp[i].value;
            if (rand_value <= sum)
            {
                return temp[i].pos;
            }
        }
        SAFE_DELETE_ARRAY(temp);
        return 0;
    }

} //namespace algorithm
