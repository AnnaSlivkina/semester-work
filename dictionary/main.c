#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int read_line(char **str)
{
    char tmp_char = '\0';
    size_t size = 0;
    int isBreak = 0;
    int isEOF = 0;
    size_t block_size = 100;
    *str = (char *) malloc(block_size * sizeof(char));

    while ((tmp_char = getchar()))
    {
        if (tmp_char == '\n' || tmp_char == EOF)
        {
            tmp_char = '\0';
            isBreak = 1;
            if (tmp_char == EOF) isEOF = 1;
        }

        if (size + 1 > block_size)
        {
            block_size *= 2;
            char *tmp_str = (char *) realloc(*str, block_size * sizeof(char));

            if (tmp_str == NULL)
            {
                printf("[error]");
                free(*str);
                return 0;
            }

            *str = tmp_str;
        }

        (*str)[size++] = tmp_char;
        if (isBreak) break;
    }
    if (isEOF) return -1;
    else return 1;
}

void write_lines(char **lines, size_t num_lines)
{
    size_t i = 0;
    for (i = 0; i < num_lines; ++i)
    {
        printf("%s\n", lines[i]);
    }
}

void free_lines(char **lines, size_t num_lines)
{
    size_t i = 0;
    for (i = 0; i < num_lines; ++i)
    {
        free(lines[i]);
    }
}


size_t filter_lines(char **lines, size_t num_lines, char ***true_lines)
{
    int flag = 0;
    size_t i = 0;
    size_t j = 0;
    size_t true_num = 0;
    char **res_lines = (char **) malloc(num_lines * sizeof(char *));
    for (i = 0; i < num_lines; ++i)
    {
        for (j = 0; lines[i][j] != '\0'; ++j)
        {
            if (flag < 0) break;
            if (lines[i][j] == '(') flag++;
            if (lines[i][j] == ')') flag--;
        }

        if (!flag)
        {
            res_lines[true_num] = (char *) malloc((strlen(lines[i]) + 1) * sizeof(char));
            if (res_lines == NULL)
            {
                printf("[error]");
                free(res_lines);
                return -1;
            }
            strcpy(res_lines[true_num++], lines[i]);
        }
        flag = 0;
    }
    *true_lines = res_lines;
    return true_num;
}


int main(int argc, char const *argv[])
{
    int block_size = 100;
    char **lines = (char **) malloc(block_size * sizeof(char *));
    char *str = NULL;
    int isRead = 0;
    int num_lines = 0;
    for(;;)
    {
        isRead = read_line(&str);
        if (isRead == 0)
        {
            return 0;
        }

        if (!strlen(str))
        {
            free(str);
            break;
        }

        if (num_lines + 1 > block_size)
        {
            block_size *= 2;
            char **tmp_lines = (char **) realloc(lines, block_size * sizeof(char *));
            if (tmp_lines == NULL)
            {
                printf("[error]");
                free(str);
                free(lines);
                return 0;
            }
            lines = tmp_lines;
        }

        lines[num_lines] = (char *) malloc((strlen(str) + 1) * sizeof(char));
        if (lines == NULL)
        {
            printf("[error]");
            free(lines);
            free(str);
            return 0;
        }
        strcpy(lines[num_lines++], str);
        free(str);

        if (isRead == -1)
        {
            break;
        }
    }
    if (!num_lines)
    {
        printf("[error]");
        return 0;
    }

    char **true_lines = NULL;
    int true_num = filter_lines(lines, num_lines, &true_lines);

    if (true_num != -1) write_lines(true_lines, true_num);

    free_lines(true_lines, true_num);
    free(true_lines);

    free_lines(lines, num_lines);
    free(lines);
    return 0;
}

