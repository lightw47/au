# au
Little project to handle au audio files

### Contents
* au.h: header file containing macros, structs, enums definitions and function declarations
* au.c: source file containing implementation of functions declared in au.h
* au_reader.c: source file of a program that reads the fields in the header of an au file.
* au_reverse.c: source file of a program that reverses a 8-bit LPCM encoded au file. 

### Instructions:
Compile each component separately:

`gcc -o au_reader au_reader.c au.c`

`gcc -o au_reverse au_reverse.c au.c`

After compiling, call the programs:

`./au_reader file1.au [file2.au ...]`

`./au_reverse input.au [output.au]`

Arguments inside brackets are optional.
