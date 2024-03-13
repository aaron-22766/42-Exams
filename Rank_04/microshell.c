#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

void	ft_puterr(const char *str)
{
	while (str && *str)
		write(STDERR_FILENO, str++, 1);
}

int	ft_error(const char *msg, const char *context)
{
	ft_puterr("error: ");
	ft_puterr(msg);
	ft_puterr(context);
	ft_puterr("\n");
	return (1);
}

int	ft_cd(char **argv, int len)
{
	if (len != 2)
		return (ft_error("cd: bad arguments", NULL));
	if (chdir(argv[1]) == -1)
		return (ft_error("cd: cannot change directory to ", argv[1]));
	return (0);
}

void	ft_dup(int from, int to, int fd[2], int has_pipe)
{
	if (!has_pipe)
		return ;
	if (dup2(from, to) == -1)
	{
		close(fd[0]);
		close(fd[1]);
	}
	else if (close(fd[0]) == -1)
		close(fd[1]);
	else if (close(fd[1]) != -1)
		return ;
	exit(ft_error("fatal", NULL));
}

int	ft_exec(char **argv, int len, char **envp)
{
	pid_t	pid;
	int		fd[2];
	int		status;
	int		has_pipe;

	has_pipe = (argv[len] && !strcmp(argv[len], "|"));
	if (has_pipe && pipe(fd) == -1)
		exit(ft_error("fatal", NULL));
	pid = fork();
	if (pid == 0)
	{
		ft_dup(fd[1], 1, fd, has_pipe);
		argv[len] = NULL;
		execve(argv[0], argv, envp);
		exit(ft_error("cannot execute ", argv[0]));
	}
	waitpid(pid, &status, WUNTRACED);
	ft_dup(fd[0], 0, fd, has_pipe);
	return WIFEXITED(status) && WEXITSTATUS(status);
}

int	main(int argc, char **argv, char **envp)
{
	int	status;
	int	i;

	if (argc < 2)
		return (0);
	status = 0;
	i = 0;
	while (argv[i] && argv[++i])
	{
		argv += i;
		i = 0;
		while (argv[i] && strcmp(argv[i], "|") && strcmp(argv[i], ";"))
			i++;
		if (!strcmp(argv[0], "cd"))
			status = ft_cd(argv, i);
		else
			status = ft_exec(argv, i, envp);
	}
	return (status);
}
