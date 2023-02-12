#ifndef EXEC_H
# define EXEC_H
#include "../includes/minishell.h"
char	*is_valid(char *cmd, char **env);
char	**ft_split_with_slash(char *str, char cut);
char	**my_split(char *str, char cut);
int	ft_lstsize(t_lexer_node *lst);
char	*ft_strnstr(const char *str, const char *to_find, size_t len);
#endif