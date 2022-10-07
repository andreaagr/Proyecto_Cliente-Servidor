/*
    Creado por Andrea Garcia Ruiz, el 05/10/2022
    
    Las funciones descritas en este archivo permiten efectuar las operaciones CRUD sobre archivos
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "CRUD.h"

int my_socket;

/*
    Define el socket al que le comunicaremos los resultados
*/
void set_socket(int s) {
    my_socket = s;
}

/*
    Nos ayuda a enviarle mansajes al cliente
*/
void send_message(char const *message) {
    if (send(my_socket, message, strlen(message), 0) == -1)
        perror("Server-send() error");
}

/*
    Convierte una cadena a entero
*/
int string_to_int(char const *number) {
    return (int) strtol(number, NULL, 10);
}

/*
    Crea un archivo con el nombre especificado y coloca como primera linea lo que contiene el parametro columnas
*/
void create_table(char const *name, char const *columns) {
    FILE* file;
    file = fopen(name, "wt");
    fprintf(file, "%s\n", columns);
    fclose(file);
    send_message("[-] Table created\n");
}
/*
    Agrega un nuevo registro al final del archivo, incrementando el valor del ultimo id
*/
void insert_values(char const *table, char const *values) {
    FILE* file;
    char line[200];
    int num_lines = 0, id;
    // Abre el archivo en modo lectura
    file = fopen(table, "rt");
    // Cuenta las filas de la tabla indicada, es decir, cuenta el numero de lineas
    while(fgets(line, 200, file) != NULL)
        num_lines++;
    fclose(file);
    // Abre el archivo nuevamente pero en modo adicion
    file = fopen(table, "at");
    if (num_lines > 1)
    {
        // Si no es el primer registro, obtiene el ultimo y le suma 1
        char* token = strtok(line, ",");
        id = string_to_int(token) + 1;
    } else {
        // Si es el primer registro, le corresponde el id 1
        id = 1;
    }
    // Cuando ya se conoce el id, se agregan los valores a la tabla
    fprintf(file, "%d,%s\n", id, values);
    fclose(file);
    send_message("[-] Record added\n");
}

/*
    Devuelve el contenido del archivo indicado
*/
void select_all(char const *table) {
    FILE* file;
    char line[200];
    file = fopen(table, "rt");
    fgets(line, 200, file);
    send_message("\n-------------------------------------------------------\n");
    send_message(line);
    send_message("-------------------------------------------------------\n");
    while (fgets(line, 200, file) != NULL) {
        send_message(line);
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
    // Archivo del que leemos el contenido de la tabla
    file = fopen(table, "rt");
    // Archivo auxiliar en el que colocamos los registros actualizados
    temp = fopen("temp", "wt");
    id = string_to_int(where);
    if(id > 0) {
        if (strcmp(column, "Id") == 0)
        {
            send_message("[-] Operation update failed <Id cannot be updated\n");
        } else {
            // Obtiene las columnas de la tabla
            fgets(table_structure, 200, file);
            // Guarda el contenido de table_structure en temp
            fputs(table_structure, temp);
            // Quita el salto de linea al final del renglon
            table_st_sin_salto = strtok(table_structure, "\n");
            // Separa la cadena por comas y guarda en token_column el nombre de la primera columna
            token_column = strtok(table_st_sin_salto, ",");
            while (token_column != NULL)
            {
                // Compara el nombre de la columna con el que dio el usuario y guarda el numero de columna
                if(strcmp(token_column, column) == 0) {
                    column_exists = num_column;
                }
                num_column++;
                // Obtiene la siguiente columna
                token_column = strtok(NULL, ",");
            }

            if (column_exists != -1)
            {
                // Itera sobre todos los registros guardado en el archivo
                while (fgets(line, 200, file) != NULL)
                {
                    // Quita el salto de linea de cada linea del archivo
                    line_sin_salto = strtok(line, "\n");
                    if (num_line == id)
                    {
                        token_value = strtok(line_sin_salto, ",");
                        fprintf(temp, "%d", id);
                        // Itera sobre todas las columnas de la linea
                        for (int i = 1; i < num_column; i++)
                        {
                            token_value = strtok(NULL, ",");
                            if (i == column_exists)
                            {
                                // Cuando la columna coincide con la especificada, coloca el nuevo valor
                                fprintf(temp, ",%s", new_value);
                                send_message("[-] Record updated\n");
                            } else {
                                // Si no coincide coloca el valor del archivo
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
                // Cuando hay cambios...
                // Se borra el archivo original
                remove(table);
                // Y temp se convierte en la tabla
                rename("temp", table);
            } else {
                send_message("[-] Operation update failed <Column doesn't exist>\n");
            }
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
    char full_line[200];
    int id;
    char* has_next_line;
    int record_updated = 0;
    id = string_to_int(where);

    if (id > 0)
    {
        file = fopen(table, "rt");
        temp = fopen("temp", "wt");
        has_next_line = fgets(line, 200, file);
        // Itera sobre todos los registros de la tabla
        while (has_next_line != NULL) {
            strcpy(full_line, line);
            // Si la linea no se quiere borrar, se manda a temp
            if (string_to_int(strtok(line, ",")) != id)
            {
                fputs(full_line, temp);
            } else
                record_updated = 1;
            has_next_line = fgets(line, 200, file);
        }

        if (record_updated == 1)
        {
            send_message("[-] Record deleted\n");
        } else
            send_message("[-] Operation delete failed <Id doesn't exist>\n");
        // Elimina la tabla original
        remove(table);
        // Temp se convierte en la tabla actualizada
        rename("temp", table);
        fclose(file);
        fclose(temp);
    } else {
        send_message("[-] Operation delete failed <Invalid id>\n");
    }
}