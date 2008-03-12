

#include <stdio.h>

void show_help()
{
	puts("\n\n"
		 "===========================================\n"
		 "  (O)NScripter Script Decompiler\n"
		 "===========================================\n"
		 "\n"
		 "USAGE:\n"
		 "   nscrdec [in_file] [out_file]\n"
		 "\n"
		 "By default, in_file is \"nscript.dat\" and out_file is \"nscript.txt\".\n"
		 "Both in_file and out_file can be \"-\" to specify stdin and/or stdout.\n");
}

int main(int argc, char *argv[])
{
	char *in_file, *out_file;
	FILE *fp_in, *fp_out;
	unsigned char buf[2048];
	int i;
	size_t dw;
	
	in_file = (argc<2) ? "nscript.dat" : argv[1];
	out_file = (argc<3) ? "nscript.txt" : argv[2];
		
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
	
	if (strcmp(out_file,"-"))
		fp_out = fopen(out_file,"wt");
	else
		fp_out = stdout;
	if (!fp_out)
	{
		printf("Cannot open output file %s!!!",out_file);
		return -1;
	}
	
	while(!feof(fp_in))
	{
		dw = fread(buf, 1, 2048, fp_in);
		for (i=0; i<dw; i++)
		{
			buf[i] ^= 0x84;
		}
		fwrite(buf, 1, dw, fp_out);
	}
	
	fclose(fp_in);
	fclose(fp_out);
	return 0;
}