// KeyGen.cpp : Defines the entry point for the console application.
//

// #include "stdafx.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
	int retCode = 1;
	if( argc < 2/*Program name + User name*/ )
		printf( "\nPlease give a user name in the command line. For example:\n"
				"\t>KeyGen \"John Smith\"\n" );
	else
	{
		int sum = 0, i;
// бнр гдеяэ мюдн ме INT, ю UNSIGNED INT???
		for( i=0; argv[1][i] != 0; i++ )
			sum += argv[1][i];
		printf( "DA02-%05d-zI3k", sum );

		retCode = 0;
	}
	return retCode;
}

