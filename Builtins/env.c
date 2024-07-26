
#include "../Includes/minishell.h"

void    ft_env(t_env *env)
{
    while (env->next != NULL)
    {
        printf("%s=%s\n", env->key, env->value);
        env = env->next;
    }
}
