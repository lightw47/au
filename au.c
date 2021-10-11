#include "au.h"

uint32 au_ReverseBytes(uint32 value)
{
	uint32 result;
	uint8 *bytes = &value;
	uint8 *reversed = &result;

	reversed[0] = bytes[3];
	reversed[1] = bytes[2];
	reversed[2] = bytes[1];
	reversed[3] = bytes[0];

	return result;
}

AuHeaderFields au_GetNullFields(void)
{
    AuHeaderFields h_fields;

    h_fields.magic       = 0;
    h_fields.data_offset = 0;
    h_fields.data_size   = 0;
    h_fields.encoding    = 0;
    h_fields.sample_rate = 0;
    h_fields.channels    = 0;

    return h_fields;
}

AuHeaderFields au_GetHeaderFields(char* file_name)
{
    int read_value[5];
    AuHeaderFields h_fields = au_GetNullFields();

    FILE* file_obj = fopen(file_name, "rb");
    if (file_obj == NULL)
        goto au_GetHeaderFields_end;

    fread(read_value, sizeof(uint32), 1, file_obj);
    h_fields.magic = au_ReverseBytes(read_value[0]);

    if (h_fields.magic == AU_MAGIC)
    {
        fread(read_value, sizeof(uint32), 5, file_obj);
        h_fields.data_offset = au_ReverseBytes(read_value[0]);
        h_fields.data_size   = au_ReverseBytes(read_value[1]);
        h_fields.encoding    = au_ReverseBytes(read_value[2]);
        h_fields.sample_rate = au_ReverseBytes(read_value[3]);
        h_fields.channels    = au_ReverseBytes(read_value[4]);
    }

    fclose(file_obj);

    au_GetHeaderFields_end:
    return h_fields;
}

AuFileData au_GetNullData(void)
{
    AuFileData au_data;

    au_data.fields = au_GetNullFields();
    au_data.header = NULL;
    au_data.data   = NULL;

    return au_data;
}

void au_FreeData(AuFileData* au_data)
{
    au_data->fields = au_GetNullFields();

    free(au_data->header);
    au_data->header = NULL;

    free(au_data->data);
    au_data->data = NULL;
}

uint8 au_ReadFile(char* file_name, AuFileData* au_data)
{
    au_FreeData(au_data);
    au_data->fields = au_GetHeaderFields(file_name);
    if (au_data->fields.magic != AU_MAGIC)
        return AU_READ_FAILURE_INVALID_MAGIC;

    uint32 header_size = au_data->fields.data_offset;
    uint32 data_size = au_data->fields.data_size;

    FILE* file_obj = fopen(file_name, "rb");
    if (file_obj == NULL)
        return AU_READ_FAILURE_OPEN_FILE_ERROR;

    au_data->header = (uint8*) malloc(header_size);
    au_data->data   = (uint8*) malloc( data_size );

    fread(au_data->header, 1, header_size, file_obj);
    fread(au_data->data,   1, data_size,   file_obj);

    fclose(file_obj);

    return AU_READ_SUCCESS;
}

uint8 au_WriteFile(char* file_name, AuFileData au_data)
{
    FILE* file_obj = fopen(file_name, "wb");
    if (file_obj == NULL)
        return AU_WRITE_FAILURE_OPEN_FILE_ERROR;

    uint32 header_size = au_data.fields.data_offset;
    uint32 data_size = au_data.fields.data_size;

    fwrite(au_data.header, 1, header_size, file_obj);
    fwrite(au_data.data,   1, data_size,   file_obj);

    fclose(file_obj);

    return AU_WRITE_SUCCESS;
}
