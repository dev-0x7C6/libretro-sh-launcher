/*You are free to use, copy, and distribute any of the code on this web site, 
 * whether modified by you or not. You need not give attribution. This includes 
 * the algorithms (some of which appear in Hacker's Delight), the Hacker's Assistant, 
 * and any code submitted by readers. Submitters implicitly agree to this. */
 
 /*http://www.hackersdelight.org/permissions.htm*/
 /*http://www.hackersdelight.org/hdcodetxt/crc.c.txt*/

// ----------------------------- crc32b --------------------------------

/* This is the basic CRC-32 calculation with some optimization but no
table lookup. The the byte reversal is avoided by shifting the crc reg
right instead of left and by using a reversed 32-bit word to represent
the polynomial.
   When compiled to Cyclops with GCC, this function executes in 8 + 72n
instructions, where n is the number of bytes in the input message. It
should be doable in 4 + 61n instructions.
   If the inner loop is strung out (approx. 5*8 = 40 instructions),
it would take about 6 + 46n instructions. */


/*https://stackoverflow.com/questions/21001659/crc32-algorithm-implementation-in-c-without-a-look-up-table-and-with-a-public-li*/

#ifndef __CRC_H__
#define __CRC_H__

class crc
{
	public:
		crc();
		~crc();
		
	protected:
		static unsigned int crc32b(const char *message);
};

#endif /* __CRC_H__ */
