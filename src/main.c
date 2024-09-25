#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ssize_t getline(char **lineptr, size_t *n, FILE *stream)
{
    size_t pos = 0;
    int c;

    if (*lineptr == NULL || *n == 0)
    {
        *n = 1024;
        *lineptr = (char *)malloc(*n);
        if (*lineptr == NULL)
        {
            return -1;
        }
    }

    while ((c = fgetc(stream)) != EOF)
    {
        if (pos + 1 >= *n)
        {
            *n *= 2;
            char *new_line = (char *)realloc(*lineptr, *n);
            if (new_line == NULL)
            {
                return -1;
            }
            *lineptr = new_line;
        }
        (*lineptr)[pos++] = (char)c;
        if (c == '\n')
        {
            break;
        }
    }

    if (pos == 0 && c == EOF)
    {
        return -1;
    }

    (*lineptr)[pos] = '\0';
    return pos;
}

typedef struct
{
    char *buffer;
    size_t buffer_length;
    ssize_t input_length;
} InputBuffer;

InputBuffer *new_input_buffer()
{
    InputBuffer *input_buffer = malloc(sizeof(InputBuffer));
    input_buffer->buffer = NULL;
    input_buffer->buffer_length = 0;
    input_buffer->input_length = 0;
    return input_buffer;
}

void print_prompt()
{
    printf("db > ");
}

void read_input(InputBuffer *input_buffer)
{
    ssize_t bytes_read = getline(&input_buffer->buffer, &input_buffer->buffer_length, stdin);

    if (bytes_read <= 0)
    {
        printf("Error reading input\n");
        exit(EXIT_FAILURE);
    }

    // Ignore trailing newline // for ignoring "\n" ?
    input_buffer->input_length = bytes_read - 1;
    input_buffer->buffer[input_buffer->input_length] = '\0';
}

void close_input_buffer(InputBuffer *input_buffer)
{
    free(input_buffer->buffer);
    free(input_buffer);
}

int main(int argc, char *argv[])
{
    InputBuffer *input_buffer = new_input_buffer();
    while (true)
    {
        print_prompt();
        read_input(input_buffer);

        if (strcmp(input_buffer->buffer, ".exit") == 0)
        {
            close_input_buffer(input_buffer);
            exit(EXIT_SUCCESS);
        }
        else
        {
            printf("Unrecognized command '%s' .\n", input_buffer->buffer);
        }
    }

    printf("Hello world");
}
