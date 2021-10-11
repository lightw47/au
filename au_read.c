#include "au.h"

int main(int argc, char** argv)
{
	// print error if user don't provide file names
	if (argc == 1)
	{
		printf("Error: provide at least one file name. Example:\n");
		printf("%s file1.au [file2.au ...]\n", argv[0]);
		return 1;
	}

	uint32 i;
	AuHeaderFields fields;

	// for each file name the user provided...
	for (i = 1; i < argc; ++i)
	{
		// read the header fields
		fields = au_GetHeaderFields(argv[i]);

		printf("File: %s\n", argv[i]);

		if (fields.magic != AU_MAGIC)
		{
			printf("  does not exist or is not an au file.\n");
		}
		else
		{
			printf("  data offset = %d\n", fields.data_offset);
			printf("  data size   = %d\n", fields.data_size);
			printf("  encoding    = %d\n", fields.encoding);
			printf("  sample rate = %d\n", fields.sample_rate);
			printf("  channels    = %d\n", fields.channels);
		}

		printf("\n");
	}

	return 0;
}
