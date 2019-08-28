#ifndef _PARSE_H
#define _PARSE_H

#include <readline/readline.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* parse_read_line()
{
	char* line = readline("\ncdbg> ");
	return line;
}


char** parse_tokenize_line(char* line)
{
	char** cmd = (char**)malloc(10 * sizeof(char*));
	char* parsed;
	char* separator = " ";
	int i = 0;

	parsed = strtok(line, separator);

	if (parsed == NULL)
	{
		cmd[i++] = line;
		cmd[i] = NULL;
		return cmd;
	}

	while (parsed != NULL)
	{
		if (!(strcmp(parsed, "\n")))
			break;

		if (i <= 10)
		{
			cmd[i++] = parsed;
			parsed = strtok(NULL, separator);
		}
		else
		{
			printf("cmd : a lot of args\n");
			return NULL;
		}
	}

	cmd[i] = NULL;
	return cmd;
}

#endif

