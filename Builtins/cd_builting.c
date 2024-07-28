#include "../Includes/minishell.h"

/*
To help you recode the cd function for your mini shell,
let's break down the requirements and steps involved. 
The cd function typically involves changing the current working directory, 
updating environment variables (PWD and OLDPWD), 
and handling various cases like cd, cd ., cd .., and cd <directory>.
*/

void	ft_update_env_lst(char *pwd, char *oldpwd, t_env *env_lst)
{
    while (env_lst != NULL)
    {
        if (!ft_strcmp(env_lst->key, "PWD"))
        {
            free(env_lst->value);
            env_lst->value = ft_strdup(oldpwd);
        }
        else if (!ft_strcmp(env_lst->key, "OLDPWD"))
        {
            free(env_lst->value);
            env_lst->value = ft_strdup(pwd);
        }
        env_lst = env_lst->next;
    }
}

void	ft_goto_home(t_env *env_lst)
{
	char	*current_pwd;
	char	*dest;

	current_pwd = ft_getenv("PWD", env_lst);
	dest = ft_getenv("HOME", env_lst);
	ft_update_env_lst(dest, current_pwd, env_lst);
	if (chdir(dest) == -1)
	{
		ft_error("cd ", NULL);
		perror(strerror(errno));
		exit(1);
	}
}

void	ft_cd(t_cmd *cmd, t_env *env_lst)
{
    char    *old_pwd;
	char	*pwd;

	if (!cmd->command[1])
		ft_goto_home(env_lst);
    else if (!ft_strcmp(cmd->command[1], "-"))
    {
		old_pwd = ft_getenv("OLDPWD", env_lst);
        if (!old_pwd) // OLDPWD is not set yet
        {
			ft_error("cd: ", "OLDPWD not set\n");
            exit(1);
        }
		else
		{
			pwd = ft_getenv("PWD", env_lst);
			ft_update_env_lst(pwd, old_pwd, env_lst);
			ft_putstr_fd(old_pwd, 1); // first we print the path than we switch to it
			ft_putstr_fd("\n", 1);
			if (chdir(old_pwd) == -1)
			{
				ft_error("cd ", NULL);
				perror(strerror(errno));
				exit(1);
			}
		}
    }
	else
	{
		pwd = ft_getenv("PWD", env_lst);
		if (!ft_getenv("OLDPWD", env_lst))
			ft_add_env(&env_lst, "OLDPWD", pwd);
		ft_update_env_lst(cmd->command[1], pwd, env_lst);
		if (chdir(cmd->command[1]) == -1)
		{
			ft_error("cd ", NULL);
			perror(strerror(errno));
			exit(1);
		}
	}
}
