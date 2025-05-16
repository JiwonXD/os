#include "find.h"

#define MAX_THREADS 20

extern tree* structure;
int cd(tree* t, char* path);	// cd 함수 임시 선언

FILE* find_output_fp;
pthread_mutex_t find_mutex;

void* threaded_find(void* arg)
{
    char* thread_path = (char*)arg;

    if (cd(structure, thread_path) == 0)
    {
        pthread_mutex_lock(&find_mutex);
        fprintf(find_output_fp, "%s\n", thread_path);
        pthread_mutex_unlock(&find_mutex);

        find_file(structure, thread_path);
    }

    free(arg);
    return NULL;
}

void find_file(tree* t, char* path)
{
    file* f = t->current->file_inside;
    while (f != NULL)
    {
        fprintf(find_output_fp, "%s/%s\n", path, f->name);
        f = f->right;
    }
}

void find(tree* t, char* path)
{
    pthread_t threads[MAX_THREADS];
    int thread_count = 0;

    pthread_mutex_init(&find_mutex, NULL);

    find_output_fp = fopen("find_output.txt", "w");
    if (!find_output_fp)
    {
        perror("Cannot open find_output.txt.");
        return;
    }

    if (cd(t, path) == 0)
    {
        fprintf(find_output_fp, "%s\n", path);

        dir* now = t->current;
        dir* temp = now->left;

        while (temp != NULL && thread_count < MAX_THREADS)
        {
            char* thread_path = (char*)malloc(100);
            snprintf(thread_path, 100, "%s/%s", path, temp->name);
            pthread_create(&threads[thread_count++], NULL, threaded_find, thread_path);

            temp = temp->right;
        }

        find_file(t, path);

        for (int i = 0; i < thread_count; i++)
            pthread_join(threads[i], NULL);
    }

    fclose(find_output_fp);
    pthread_mutex_destroy(&find_mutex);
    cd(t, "/");
}

int cd(tree* t, char* path)  // 임시, 정확한 cd함수 구현이 필요함
{
	return 0;
}
