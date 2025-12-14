/**
 * @file knuth_hash.cpp
 * @brief Knuth multiplicative hash algorithm for string to uint64 conversion
 *
 * This implementation provides a consistent hash value for strings across
 * C++ and C# platforms. Useful for scenarios where string-based keys need
 * to be converted to integer primary keys (e.g., file paths as database PKs).
 *
 * Key features:
 * - Uses Knuth's multiplicative hash constants
 * - Produces 64-bit unsigned integer hash values
 * - Compatible with the C# implementation (see knuth_hash.cs)
 *
 * Note: This example uses MFC's CString. For standard C++, see the README
 * for std::string or std::wstring alternatives.
 */

#include <locale>
#include <atlstr.h>

using namespace std;

/**
 * Calculate Knuth multiplicative hash for a string
 * @param read Input string (CString)
 * @return 64-bit unsigned hash value
 *
 * Algorithm:
 * - Initial value: 3074457345618258791 (large prime)
 * - For each character: hash = (hash + char) * 3074457345618258799
 */
UINT64 CalculateHash(CString read)
{
	UINT64 hashedValue = 3074457345618258791ull;
	for (int i = 0; i < read.GetLength(); i++)
	{
		hashedValue += read[i];
		hashedValue *= 3074457345618258799ull;
	}
	return hashedValue;
}

int _tmain(int argc, _TCHAR* argv[])
{
	// Set locale for proper Unicode handling
	locale::global(locale(""));

	// Example: Hash a file path (case-insensitive)
	CString str = R"(C:\Users\user\Desktop\test_file.png)";
	str.MakeLower();

	// Display the string and its hash value
	printf("%ws\n", str.GetBuffer());
	printf("%llu\n", CalculateHash(str));

	return 0;
}