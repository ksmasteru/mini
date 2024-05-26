#include "tokens.h"
#include "parser.h"
#include "pipes.h"
#include "executer.h"
// < redirection id followed by the filename
// unless it is a heredoc << it is followed by a delimiter
t_token_type decode_type(char *start, int c)
{
    t_token_type type;

    if (c == '|')
        return  PIPE;
    else if (c == '<' && *(start + 1) == '<') // this might SEGv
        return (HEREDOC);
    else if (c == '<')
        return (FROM);
    else if (c == '>' && *(start + 1) == '>')
        return (APPEND);
    else if (c == '>')
        return (TO);
    else if (c == '(')
        return (PAREN_L);
    else if (c == ')')
        return (PAREN_R);
    else
        return (WORD);
}
char *clean_quotes_from_word(char *res, int j, int c);
char *make_quoted_word(char **str, int c)
{
  int qoutes_counter;
  char *res;
  int str_len;
  char *itr;
  int i;
  int j;

  i = 0;
  j = 0;
  qoutes_counter = 1;
  *str = *str + 1;
  if (**str == '\0')
  {
    printf("error no closing quote found\n");
    exit(1);
  }
  itr = *str;
  str_len = strlen(itr);
  res = (char *)malloc(sizeof(char) * (str_len + 1));
  res[str_len] = '\0';
  while (itr[i] != '\0')
  {
    if (itr[i]  == c)
      qoutes_counter++;
    res[j++] = itr[i++];
    *str = *str + 1;
    if (qoutes_counter % 2 == 0 && strchr(" \v\t\r\n|<>()", itr[i]))
          break;
  }
  if (qoutes_counter % 2 == 1)
  {
    printf("quoted arent balanced\n");
    exit(1);
  }
  res[j] = '\0';
  return (clean_quotes_from_word(res, j, c));
}

char *clean_quotes_from_word(char *res, int j, int c)
{
  char *clean_word;
  int i;
  int x;

  i = 0;
  x = 0;
  clean_word = (char *)malloc(sizeof(char) * (j + 1));
  if (!clean_word)
      return (NULL);
  clean_word[j] = '\0';
  while (res[i] != '\0')
  {
    if (res[i] != c)
      clean_word[x++] = res[i];
    i++;
  }
  clean_word[x] = '\0';
  free(res);
  return (clean_word);
}

char *word_till_quotes(char *str)
{
    int i;
    char *res;
    int j;
    int len;

    j = 0;
    i = 0;
    while (str[i])
    {
        if (str[i] == 34 || str[i] == 39)
            break;
        i++;
    }
    if (i == 0)
        return (NULL);
    res = (char *)malloc(sizeof(char) * (i + 1));
    if (!res)
        return (NULL);
    len = i;
    i = 0;
    while (i < len)
        res[j++] = str[i++];
    res[j] = '\0';
    return (res);
}
t_token  *lexer(char *str)
{
    char *start;
    size_t length;
    int word;
    t_token *head = NULL;
    char spaces[] = " \t\n\v\f\r";
    char *word_before_qoutes;
    char *new_word;
    while (*str)
    {
        word = 0;
        length = 0;
        while (*str && (strchr(" \t\v\r", *str)))
            str = str + 1;
        start = str;
        if(*str && strchr("|<>()", *str))
        {
            add_new_token(&head, *str, start, 1);
            str = str + 1;
            start = str;
        }
        while (*str && !(strchr(" \t\v\r|><)(", *str)))
        {
            if (*str == 34 || *str == 39)
            {
              word = 0;
              word_before_qoutes = word_till_quotes(start);
              new_word = ft_strjoin(word_before_qoutes, make_quoted_word(&str,  *str));
              add_new_token(&head, 1, new_word, strlen(new_word));
              break;
            }
            word = 1;
            str = str + 1;
            length++;
        }
        if (word == 1)
            add_new_token(&head, *str, start, length);
    }
    return (head);
}
t_token *make_new_node(t_token_type type, char *start, size_t length)
{
    t_token *new;

    new = malloc(sizeof(t_token));
    if (!new)
        return (NULL);
    new->type = type;
    new->location = (t_slice){start, length};
    new->next = NULL;
	new->up = NULL;
    new->down = NULL;
    return (new);
}

void add_new_token(t_token **head , int c, char *start, size_t length)
{
    t_token *tmp;
    t_token *new;

    tmp = *head;
    t_token_type type = decode_type(start, c);
    if (*head == NULL)
    {
        *head = make_new_node(type, start, length);
        return ;
    }
    new = make_new_node(type, start, length);
    if (!new)
        return ;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = new;
}
int parse_cmd(char *line, char **envp)
{
    char *str;
    t_data data;
    str = line;
    data.envp = envp;
    data.words_count = 0;
    t_token *tokens;
    t_token *tmp;
    t_tree *temp;
    t_tree *tempo;
    t_tree *root;
    t_token *tempa;
    char limiter[] = {"|<>()"};
    tokens = lexer(str);
	tokens_v2(&tokens);
    tmp = tokens;
    t_token *holder;
    // uncomment to test tokens
   /* while (tmp)
    {
        holder = tmp;
        printf(" parent type %d\n", tmp->type);
        if (tmp->down)
            printf("  down is %d\n", tmp->down->type);
        while (holder->up)
        {
            printf("  child up is %d\n", holder->up->type);
            printf("child value is %s\n", holder->up->location.location);
            holder = holder->up;
        }
        tmp = tmp->next;
    }
    exit(0);*/
    root = parser(tokens, &data);
    data.env = get_envp(data.envp);
    if (data.words_count > 1)
    {
      int *pids = (int *)malloc(sizeof(int) * (data.words_count - 1));
        int **pfd = (int **)malloc(sizeof(int *) * data.words_count - 1);
        for (int i = 0 ; i < data.words_count ; i++)
          pfd[i] = (int *)malloc(sizeof(int) * 2);
        fill_pipes(&pfd, data.words_count - 1);
        data.fdx = pfd;
        data.pids = pids;
        run_cmd(&root, data.words_count - 1, data.words_count, &data);
    }
    else
        execute_cmd(root, 0, 1, &data);
    exit(0);
}