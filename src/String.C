#include <cstdint>
#include <string>   //for std::string functions
#include <string.h> //for c-string functions (strlen, strcpy, ..)
#include <assert.h>
#include "String.h"

/*
 * String
 *
 * Construct a String object out of a std::string 
 * This constructor would also be called on a c-str
 *
 * Modifies: str and length data members
 */
String::String(std::string str)
{
   //Prevent calling constructor with NULL string
   assert(str.length() > 0);

   //TODO
   //Dynamically allocate an array of chars just large enough to 
   //hold the chars of the std::string.
   this->str = new char[str.length()];
   //Don't store the NULL or allocate space for it.
   //(You don't need the NULL since you are storing the length.)
   //Copy the characters in the std::string (excluding a NULL)
   for (uint64_t i = 0; i < str.length(); i++)
   {
	    if (str[i] > 0)
	    {
		    this->str[i] = str[i];
	    }
	 
   }
   //into your str array.
   //Set length to the size of the array.
   length = str.length();
   
}

/*
 * get_cstr
 *
 * Build and return a c-str from the array of characters.
 */
char * String::get_cstr()
{
   //TODO
   //You need to dynamically allocate space (array of char) for the
   //c-string that you are building
   //That space needs to include space for a NULL
   //Don't forget to add the NULL.
   char * newStr = new char[length + 1];
   for (int i = 0; i <= length; i++)
   {
        newStr[i] = str[i];
   }
   newStr[length] = 0;

   return str; //change this
}

/*
 * get_stdstr
 *
 * Build and return a std::string from the array of characters.
 */
std::string String::get_stdstr()
{
   //TODO
   //You need to declare std::string and append the characters
   //from your str array to it
   
   return std::string(this->str, length); //change this
}

/*
 * get_length
 *
 * return the value of the length data member
 */
int32_t String::get_length()
{
   //TODO
  // return strlen(String::);
  return length;  
}

/*
 * badIndex
 *
 * Returns true if this index into the str array is
 * invalid (negative or greater than array size)
 */
bool String::badIndex(int32_t idx)
{
   //TODO
   if (idx < 0 || idx >= length)
   {
	return true;
   }
   return false; 
}

/*
 * isRepeatingChar
 *
 * Returns true if the characters in the array from
 * startIdx to endIdx are all the character what.
 * For example, if what is ' ' and startIdx is 2 and len
 * is 3 then this function returns true if str[2], str[3],
 * and str[4] are all ' '
 *
 * Three cases are possible:
 * 1) error set to true and returns false if indices are invalid
 *    (starting index and calculated ending index are not with
 *    the string)
 * 2) error set to false and returns false if the characters in
 *    the array at the specified indices are not all the character
 *    what
 * 3) error set to false and returns true if the characters in
 *    the array at the specified indices are all the character 
 *    what
 */
bool String::isRepeatingChar(char what, int32_t startIdx, 
                             int32_t len, bool & error)
{
   //TODO
   //use your badIndex method to check if the
   //starting and ending indices are 
    bool ret = false;
    if (badIndex(startIdx) || badIndex(startIdx + len - 1) || len < 0)
    {
        error = true;
	    ret = false;
    }
    else
    {
	    error = false;
        for (int i = startIdx; i < startIdx + len; i++)
        {
            if (this->str[i] == what)
            {
                
                ret = true;
            }
            else
            {
                
                ret = false;
                
                break;
            }
        }
    }
    return ret;
}

/*
 * convert2Hex
 *
 * Builds and returns a number from the values in the array from 
 * startIdx up to len characters treating those characters as 
 * if they are hex digits. Returns 0 and sets error to true 
 * if the indices are invalid or if the characters are not hex.
 *
 * Valid characters for conversion are:
 *  '0', '1' through '9'
 *  'a', 'b' through 'f'
 *  'A', 'B' through 'F'
 *
 *  Three cases are possible:
 *  1) sets error to false and returns converted number if 
 *     the characters in the specified indices are valid 
 *     hex characters
 *  2) sets error to true and returns 0 if the indices are invalid
 *  3) sets error to true and returns 0 if the indices are 
 *     valid but the characters are not hex
 */
uint32_t String::convert2Hex(int32_t startIdx, int32_t len, bool & error)
{
   //TODO
   //use your badIndex method
   //you can use strtoul for this or you can just write the code to do
   //it yourself. Doing it yourself also makes it easier to simultaneously
   //check for errors.  If the array contains '2''a''f' then you 
   //would return (2 << 8) + (10 << 4) + 15.  You can build it 
   //by setting a result variable and adding to it each time 
   //through the loop.
   //First time through loop: result = 0x2
   //Second time through loop: result = 0x2a
   //Third time through loop: result = 0x2af
   
   uint32_t answer = 0;
   if (badIndex(startIdx) || badIndex(startIdx + len - 1) || len < 0)
   {
    error = true;
	return 0;
   }
   else
   {
	error = false;
   for (int i = startIdx; i < startIdx + len; i++)
    {
        if ((this->str[i] >= 48 && this->str[i] <= 57))
	    {
        answer += (this->str[i] - 48);
	    }
        else if (this->str[i] >= 65 && this->str[i] <= 70)
        {
        answer += (this->str[i] - 55);
        }
        else if (this->str[i] >= 97 && this->str[i] <= 102)
        {
            answer += (this->str[i] - 87);
        }
		else 
		{
			answer = 0;
			error = true;
			break;
		}
        if (i != (startIdx + len) - 1)
        {
            answer = answer << 4;
        }
   }
   return answer;
}
}
/* 
 * isChar
 * Returns true if str[idx] is equal to what 
 *
 * Three cases are possible:
 * 1) set error to true and return false if idx is invalid
 * 2) set error to false and return true if str[idx] is what
 * 3) set error to false and return false if str[idx] is not what
 */
bool String::isChar(char what, int32_t idx, bool & error)
{
   //TODO
   if (idx < 0 || idx >= length)
   {
        error = true;
        return false;
   }
   else
   {
        error = false;
        if (this->str[idx] == what)
        {
            return true;
        }
        else
        {
            return false;
        }
   }
} 

/* 
 * isHex
 * Returns true if the sequence of len characters starting at index 
 * startIdx are hex 
 *
 * Three cases are possible:
 * 1) set error to true and return false if starting or 
 *    calculated ending index is invalid
 * 2) set error to false and return true if sequence of characters 
 *    starting at startIdx is hex
 *    Valid hex characters are:
 *    '0', '1' through '9'
 *    'a', 'b' through 'f'
 *    'A', 'B' through 'F'
 * 
 * 3) set error to false and return false otherwise
 */
bool String::isHex(int32_t startIdx, int len, bool & error)
{
   //TODO
    bool ret;
    if (badIndex(startIdx) || badIndex(startIdx + len - 1) || len < 0)
    {
        error = true;
        ret = false;
    }
    else
    {
        error = false;
        for (int i = startIdx; i < startIdx + len; i++)
        {
            if ((this->str[i] >= '0' && this->str[i] <= '9') || 
            (this->str[i] >= 'a' && this->str[i] <= 'f') ||  
            (this->str[i] >= 'A' && this->str[i] <= 'F'))
            {
                ret = true;
            }
            else
            {
                ret = false;
                break;
            }
        }
    }
   
   return ret;
} 

/*
 * isSubString
 *
 * Returns true if the c-str subStr is in the str array starting
 * at index startIdx 
 *
 * Three cases are possible:
 * 1) starting and/or calculated ending index are invalid: 
 *    set error to true and return false
 * 2) indices are valid and subStr is in str array: set error to 
 *    false and return true
 * 3) indices are valid and subStr is not in str array: set 
 *    error to false and return false
 */
bool String::isSubString(const char * subStr, int32_t startIdx, bool & error)
{
   //TODO
    bool ret;
    String newStr(subStr);
    int endIdx = startIdx + newStr.get_length() - 1;
    if (badIndex(startIdx) || badIndex(endIdx))
    {
        error = true;
        return false;
    }
    error = false;
    int i = 0;
    while (subStr[i] > 0)
    {
        if (this->str[startIdx] == subStr[i])
        {
            ret = true;
        }
        else
        {
            ret = false;
            
            break;
        }
        startIdx++;
        i++;
    }
    return ret;
}


/*
 * isSubString
 *
 * Returns true if the std::string subStr is in the str array 
 * starting at index startIdx 
 *
 * Three cases are possible:
 * 1) starting and/or calculated ending index are invalid: 
 *    set error to true and return false
 * 2) indices are valid and subStr is in str array: set error to 
 *    false and return true
 * 3) indices are valid and subStr is not in str array: set 
 *    error to false and return false
 */
bool String::isSubString(std::string subStr, int32_t startIdx, 
                         bool & error)
{  
   //TODO
    bool ret;
    String newStr(subStr);
    int endIdx = startIdx + newStr.get_length() - 1;
    if (badIndex(startIdx) || badIndex(endIdx))
    {
        error = true;
        return false;  
    }
    error = false;
    int i = 0;
    while (subStr[i] > 0)
    {
        if (this->str[startIdx] == subStr[i])
        {
            ret = true;
        }
        else
        {
            ret = false;
            
            break;
        }
        startIdx++;
        i++;
    }
    return ret;
}

 