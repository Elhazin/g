#include "../includes/minishell.h"
#include "../pipex/pipex.h"

void ft_dup(int *fd, t_cmd *str, int size)
{
	if (str->files_head)
	{
		while (str->files_head)
		{
			if (str->files_head->type == 'i')
			{
				str->f->fd_input = open(str->files_head->file, O_RDWR);
				dup2(str->f->fd_input, 0);
				close(str->f->fd_input);
			}
			else if (str->files_head->type == 'o')
			{
				str->f->fd_output = open(str->files_head->file, O_CREAT | O_TRUNC | O_RDWR, 0777);
				dup2(str->f->fd_output, 1);
				close(str->f->fd_output);
			}
			if (str->files_head->type == 'H')
			{
				
			}
			str->files_head = str->files_head->next;
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
	int s = 0;
	int **fd = (int **)malloc(sizeof(int *) * ft_lstsize(my_arg));
	while(s < ft_lstsize(my_arg))
	{
		fd[s++] = malloc(sizeof(int) * 2);
	}
	int c = 0;
	int x = 0;
	int i;
	i = 0; 
	int pid;
	ft_pipe(ft_lstsize(my_arg), fd);
	pid = fork();
	if (pid == 0)
	{
		ft_dup(fd[i], &my_arg->cmd_struct, ft_lstsize(my_arg));
		char *cmd = is_valid(my_arg->cmd_struct.cmd[0], my_arg->env);
		if (cmd  == NULL)
		{
			write(2, "minishell: command not found", 29);
			return ;
		}
		execve(cmd, my_arg->cmd_struct.cmd, NULL);	
	}
	my_arg = my_arg->next;
	while (my_arg)
	{
		if (fork() == 0)
		{
			ft_dup(fd[i], &my_arg->cmd_struct, ft_lstsize(my_arg));
			while (x)
			{
				close(fd[i - x][0]);
				close(fd[i - x][1]);
				x--;
			}
			char *cmd = is_valid(my_arg->cmd_struct.cmd[0], my_arg->env);
		if (cmd  == NULL)
		{
			write(2, "minishell: command not found", 29);
			return ;
		}
			execve(cmd, my_arg->cmd_struct.cmd, NULL);	
		}
		i++;
		my_arg = my_arg->next;
	}
}
