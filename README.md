# SOPETP1
First project for FEUP MIEIC's curricular unit, SOPE.
The purpose is to write a UNIX command line application similar to `grep` called `simgrep`, meaning "simplified grep". Basically, it allows the user to find a pattern in a file or directory (or even the input stream), allowing him to specify a restricted number of options. In general, the lines where the pattern was found are printed in the terminal.

## Usage

1. Compile using `make`.
2. Run with the following command:

```
$ simgrep [options] pattern [file/dir]
```

Where options can be one of the following:
- **-i** ignores the letter case (upper or lower).
- **-l** shows only the file names.
- **-n** also shows the line numbers.
- -**c** shows how many lines the pattern is in.
- **-w** the pattern the user is looking for should form a complete word.
- **-r** the pattern is looked for in every file under the tree of the specified directory.

If no file or directory is specified, the pattern in looked for in the input stream, meaning the input text has to be sent through the terminal.

More specifications can be found [here](SOPETP1/enunciado.pdf) (in portuguese).
