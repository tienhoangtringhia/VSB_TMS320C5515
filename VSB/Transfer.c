#include "Transfer.h"

Uint16 ExportFile( Uint32 pixel, Uint16 *p_buffer_data, int check)
{
	FILE *fp ; 
	Uint32 i ; 
	switch (check)
	{
		case 0:	fp = fopen ( "E:\\HTTTNC\\TinHieuTuKit\\x.bin", "wb" );
			break;
	    case 1: fp = fopen ( "E:\\HTTTNC\\TinHieuTuKit\\vsb.bin", "wb");
			break;		
		case 2: fp = fopen ( "E:\\HTTTNC\\TinHieuTuKit\\x1.bin", "wb");
			break;
		case 3: fp = fopen ( "E:\\HTTTNC\\TinHieuTuKit\\vsb1.bin", "wb");
			break;
	}
	 
	if ( fp == (FILE*)NULL ) 
	{
		printf(" Error : can't open file_in \n" ) ;
		return 1 ;        // Check error 
	}
	for (i = 0; i < pixel; i++)
	{
		fputc(p_buffer_data[i] & 0xFF, fp);  	// Send 8 low bit
		fputc(p_buffer_data[i] >> 8, fp); 		// Gui 8 hight bit
	}
	fclose(fp);
	
	return 0 ; 
}
