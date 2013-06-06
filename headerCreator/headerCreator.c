/******************************************************************************
*
* Reads an intel HEX file and creates a header file which is "application
* readable" version of the HEX file.
*
* Compile with: 
*  gcc headerCreater.c -o headerCreater
*
* ihsan Kehribar - 2013
******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
/*----------------------------------------------------------------------------*/
static int parseIntelHex(char *hexfile, char* buffer, int *startAddr, int *endAddr);
static int parseHex(FILE *file_pointer, int num_digits);
static int parseUntilColon(FILE *file_pointer);
/*----------------------------------------------------------------------------*/
unsigned char dataBuffer[65536 + 256];
/*----------------------------------------------------------------------------*/
int main(int argc, char **argv) 
{
	int startAddress = 1, endAddress = 0;
	char* fileName = NULL;
	FILE* outputFile;
	int i;

  if(argc == 1)
  {
    printf("What is the file name?\n");
    printf("Usage: headerCreater myProgram.hex\n");
    return 0;
  }
  else if(argc > 2)
  {
    printf("Unneccessary parameters!\n");
    printf("Usage: headerCreater myProgram.hex\n");
    return 0;
  }

	fileName = argv[1];

	memset(dataBuffer, 0xFF, sizeof(dataBuffer));

	parseIntelHex(fileName, dataBuffer, &startAddress, &endAddress);

	outputFile = fopen("programMemory.h","wr");

  fprintf(outputFile,"int startAddress = %d;\n",startAddress);

  fprintf(outputFile,"int endAddress = %d;\n",endAddress);

  fprintf(outputFile, "char* programName = \"%s\";\n",fileName);

	fprintf(outputFile,"unsigned char dataBuffer[%d] = \n{\n    ",endAddress);	

	for(i=0;i<endAddress-1;i++)
	{
		fprintf(outputFile,"0x%X,",dataBuffer[i]);
		if(((i%40)==0) && i!=0)
		{
			fprintf(outputFile,"\n    ");
		}
	}

	fprintf(outputFile,"0x%X\n};\n",dataBuffer[endAddress-1]);

  printf("headerCreater done! See the 'programMemory.h' file. \n");

	return 0;
}
/*----------------------------------------------------------------------------*/
static int parseIntelHex(char *hexfile, char* buffer, int *startAddr, int *endAddr) 
{
  int address, base, d, segment, i, lineLen, sum;
  FILE *input;
  
  input = strcmp(hexfile, "-") == 0 ? stdin : fopen(hexfile, "r");
  if (input == NULL) {
    printf("> Error opening %s: %s\n", hexfile, strerror(errno));
    return 1;
  }
  
  while (parseUntilColon(input) == ':') {
    sum = 0;
    sum += lineLen = parseHex(input, 2);
    base = address = parseHex(input, 4);
    sum += address >> 8;
    sum += address;
    sum += segment = parseHex(input, 2);  /* segment value? */
    if (segment != 0) {   /* ignore lines where this byte is not 0 */
      continue;
    }
    
    for (i = 0; i < lineLen; i++) {
      d = parseHex(input, 2);
      buffer[address++] = d;
      sum += d;
    }
    
    sum += parseHex(input, 2);
    if ((sum & 0xff) != 0) {
      printf("> Warning: Checksum error between address 0x%x and 0x%x\n", base, address);
    }
    
    if(*startAddr > base) {
      *startAddr = base;
    }
    if(*endAddr < address) {
      *endAddr = address;
    }
  }
  
  fclose(input);
  return 0;
}
/*----------------------------------------------------------------------------*/
static int parseUntilColon(FILE *file_pointer) 
{
  int character;
  
  do {
    character = getc(file_pointer);
  } while(character != ':' && character != EOF);
  
  return character;
}
/*----------------------------------------------------------------------------*/
static int parseHex(FILE *file_pointer, int num_digits) 
{
  int iter;
  char temp[9];

  for(iter = 0; iter < num_digits; iter++) {
    temp[iter] = getc(file_pointer);
  }
  temp[iter] = 0;
  
  return strtol(temp, NULL, 16);
}
/*----------------------------------------------------------------------------*/