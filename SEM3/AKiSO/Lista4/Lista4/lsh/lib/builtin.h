#ifndef BUILTIN_H
#define BUILTIN_H

int lsh_cd(char **args);
int lsh_help(char **args);
int lsh_exit(char **args);

extern char *builtin_str[];

extern int (*builtin_func[])(char **);

int lsh_builtin_num();

#endif
