#include "../Includes/minishell.h"

void	ft_sorted_envlst(t_cmd *cmd, t_env *env_lst)
{
    t_env   *current;
    t_env   *smallest;
    int     biggest_char;

    while (1)
    {
        smallest = NULL;
        biggest_char = 127;
		current = env_lst;
        while (current)
        {
            if (current->key[0] < biggest_char && current->has_printed != 0)
			{
				biggest_char = current->key[0];
				smallest = current;
			}
			current = current->next;
        }
		if (smallest == NULL)
			break ;
		smallest->has_printed = 0;
		printf("%s=%s\n", smallest->key, smallest->value);
    }
}

int	ft_isvalid(t_cmd *cmd)
{
	char	 *str;
	int		i;

	str = cmd->command[1];
	i = 0;
	if (ft_isalpha(str[0]) && str[0] != '_')
		return (1);
	while (str[i])
	{
		if (str[i] >= 33 && str[i] <= 47
			|| str[i] >= 58 && str[i] <= 64
				|| str[i] >= 91 && str[i] <= 94
					|| str[i] >= 123 && str[i] <= 126)
			return (1);
		i++;
	}
	return (0);
}

void	ft_export(t_cmd *cmd, t_env *env_lst)
{
	if (!cmd->command[1]) // export
	{
		ft_sorted_envlst(cmd, env_lst);
		return ;
	}
	// add it to the env lst it can be with = or without
	if (ft_isvalid(cmd) == 1)
	{
		ft_error("export: ", "not a valid identifier\n");
		exit(1);
	}
}

t_env *create_node(const char *key, const char *value) {
    t_env *new_node = (t_env *)malloc(sizeof(t_env));
    if (!new_node) return NULL;

    new_node->key = strdup(key);
    new_node->value = strdup(value);
    new_node->has_printed = 1;
    new_node->next = NULL;

    return new_node;
}

void append_node(t_env **head, const char *key, const char *value) {
    t_env *new_node = create_node(key, value);
    if (!new_node) return;

    if (*head == NULL) {
        *head = new_node;
        return;
    }

    t_env *temp = *head;
    while (temp->next) {
        temp = temp->next;
    }
    temp->next = new_node;
}

int main(void) {
    t_env *env_lst = NULL; // Initialize the head of the linked list

    // Add environment variables to the linked list
    append_node(&env_lst, "TMPDIR", "/var/folders/zz/zyxvpxvq6csfxvn_n000cmmm003555/T/");
    append_node(&env_lst, "XPC_FLAGS", "0x0");
	append_node(&env_lst, "AZBOZ", "0x0");
	append_node(&env_lst, "YOIIUIJ", "0x0");
    append_node(&env_lst, "amedcodes", "433");
    append_node(&env_lst, "LANG", "en_US.UTF-8");
    append_node(&env_lst, "TERM_PROGRAM", "Apple_Terminal");
    append_node(&env_lst, "XPC_SERVICE_NAME", "0");
    append_node(&env_lst, "bcddfd", "676FD00C-465C-4F9F-99BB-E80EF070CF08");
    append_node(&env_lst, "TERM", "xterm-256color");
    append_node(&env_lst, "SSH_AUTH_SOCK", "/private/tmp/com.apple.launchd.DOhOtrWTjA/Listeners");
    append_node(&env_lst, "SHELL", "/bin/zsh");
    append_node(&env_lst, "HOME", "/Users/moait-la");
    append_node(&env_lst, "LOGNAME", "moait-la");
    append_node(&env_lst, "USER", "moait-la");
    append_node(&env_lst, "PATH", "/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/munki:/Library/Apple/usr/bin");
    append_node(&env_lst, "SHLVL", "1");
    append_node(&env_lst, "PWD", "/Users/moait-la");
    append_node(&env_lst, "OLDPWD", "/Users/moait-la");
    append_node(&env_lst, "ZSH", "/Users/moait-la/.oh-my-zsh");
    append_node(&env_lst, "PAGER", "less");
    append_node(&env_lst, "LESS", "-R");
    append_node(&env_lst, "LSCOLORS", "Gxfxcxdxbxegedabagacad");
    append_node(&env_lst, "LS_COLORS", "di=1;36:ln=35:so=32:pi=33:ex=31:bd=34;46:cd=34;43:su=30;41:sg=30;46:tw=30;42:ow=30;43");
    append_node(&env_lst, "_", "/usr/bin/env");

    // Print the environment variables in sorted order
    ft_sorted_envlst(env_lst);

    // Free allocated memory (not shown here, but recommended for completeness)

    return 0;
}
