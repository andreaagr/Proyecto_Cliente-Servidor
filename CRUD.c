#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "CRUD.h"

int my_socket;

void set_socket(int s) {
    my_socket = s;
}

void send_message(char const *message) {
    //printf("El mensaje a enviar es: %s", message);
    if (send(my_socket, message, strlen(message), 0) == -1)
        perror("Server-send() error");
}

void send_error(char const *error) {
    send_message(error);
}

int string_to_int(char const *number) {
    return (int) strtol(number, NULL, 10);
}

void create_table(char const *name, char const *columns) {
    FILE* file;
    file = fopen(name, "wt");
    fprintf(file, "%s\n", columns);
    fclose(file);
    send_message("[-] Table created\n");
}

void insert_values(char const *table, char const *values) {
    FILE* file;
    char line[200];
    int num_lines = 0, id;
    
    file = fopen(table, "rt");
    while(fgets(line, 200, file) != NULL)
        num_lines++;
    fclose(file);
    file = fopen(table, "at");
    if (num_lines > 1)
    {
        char* token = strtok(line, ",");
        id = string_to_int(token) + 1;
    } else {
        id = 1;
    }
    fprintf(file, "%d,%s\n", id, values);
    fclose(file);
    send_message("[-] Record added\n");
}

void select_all(char const *table) {
    FILE* file;
    char line[200];
    file = fopen(table, "rt");
    printf("Estamos en SELECT");
    fgets(line, 200, file);
    send_message("\n-------------------------------------------------------\n");
    send_message(line);
    send_message("-------------------------------------------------------\n");
    while (fgets(line, 200, file) != NULL) {
        send_message(line);
        send_message("\n");
    }
    fclose(file);
}

void update_record(
    char const *table, 
    char const *column, 
    char const *new_value, 
    char const *where
) {
    FILE* file;
    FILE* temp;
    char line[200];
    char table_structure[200];
    char* table_st_sin_salto;
    char* line_sin_salto;
    char* token_column;
    char* token_value;
    int num_line = 1, column_exists = -1, id, first_time = 1, num_column = 0;
    file = fopen(table, "rt");
    temp = fopen("temp", "wt");
    id = string_to_int(where);
    if(id > 0) {
        fgets(table_structure, 200, file);
        fputs(table_structure, temp);
        table_st_sin_salto = strtok(table_structure, "\n");
        
        token_column = strtok(table_st_sin_salto, ",");
        while (token_column != NULL)
        {
            if(strcmp(token_column, column) == 0) {
                column_exists = num_column;
            }
            num_column++;
            token_column = strtok(NULL, ",");
        }
        
        if (column_exists != -1)
        {
            while (fgets(line, 200, file) != NULL)
            {
                line_sin_salto = strtok(line, "\n");
                if (num_line == id)
                {
                    token_value = strtok(line_sin_salto, ",");
                    fprintf(temp, "%d", id);
                    for (int i = 1; i < num_column; i++)
                    {
                        token_value = strtok(NULL, ",");
                        if (i == column_exists)
                        {
                            fprintf(temp, ",%s", new_value);
                            send_message("[-] Record updated\n");
                        } else {
                            fprintf(temp, ",%s", token_value);
                        }
                    }
                    fputs("\n", temp);
                } else {
                    fprintf(temp, "%s\n", line);
                }
                num_line++;
            }
            if (num_line < id)
            {
                send_message("[-] Operation update failed <Id doesn't exist>\n");
            }
            
            remove(table);
            rename("temp", table);
        } else {
            send_message("[-] Operation update failed <Column doesn't exist>\n");
        }
    } else {
        send_message("[-] Operation update failed <Invalid id>\n");
    }
    fclose(file);
    fclose(temp);
}

void delete_record(char const *table, char const *where) {
    FILE* file;
    FILE* temp;
    char line[200];
    int id, num_line = 0;
    char* has_next_line;
    file = fopen(table, "rt");
    temp = fopen("temp", "wt");
    id = string_to_int(where);

    if (id > 0)
    {
        has_next_line = fgets(line, 200, file);
        while (has_next_line != NULL) {
            if (num_line != id)
            {
                fputs(line, temp);
            } else
                send_message("[-] Record deleted\n");
            num_line++;
            has_next_line = fgets(line, 200, file);
        }
    }
    fclose(file);
    fclose(temp);
    remove(table);
    rename("temp", table);
}