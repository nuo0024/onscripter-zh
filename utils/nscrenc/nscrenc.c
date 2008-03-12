
#include <stdio.h>
#include <string.h>

void show_help()
{
	puts("\n\n"
		 "===========================================\n"
		 "  (O)NScripter Script Compiler\n"
		 "===========================================\n"
		 "\n"
		 "USAGE:\n"
		 "   nscrenc [in_dir] [out_file]\n"
		 "\n"
		 "By default, in_dir is the current directory and out_file is \"nscript.dat\".\n"
		 "out_file can be \"-\" to specify stdout.\n");
}

int main(int argc, char *argv[])
{
	char *in_dir, *out_file;
	FILE *fp_in, *fp_out;
	char in_file[16];
	unsigned char buf[2048];
	int i;
	size_t dw;
	int digs;
	
	in_dir = (argc<2) ? "" : argv[1];
	out_file = (argc<3) ? "nscript.dat" : argv[2];
		
/*
	if (strcmp(in_file,"-"))
		fp_in = fopen(in_file,"rb");
	else
		fp_in = stdin;
	if (!fp_in)
	{
		if (argc<2) show_help();
		else printf("Cannot open input file %s!!!",in_file);
		return -1;
	}
*/	
	if (strcmp(out_file,"-"))
		fp_out = fopen(out_file,"wb");
	else
		fp_out = stdout;
	if (!fp_out)
	{
		printf("Cannot open output file %s!!!",out_file);
		return -1;
	}
	
	_chdir(in_dir);
	fp_in = fopen("0.txt","rt");
	if (!fp_in)
	{
		fp_in = fopen("00.txt","rt");
		if (!fp_in)
		{
			fp_in = fopen("000.txt","rt");
			if (!fp_in)
			{
				show_help();
				return -1;
			}
			else digs = 3; 
		}
		else digs = 2;
	}
	else digs = 1;
	
	i=0;
	while (fp_in)
	{
		while(!feof(fp_in))
		{
			dw = fread(buf, 1, 2048, fp_in);
			for (i=0; i<dw; i++)
			{
				buf[i] ^= 0x84;
			}
			fwrite(buf, 1, dw, fp_out);
		}
		fputc('\n',fp_out);
		fclose(fp_in);
		
		switch(digs)
		{
			case 1: sprintf(in_file, "%d", i); break;
			case 2: sprintf(in_file, "%02d", i); break;
			case 3: sprintf(in_file, "%03d", i); break;
			default: in_file[0]=0;
		}
		fp_in = fopen(in_file, "rt");
	}
	
	fclose(fp_out);
	return 0;
}