#include "../includes/minishell.h"
#include "../pipex/pipex.h"

void ft_dup(int *fd, t_cmd *str, int size)
{
	if (str->files_head)
	{
		if (str->files_head->type == 'i')
		{
			str->f->fd_input = open(str->files_head->file, O_RDWR);
			dup2(str->f->fd_input, 0);
		}
		else if (str->files_head->type == 'o')
		{
			str->f->fd_output = open(str->files_head->file, O_CREAT | O_TRUNC | O_RDWR, 0777);
			dup2(str->f->fd_output, 1);
		}
	}
	else if (size > 1)
	{
		dup2(fd[1], 1);	
		dup2(fd[0], 0);
		close(fd[1]);
	}

}


void ft_pipe(int size, int **fd)
{
	int i;

	 i = 0;
	 while (i < size)
	 	pipe(fd[i++]);
}
void execute(t_lexer_node *my_arg)
{
	int number_of_pipes;
	int fd[ft_lstsize(my_arg) - 1][2];

	int c = 0;
	int x = 0;
	int i;
	i = 0; 
	int pid;
	ft_pipe(ft_lstsize(my_arg), fd);
	pid = fork();
	if (pid  = 0)
	{
		ft_dup(&fd[i], &my_arg->cmd_struct, ft_lstsize(my_arg));
		char *cmd = is_valid(my_arg->cmd_struct.cmd[0], my_arg->env);
		if (cmd  == NULL)
			return (ft_putstr_fd("minishell: command not found", 2));
		execve(cmd, my_arg->cmd_struct.cmd, NULL);	
	}
	my_arg = my_arg->next;
	while (my_arg)
	{
		if (fork() == 0)
		{
			ft_dup(&fd[i], &my_arg->cmd_struct, ft_lstsize(my_arg));
			while (x)
			{
				close(fd[i - x][0]);
				close(fd[i - x][1]);
				x--;
			}
			char *cmd = is_valid(my_arg->cmd_struct.cmd[0], my_arg->env);
			if (cmd  == NULL)
				return (ft_putstr_fd("minishell: command not found", 2));
			execve(cmd, my_arg->cmd_struct.cmd, NULL);	
		}
		i++;
		my_arg = my_arg->next;
	}
}
