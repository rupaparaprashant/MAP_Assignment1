//Assignment 1 
//Question 2
//EE 675 Group 10
//Authors : Prashant Rupapara & Shreyas Shyamsunder

#include <stdio.h>
#include <time.h>

unsigned char buffer[44]; // holds bytes separated from .text and .data 32 bit hex values.(instruction and data)
int count_buffer=0; // holds current location of buffer array.
unsigned char result_conv[4]; //holds conversion result given by conv_digit_to_number. made global so that array can be easily accesible to all.

// convertes 32 bit number(string) in to 4 bytes in little endian.
//e.g. "50700080" => 4 unsigned chars which are 80,00,70,50 which now can be stored in coff file as bytes.
void conv_digit_to_number(unsigned char *rec_data)
{
	int i=0,j=0;
	unsigned char temp[4];
	for(i=0;i<8;i+=2)
	{
		if(rec_data[i] == 'a' || rec_data[i] == 'b'|| rec_data[i] == 'c' || rec_data[i] == 'd'||rec_data[i] == 'e'||rec_data[i] == 'f')
			rec_data[i] -= 0x27;
		else if (rec_data[i] == 'A' || rec_data[i] == 'B'|| rec_data[i] == 'C' || rec_data[i] == 'D'||rec_data[i] == 'E'||rec_data[i] == 'F')
			rec_data[i] -= 0x7;
		if(rec_data[i+1] == 'a' || rec_data[i+1] == 'b'|| rec_data[i+1] == 'c' || rec_data[i+1] == 'd'||rec_data[i+1] == 'e'||rec_data[i+1] == 'f')
			rec_data[i+1] -= 0x27;
		else if (rec_data[i+1] == 'A' || rec_data[i+1] == 'B'|| rec_data[i+1] == 'C' || rec_data[i+1] == 'D'||rec_data[i+1] == 'E'||rec_data[i+1] == 'F')
			rec_data[i+1] -= 0x7;
	
		temp[i/2] = ((((int)rec_data[i])-0x30)*16)+(((int)rec_data[i+1])-0x30);
	}
	//conversion from big endian to little endian. 
	// If you want to make coff file with big endian format just modify below function i.e. copy temp to result_conv on same index.
	for(i=3;i>=0;i--) 
	{
		result_conv[j]=temp[i];
		j++;
	}
}


int main()
{
	//.text and .data section values given in problem.
	unsigned char prb_data[11][8] = {/*.text*/"50700080","07200000","14200001","08600003","0ae00004","15200002","6a00fff9",/*.data*/"12345678","abcd1234","1a2b3c4d","a1b2c3d4"};
	
	//memory address where .text and .data section should begin.
	unsigned char test_mem_address[8] = "00809800";
	unsigned char test_data_address[8] = "00800000";
	
	// different header are made below as array. The information like data, entry point address, initialised data block address, uninitialised data block address etc.
	// are added later in this arrays.
	 
	//file header which is of 22 bytes 
	unsigned char file_header[22] = {/*0*/0xC2,/*1*/0x00,/*2*/0x02,/*3*/0x00,/*4*/0x00,/*5*/0x00,/*6*/0x00,/*7*/0x00,/*8*/0x86,/*9*/0x00,/*10*/0x00,/*11*/0x00,/*12*/0x00,/*13*/0x00,/*14*/0x00,/*15*/0x00,/*16*/0x1C,/*17*/0x00,/*18*/0x1F,/*19*/0x01,/*20*/0x93,/*21*/0x00};
	
	//optional file header which is of 28 bytes 	
	unsigned char optional_header[28] = {/*0*/0x08,/*1*/0x01,/*2*/0x00,/*3*/0x00,/*4*/0x07,/*5*/0x00,/*6*/0x00,/*7*/0x00,/*8*/0x04,/*9*/0x00,/*10*/0x00,/*11*/0x00,/*12*/0x00,/*13*/0x00,/*14*/0x00,/*15*/0x00,/*16*/0x00,/*17*/0x98,/*18*/0x80,/*19*/0x00,/*20*/0x00,/*21*/0x98,/*22*/0x80,/*23*/0x00,/*24*/0x00,/*25*/0x00,/*26*/0x80,/*27*/0x00};

	//.text section header which is of 48 bytes
	unsigned char section_header_text[48] = {/*0*/0x2E,/*1*/0x74,/*2*/0x65,/*3*/0x78,/*4*/0x74,/*5*/0x00,/*6*/0x00,/*7*/0x00,/*8*/0x00,/*9*/0x98,/*10*/0x80,/*11*/0x00,/*12*/0x00,/*13*/0x98,/*14*/0x80,/*15*/0x00,/*16*/0x07,/*17*/0x00,/*18*/0x00,/*19*/0x00,/*20*/0x92,/*21*/0x00,/*22*/0x00,/*23*/0x00,/*24*/0x00,/*25*/0x00,/*26*/0x00,/*27*/0x00,/*28*/0x00,/*29*/0x00,/*30*/0x00,/*31*/0x00,/*32*/0x00,/*33*/0x00,/*34*/0x00,/*35*/0x00,/*36*/0x00,/*37*/0x00,/*38*/0x00,/*39*/0x00,/*40*/0x20,/*41*/0x00,/*42*/0x00,/*43*/0x00,/*44*/0x00,/*45*/0x00,/*46*/0x00,/*47*/0x00};

	//.data section header which is of 48 bytes
	unsigned char section_header_data[48] = {/*0*/0x2E,/*1*/0x64,/*2*/0x61,/*3*/0x74,/*4*/0x61,/*5*/0x00,/*6*/0x00,/*7*/0x00,/*8*/0x00,/*9*/0x80,/*10*/0x00,/*11*/0x00,/*12*/0x00,/*13*/0x80,/*14*/0x00,/*15*/0x00,/*16*/0x04,/*17*/0x00,/*18*/0x00,/*19*/0x00,/*20*/0xAE,/*21*/0x00,/*22*/0x00,/*23*/0x00,/*24*/0x00,/*25*/0x00,/*26*/0x00,/*27*/0x00,/*28*/0x00,/*29*/0x00,/*30*/0x00,/*31*/0x00,/*32*/0x00,/*33*/0x00,/*34*/0x00,/*35*/0x00,/*36*/0x00,/*37*/0x00,/*38*/0x00,/*39*/0x00,/*40*/0x40,/*41*/0x00,/*42*/0x00,/*43*/0x00,/*44*/0x00,/*45*/0x00,/*46*/0x00,/*47*/0x00};
	
	
	long int res;// holds temporary value while converting 32 bit integer to 4 separate bytes.
	
	int i=0,j=3,k=0;
    unsigned char time_array[4]; // stores time and date stamp(4 bytes separated from 32 bit long info got from computer)
	time_t rawtime;

	FILE *fp1; //file pointer
	
	fp1=fopen("Assignment1_part2.out","wb");
	
	
  	time(&rawtime);// gets current time from computer clock.
  	// conversion of 32 bit number into 4 separate bytes as need to store in coff file.
  	res = rawtime;
  	while(res>0)
  	{
  		time_array[i] = res%256;
  		res = res/256;
  		i++;
  	}
  	for(i=0;i<4;i++)
  	{
  		
  		file_header[i+4]=time_array[i];
  	}	
  	
  	// putting information from problem data given in to various header and raw data part.
  	conv_digit_to_number(test_mem_address);
  	for(i=0;i<4;i++)
  	{
  		optional_header[i+16] = result_conv[i];
  		optional_header[i+20] = result_conv[i];
  		section_header_text[i+8] = result_conv[i];
  		section_header_text[i+12] = result_conv[i];
  	}
  	conv_digit_to_number(test_data_address);
  	for(i=0;i<4;i++)
  	{
  		optional_header[i+24] = result_conv[i];
  		section_header_data[i+8] = result_conv[i];
  		section_header_data[i+12] = result_conv[i];
  	}
	
  	//writes in to binary file to make coff file.
  	
	for(i=0;i<22;i++)
	{
		//printf("%x\t",file_header[i]);
		fprintf(fp1,"%c",file_header[i]);
	}
	for(i=0;i<28;i++)
	{
		fprintf(fp1,"%c",optional_header[i]);
	}
	for(i=0;i<48;i++)
	{
		fprintf(fp1,"%c",section_header_text[i]);
	}
	for(i=0;i<48;i++)
	{
		fprintf(fp1,"%c",section_header_data[i]);
	}
	for(i=0;i<12;i++)
	{
		conv_digit_to_number(prb_data[i]);
		for(j=0;j<4;j++)
		{
			buffer[count_buffer]=result_conv[j];
			count_buffer++;
		}		
	}
	for(i=0;i<44;i++)
	{
		fprintf(fp1,"%c",(int)buffer[i]);
	}	
	fclose(fp1);
	printf("\nSuccessfully created COFF version 2 binary file \"Assignment1_part2.out\" in current directory.\n\n");
	return(0);
}
