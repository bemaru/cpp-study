/**
 * @file knuth_hash.cs
 * @brief C# implementation of Knuth multiplicative hash algorithm
 *
 * This implementation provides identical hash values to the C++ version
 * (knuth_hash.cpp) for the same input strings. This is useful for
 * cross-platform scenarios where both C++ and C# code need to generate
 * consistent hash values.
 *
 * Use case: Converting file paths to integer primary keys in a database
 * that is accessed by both C++ and C# applications.
 */

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace KnuthHashExample
{
    class Program
    {
        /// <summary>
        /// Calculate Knuth multiplicative hash for a string
        /// </summary>
        /// <param name="read">Input string</param>
        /// <returns>64-bit unsigned hash value</returns>
        /// <remarks>
        /// Algorithm:
        /// - Initial value: 3074457345618258791 (large prime)
        /// - For each character: hash = (hash + char) * 3074457345618258799
        /// </remarks>
        static UInt64 CalculateHash(string read)
        {
            UInt64 hashedValue = 3074457345618258791ul;
            for (int i = 0; i < read.Length; i++)
            {
                hashedValue += read[i];
                hashedValue *= 3074457345618258799ul;
            }
            return hashedValue;
        }

        static void Main(string[] args)
        {
            // Example: Hash a file path (case-insensitive)
            string str = @"C:\Users\user\Desktop\test_file.png";
            str = str.ToLower();

            Console.WriteLine(str);
            Console.WriteLine(CalculateHash(str));
        }
    }
}
