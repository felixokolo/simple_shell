#include "main.h"

/**
 * shell - shell function
 * @argv: argument velocity
 * @env: environment
 *
 * Return: 0
 */
int shell(char **argv, char **env)
{
	char *av = NULL, *arg[1024], *arg1[1024];
	size_t len = 0, size = 0;
	int err = 0, notPipe = 0, found = 0;

	do {
		if (isatty(fileno(stdin)))
		{
			notPipe = 1;
			printf("%s", SHELL_STR);
		}
		found = 0;
		size = getline(&av, &len, stdin);
		if ((int) size == EOF)
			break;
		*(av + size) = '\0';
		str_split(arg, av, "'\n'", "' '");
		if (!check_abs(arg[0]))
		{
			if (find_env_var(arg1, env, "PATH") >= 0)
			{
				err = find_file(arg1, arg[0]);
				if (err != -1)
				found = 1;
				else
				printf("%s: command not found\n", arg[0]);
			}
		}
		else
		found = 1;
		if (!found)
		continue;
		err = execute(argv[0], arg);
		if (err == -1)
			break;
	} while (notPipe || (int) size != EOF);
	/*free2(arg);*/
	free2(arg1);
	free(av);
	return (0);
}


/**
 * str_split - spliting strings
 *
 * @arg: argument
 * @av: array of arguments
 * @delmt: delimiter
 * @delmt2: delimiter
 *
 * Return: 1
 */
int str_split(char **arg, char *av, char *delmt, char *delmt2)
{
	char *token, *subtoken, *svptr, *svptr2, *str, *str2;
	int n = 0;

	token = strtok_r(av, delmt, &svptr);
	while (token != NULL)
	{
		for (str2 = token; ; str2 = NULL)
		{
			subtoken = strtok_r(str2, delmt2, &svptr2);
			if (subtoken == NULL)
				break;
			str = malloc(sizeof(char) * strlen(subtoken));
			strcpy(str, subtoken);
			*(arg + n) = str;
			n++;
		}

		token = strtok_r(NULL, delmt, &svptr);
		if (token == NULL)
			break;
		str = malloc(sizeof(char) * strlen("next") + 1);
		strcpy(str, "next");
		*(arg + n) = str;
		n++;
	}
	*(arg + n) = NULL;

	n = 0;
	while (*(arg + n) != NULL)
	{
		n++;
	}
	return (1);
}

/**
 * find_env_var - function to find env variable
 *
 * @arg: argument
 * @env: environment
 * @var: variable
 *
 * Return: -1
 */

int find_env_var(char **arg, char **env, char *var)
{

	char *av;
	int n = 0;

	while (env[n] != NULL)
	{
		av = malloc(strlen(env[n]));
		strcpy(av, env[n]);
		str_split(arg, av, "'\0'", "' =:'");
		/*free(av);*/
		if (strcmp(arg[0], var) == 0)
		{
			return (n);
		}
		n++;
	}
	return (-1);

}

/**
 * check_abs - function to check abs
 *
 * @c: value pointer
 * Return: 1 otherwise 0
 */

int check_abs(char *c)
{
	if (*c == '/')
	return (1);
	return (0);
}

/**
 * find_file - function to find file
 *
 * @arg: argument
 * @f: value
 * write it to the area pointed to by the buf argument
 *
 * Return: -1
 */

int find_file(char **arg, char *f)
{
	int n = 1;
	struct stat st;
	char *file_path = malloc(1024);

	while (arg[n] != NULL)
	{
		strcpy(file_path, arg[n]);
		strcat(file_path, "/");
		strcat(file_path, f);
		if  (stat(file_path, &st) == 0)
		{
			strcpy(f, file_path);
			return (n);
		}
		n++;
	}
	return (-1);
}
