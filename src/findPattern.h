
#ifndef _FINDPATTERN_H_
#define _FINDPATTERN_H_

char **findPattern(const char *pattern, const char *filename, int *size);

char **findPatternLines(const char *pattern, const char *filename, int *size);

#endif /* _FINDPATTERN_H_*/