#include "au.h"

#include <string.h>

#define FILE_NAME_MAX_SIZE 256

int main(int argc, char** argv)
{
    // input and output file names
    char if_name[FILE_NAME_MAX_SIZE];
    char of_name[FILE_NAME_MAX_SIZE];

    // print error if user don't provide an input file
    if (argc == 1)
	{
		printf("Error: provide one input file and an optional output file. Example:\n");
		printf("%s input.au [output.au]\n", argv[0]);
		return 1;
	}

    // read input file name from argument
    strcpy(if_name, argv[1]);

    if (argc == 2)
    {
        // if user provided only an input file, output file name is the same as
        // input, preceded by "out_"
        strcpy(of_name, "out_");
        strcat(of_name, if_name);
    }
    else if (argc == 3)
    {
        // if user provided an output file name, read output file name from argument
        strcpy(of_name, argv[2]);
    }

    // initialize input and output data
    AuFileData i_data = au_GetNullData();
    AuFileData o_data = au_GetNullData();

    // read the input file, if the program can't read it, print an error message
    uint8 return_code = au_ReadFile(if_name, &i_data);
    if ( return_code != AU_READ_SUCCESS )
    {
        printf("Error: could not open the file %s. Error code:\n", if_name);
        switch (return_code)
        {
        case AU_READ_FAILURE_INVALID_MAGIC:
            printf("AU_READ_FAILURE_INVALID_MAGIC\n");
            break;
        case AU_READ_FAILURE_OPEN_FILE_ERROR:
            printf("AU_READ_FAILURE_OPEN_FILE_ERROR\n");
        default:
            printf("AU_UNKNOWN_ERROR\n");
        }
        return return_code;
    }

    // check if the file encoding is 8-bit linear PCM, if it is not, print an error message
    if (i_data.fields.encoding != AU_8BIT_LPCM)
    {
        printf("Error: file encoding is not 8-bit linear PCM\n");
        return -1;
    }

    // copy the header fields from input to output data
    // (not really necessary here, but can be useful)
    o_data.fields = i_data.fields;

    // allocate memory for the output data
    o_data.header = (uint8*) malloc(i_data.fields.data_offset);
    o_data.data   = (uint8*) malloc(i_data.fields.data_size);

    // copy the header bytes from input to output data
    memcpy(o_data.header, i_data.header, i_data.fields.data_offset);

    // copy bytes from input data to output data in reverse
    uint32 i;
    uint32 size = i_data.fields.data_size;
    for (i = 0; i < size; ++i)
        o_data.data[i] = i_data.data[size - 1 - i];

    // write the output data to the file, if the program can't write, print an error message
    return_code = au_WriteFile(of_name, o_data);
    if (return_code != AU_WRITE_SUCCESS)
    {
        printf("Error: could not open the file %s. Error code:\n", of_name);
        switch (return_code)
        {
        case AU_WRITE_FAILURE_OPEN_FILE_ERROR:
            printf("AU_WRITE_FAILURE_OPEN_FILE_ERROR\n");
        default:
            printf("AU_UNKNOWN_ERROR\n");
        }
        return return_code;
    }

    au_FreeData(&i_data);
    au_FreeData(&o_data);

    return 0;
}
