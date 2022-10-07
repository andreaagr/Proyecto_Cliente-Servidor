/*
    Creado por Andrea Garcia Ruiz, el 05/10/2022
    
    Las funciones descritas en este archivo separan la sentencia SQL y llaman a las operaciones CRUD
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "query.h"
#include "CRUD.h"
/*
    Dependiendo de la operacion deseada se recuperan los parametros necesarios. 

    La sintaxis sobre la que se basa es la siguiente:

    CREATE -> CREATE nombre_tabla (columna1, columna2, ...)
    INSERT -> INSERT INTO nombre_tabla VALUES (valor1, valor2, ...)
    UPDATE -> UPDATE nombre_tabla SET nombre_columna=nuevo_valor WHERE ID=id_registro
    DELETE -> DELETE FROM nombre_tabla WHERE ID=id_registro
    SELECT -> SELECT * FROM nombre_tabla
*/
void define_operation(char query[], int socket) {
    char* piece;
    char* table_name;
    char* values;
    char* column;
    char* column_name;
    char* id;
    set_socket(socket);
    piece = strtok(query, " ");
    if (strcmp(piece, "CREATE") == 0) {
        for (int i = 0; i < 3; i++)
        {
            if (i == 1)
            {
                table_name = strtok(NULL, " ");

            } else if (i == 2)
            {
                values = strtok(NULL, " ");
            } else {
                strtok(NULL, " ");
            }
        }
        memmove(&values[0], &values[1], strlen(values));
        values[strlen(values)-2] = '\0';
        create_table(table_name, values);
    }
    else if(strcmp(piece, "INSERT") == 0) {
        for (int i = 0; i < 4; i++)
        {
            if (i == 1)
            {
                table_name = strtok(NULL, " ");

            } else if (i == 3)
            {
                values = strtok(NULL, " ");
            } else {
                strtok(NULL, " ");
            }
        }
        memmove(&values[0], &values[1], strlen(values));
        values[strlen(values)-2] = '\0';
        insert_values(table_name, values);
    }
    else if(strcmp(query, "UPDATE") == 0) {
        for (int i = 0; i < 5; i++)
        {
            if (i == 0)
            {
                table_name = strtok(NULL, " ");

            } else if (i == 2)
            {
                column = strtok(NULL, " ");
            } else if (i == 4)
            {
                id = strtok(NULL, " ");
            } 
            else {
                strtok(NULL, " ");
            }
        }
        column_name = strtok(column, "=");
        values = strtok(NULL, "=");
        strtok(id, "=");
        update_record(table_name, column_name, values, strtok(NULL, "="));
    }
    else if(strcmp(query, "SELECT") == 0) {
        for (int i = 0; i < 3; i++)
        {
            if (i == 2)
            {
                table_name = strtok(NULL, " ");

            } 
            else {
                strtok(NULL, " ");
            }
        }
        select_all(strtok(table_name, "\n"));
        
    }
    else if(strcmp(query, "DELETE") == 0) {
        for (int i = 0; i < 4; i++)
        {
            if (i == 1)
            {
                table_name = strtok(NULL, " ");

            } else if (i == 3)
            {
                id = strtok(NULL, " ");
            }
            else {
                strtok(NULL, " ");
            }
        }
        strtok(id, "=");
        //printf(id);
        //printf(table_name);
        //printf(strtok(NULL, "="));
        delete_record(table_name, strtok(NULL, "="));
    }
    else 
        send_message("[-] Unrecognized command\n");
}
