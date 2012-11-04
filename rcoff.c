//Assignment 1
//Question 2
//EE 675 Group 10
//Authors : Prashant Rupapara & Shreyas Shyamsunder

#include <stdio.h>
#include <string.h>
#include <time.h>


// It gives positional weight used in conversion from bytes to long integer
long int weight(unsigned int position)
{
	long int result = 1;
	int i;
	for(i=0;i<position;i++)
	{
		result = result*256;
	}
	return result;
}

// It prints string entries from COFF file ... 8 byte name till null character encountered 
void print_string(unsigned char *str)
{
	printf("%s",str);
}

// To convers any number (max. 4 as more than that not required) separate bytes in to half words, words
long int concate_bytes_to_int(int byte_num, unsigned char *number)
{
	long int result=0;
	int i;
	//printf("%ld\n",result);
	for(i=0;i<byte_num;i++)
	{
		result = result + weight(i)* *number; //i.e. half word = (byte0 * 256^0) + (byte1 * 256^1)
		number++;
	}
	return result;
}

// This function takes file coff version, fp_reloc_entries(base address of relocation entries) and file read buffer(rec)
// and prints appropriate content keeping in mind dependency of data format on coff version. 
void print_reloc_entry(int COFF_ver, long int fp_reloc_entries, unsigned char *rec)
{
	
	printf("\nVirtual Address : %.8lx",concate_bytes_to_int(4,&rec[fp_reloc_entries+0]));
	if(COFF_ver == 0)
	{
		printf("\nSymbol table index : %.8lx",concate_bytes_to_int(2,&rec[fp_reloc_entries+4]));	
		printf("\nRelocation type : %.8lx",concate_bytes_to_int(2,&rec[fp_reloc_entries+8]));
	}
	else if(COFF_ver == 1 || COFF_ver == 2)
	{
		printf("\nSymbol table index : %.8lx",concate_bytes_to_int(4,&rec[fp_reloc_entries+4]));	
		printf("\nRelocation type : %.8lx",concate_bytes_to_int(2,&rec[fp_reloc_entries+10]));
	}
}

// This function prints section header and its raw data content keeping in mind dependency of data format on coff version. 
void print_section_header(int offset_optional, long int offset_section, int COFF_ver, unsigned char *rec)
{
	long int fp_raw_data,fp_reloc_entries,fp_line_number_entries,fp_raw_data_size,i,num_relocation_entry,num_line_entry;
	
	fp_raw_data = concate_bytes_to_int(4,&rec[offset_section+offset_optional+20]); // pointer to raw data
	fp_reloc_entries = concate_bytes_to_int(4,&rec[offset_section+offset_optional+24]); // pointer to relocation entry
	fp_line_number_entries = concate_bytes_to_int(4,&rec[offset_section+offset_optional+28]); // pointer to line number
	fp_raw_data_size = concate_bytes_to_int(4,&rec[offset_section+offset_optional+16]); // data size of raw data 
	
	printf("\nSection header : ");
	//offset_optional takes care of adding other 28 to pointer if optional header is present because it is there in beetween file header and section header.
	print_string(&rec[offset_section+offset_optional+0]); // prints name of section header
	printf("\nSection physical address : %.8lx",concate_bytes_to_int(4,&rec[offset_section+offset_optional+8]));
	printf("\nSection virtual address : %.8lx",concate_bytes_to_int(4,&rec[offset_section+offset_optional+12]));
	printf("\nSection size in words : %.8lx",fp_raw_data_size);
	printf("\nFile pointer to raw data : %.8lx",fp_raw_data);
	printf("\nFile pointer to reloc entries : %.8lx",fp_reloc_entries);
	printf("\nFile pointer to line number entries : %.8lx",fp_line_number_entries);
	if(COFF_ver == 0 || COFF_ver == 1)
	{	
		num_relocation_entry = concate_bytes_to_int(2,&rec[offset_section+offset_optional+32]);
		num_line_entry = concate_bytes_to_int(2,&rec[offset_section+offset_optional+34]);
		printf("\nNumber of relocation entries : %.4lx",num_relocation_entry);
		printf("\nNumber of line number entries : %.4lx",num_line_entry);
		printf("\nSection flags : %.4lx",concate_bytes_to_int(2,&rec[offset_section+offset_optional+36]));
		printf("\nMemory page number : %.3x",rec[offset_section+offset_optional+39]);
	}
	else if(COFF_ver == 2)
	{	
		num_relocation_entry = concate_bytes_to_int(4,&rec[offset_section+offset_optional+32]);
		num_line_entry = concate_bytes_to_int(4,&rec[offset_section+offset_optional+36]);
		printf("\nNumber of relocation entries : %.4lx",num_relocation_entry);
		printf("\nNumber of line number entries : %.4lx",num_line_entry);
		printf("\nSection flags : %.4lx",concate_bytes_to_int(4,&rec[offset_section+offset_optional+40]));
		printf("\nMemory page number : %.3lx",concate_bytes_to_int(2,&rec[offset_section+offset_optional+46]));
	}
	
	printf("\n\nSection raw data : ");
	if(fp_raw_data_size == 0)
	{
		printf("\nNo raw data entries.");
	}
	else
	{
		for(i=0;i<fp_raw_data_size;i++) // prints raw data 
		{
			printf("\n%.8ld -- %.8lx",i,concate_bytes_to_int(4,&rec[fp_raw_data]));
			fp_raw_data += 4;
		}
		printf("\n\nSection relocation entries : ");
		if(num_relocation_entry == 0)
		{
			printf("\nNo Relocation entries.");
		}
		else
		{
			for(i=1;i<=num_relocation_entry;i++)
			{
				printf("\nEntry %ld",i);
				print_reloc_entry(COFF_ver,fp_reloc_entries,rec); // this function prints relocation entry details 
				if(COFF_ver == 0)
				{
					fp_reloc_entries += 10;		//relocation entry block is of 10 bytes for COFF Version 0.
				}
				else if(COFF_ver == 1 || COFF_ver == 2)
				{
					fp_reloc_entries += 12;	//relocation entry block is of 12 bytes for COFF Version 1 and 2.
				}
				printf("\n------------------------------");
			}
		}
	}
	printf("\n\n-- Section ");
	print_string(&rec[offset_section+offset_optional+0]);
	printf(" end --\n");
}	


#define SYM_NAME_CNT 4 // auxiliary table made for .file, .text, .data and .bss that for 4 things.

int main(int argc, char *argv[])
{
	int i=0;
	int j=0;
	unsigned char rec[2000]; //recevie buffer that stores binary file raw content.
	FILE *fp1; // file pointer
	unsigned char COFF_ver; // holds coff version of input file
	 
	char *sym_name[SYM_NAME_CNT] = {".file",".text",".data",".bss"};
	unsigned int sym_name_len[SYM_NAME_CNT] = {5,5,5,4};	
	int aux_table_test;// holds info whether auxiliary table exists or not 

	long int num_sec_header; // number of section header
	long int time_date_stamp; // time and date stamp raw value
	int offset_optional=0;	//Offset due to optional header(if any)
	int offset_section=22;  //Offset of to section header..by doing this we were able to point section header entries
							//by offset_section + number and this number is from section header info given in coff1.pdf 
							//so we can just use 0,1 2 as in table, we dont need to add file adder or previous section header 
							//manually. Makes code vey readble. This method is used when ever required.
	long int num_section_header=0; //number of section header
	long int symbol_table_entry = 0; // number of symbol table entires.
	long int symbol_table_pointer = 0; //symbol table pointer.
	long int offset_string_table = 0; // this info is extracted from file name bytes of symbol if name is greater then 8 bytes
									  // we can use this to find that long name
	long int string_table_pointer = 0;// pointer to string table.
	
	fp1=fopen(argv[--argc],"rb");
	
	//error handling
	if (fp1 == NULL) 
	{
 		fprintf(stderr, "Can't open input file!\n");
    		goto EXIT;
	}
	
	//file reading and storing in buffer namely rec
	for(i=0;!feof(fp1);i++)
	{
		rec[i]=fgetc(fp1);
	}
	
	fclose(fp1);

	// identify coff version	
	if(rec[0]==0x93)
		COFF_ver=0;
	else if(rec[0]==0xC1)
		COFF_ver=1;
	else if(rec[0]==0xC2)
		COFF_ver=2;
	else
	{
		//error handling
		fprintf(stderr, "Input File is not COFF !!\n");
    	goto EXIT;
	}
		
	// initialisation of varios pointers and parameters from data read from binary file 	 
	offset_optional = (int)concate_bytes_to_int(2,&rec[16]);
	num_section_header = concate_bytes_to_int(2,&rec[2]);	
	symbol_table_entry = concate_bytes_to_int(4,&rec[12]);
	symbol_table_pointer = concate_bytes_to_int(4,&rec[8]);
	string_table_pointer = symbol_table_pointer + (symbol_table_entry * 18 );//one symbol entry block is of 18 bytes
	time_date_stamp = concate_bytes_to_int(4,&rec[4]);
	printf("\n");
	printf("+--------------------------------------+\n\r");
	printf("+  EE 675 Group 10                     +\n\r");
	printf("+  Assignment 1                        +\n\r");
	printf("+  COFF Binary Read Utility            +\n\r");
	printf("+--------------------------------------+\n\r");
	printf("\nBytes read : %d",--i);
	printf("\nCOFF Version : %d",COFF_ver);
	printf("\n\nFILE HEADER : ");
	printf("\nCOFF version bytes : %.4x",rec[0]);
	printf("\nNo. of section headers : %.4lx",num_section_header);
	printf("\nTIMESTAMP : %s", ctime(&time_date_stamp));
	printf("Symbol table pointer : %.8lx",symbol_table_pointer);
	printf("\nSymbol table entries : %.8lx",symbol_table_entry);
	printf("\nFlags : %.4lx",concate_bytes_to_int(2,&rec[18]));
	printf("\nTarget ID : %.4lx",concate_bytes_to_int(2,&rec[20]));
	printf("\n\nOPTIONAL FILE HEADER : ");
	if(offset_optional == 0)
	{
		printf("\nNo optional header.");
	}
	else
	{
		printf("\nOptional header size : %.4x",offset_optional);
		printf("\nMagic number : %.4lx",concate_bytes_to_int(2,&rec[22]));
		printf("\nVersion stamp : %.4lx",concate_bytes_to_int(2,&rec[24]));
		printf("\nSize of executable code(in words) : %.8lx",concate_bytes_to_int(4,&rec[26]));
		printf("\nSize of initialised data(in words) : %.8lx",concate_bytes_to_int(4,&rec[30]));
		printf("\nSize of uninitialised data(in words) : %.8lx",concate_bytes_to_int(4,&rec[34]));
		printf("\nEntry point : %.8lx",concate_bytes_to_int(4,&rec[38]));
		printf("\nBeginning address of executable code : %.8lx",concate_bytes_to_int(4,&rec[42]));
		printf("\nBeginning address of initialised data : %.8lx",concate_bytes_to_int(4,&rec[46]));
	}		
	
	printf("\n\n");
	
	while(num_section_header)
	{
		print_section_header(offset_optional,offset_section,COFF_ver,rec);
		num_section_header--;
		if(COFF_ver == 0 || COFF_ver == 1)
		{
			offset_section += 40; //section header block is of 40 bytes for COFF Version 0 and Version 1. 
		}
		else if(COFF_ver == 2)
		{
			offset_section += 48; //section header block is of 48 bytes for COFF Version 2.
		}
	}
	
	printf("\n\nSymbol table entries : ");
	if(symbol_table_entry == 0)
	{
		printf("\nNo symbol entries.");
	}
	else
	{	
		for(i=0;i<symbol_table_entry;i++)
		{
			printf("\nSymbol table index : %d",i);
			printf("\nSymbol name : ");
			//if symbol name's first 4 bytes are NULL then bytes 4 to 7 contains offset address to string table.
			if(rec[symbol_table_pointer] == 0 && rec[symbol_table_pointer+1] == 0 && rec[symbol_table_pointer+2] == 0 && rec[symbol_table_pointer+3] == 0  )
			{
				// names longer then 8 bytes are taken care of here			
				offset_string_table = concate_bytes_to_int(4,&rec[symbol_table_pointer+4]);
				print_string(&rec[string_table_pointer + offset_string_table]);
			}
			else
			{
				// names less then or equal to 8 bytes are taken care of here	
				print_string(&rec[symbol_table_pointer]);
			}		
			printf("\nSymbol value : %.8lx",concate_bytes_to_int(4,&rec[symbol_table_pointer+8]));
			printf("\nSymbol section number : %.4lx", concate_bytes_to_int(2,&rec[symbol_table_pointer+12]));
			printf("\nSymbol type : %.4lx", concate_bytes_to_int(2,&rec[symbol_table_pointer+14]));
			printf("\nSymbol class : %.4x", rec[symbol_table_pointer+16]);
			aux_table_test = rec[symbol_table_pointer+17];		
			printf("\nSymbol aux entry : %.4x",aux_table_test);
			symbol_table_pointer += 18;
			printf("\n------------------------------");
			// finds if there is auxiliary table and also which kind of auxiliary table and the prints the content.
			if(aux_table_test)
			{
				for(j=0;j<SYM_NAME_CNT;j++)
				{	
					//checks which kind of symbol is there
					// write now we developed for .text,.data,.bss and .file other can be added further as code is written such that it is up gradation is very easy.
					if(!memcmp(&rec[symbol_table_pointer-18],sym_name[j],sym_name_len[j]))
					{
						break;
					}
				}
				if(j == 0)// file name auxiliary table
				{
					i++;
					printf("\nSymbol table index : %d",i);				
					printf("\nAuxillary table of  %s",sym_name[j]);
					printf("\nFilename : ");
					print_string(&rec[symbol_table_pointer+0]);
					symbol_table_pointer += 18;
					printf("\n------------------------------");
				}
				else if(j == 1 || j == 2 || j == 3) //.text,.bss,.data auxiliary table
				{
					i++;
					printf("\nSymbol table index : %d",i);					
					printf("\nAuxillary table of  %s",sym_name[j]);
					printf("\nSection length : %.8lx", concate_bytes_to_int(4,&rec[symbol_table_pointer+0]));
					printf("\nNo of relocation entries : %.6lx", concate_bytes_to_int(3,&rec[symbol_table_pointer+4]));
					printf("\nNo of line number entries : %.4lx",concate_bytes_to_int(2,&rec[symbol_table_pointer+7]));
					symbol_table_pointer += 18;
					printf("\n------------------------------");
				}
			}						
		}
	}
	EXIT:
	printf("\n\n");
	return(0);
}
