#include "grep.h"

file* exitfile(tree* t, char* name)
{
    file* temp = t->current->file_inside;
    while (temp != NULL)
    {
        if (strcmp(temp->name, name) == 0)
            return temp;
        temp = temp->right;
    }
    return NULL;
}

void grep(tree* t, char* pattern, char* filename)
{
    FILE* grep_output_fp = fopen("grep_output.txt", "w");
    if (!grep_output_fp)
    {
        perror("grep: cannot open grep_output.txt");
        return;
    }

    file* target = exitfile(t, filename);
    if (!target)
    {
        printf("grep: file not found\n");
        fclose(grep_output_fp);
        return;
    }

    char temp_data[1024];
    strcpy(temp_data, target->data);

    char* line = strtok(temp_data, "\n");
    while (line != NULL)
    {
        if (strstr(line, pattern))
        {
            printf("%s: %s\n", filename, line);
            fprintf(grep_output_fp, "%s: %s\n", filename, line);
        }
        line = strtok(NULL, "\n");
    }

    fclose(grep_output_fp);
}

