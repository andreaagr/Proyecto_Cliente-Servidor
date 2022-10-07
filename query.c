#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "query.h"
#include "CRUD.h"

void define_operation(char query[]) {
    char* piece;
    char* table_name;
    char* values;
    char* column;
    char* column_name;
    char* id;
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
        values[strlen(values)-1] = '\0';
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
        values[strlen(values)-1] = '\0';
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
        //printf(table_name);
        //printf(column_name);
        //printf(strtok(NULL, "="));
        //printf(values);
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
        select_all(table_name);
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
    else if(strcmp(query, "SALIR") == 0) {
        exit(0);
    } else 
        perror("[-] Unrecognized command\n");
}

/*
int main() {
    //char es[122] = "INSERT INTO TIENDA VALUES (G,K,G,O,H)";
    //char es[122] = "CREATE TABLE TIENDA (G,K,G,O,H)";
    //char es[122] = "UPDATE Alumno SET Apellido=Gomez WHERE Id=2";
    //char es[122] = "SELECT * FROM ALUMNO";
    //char es[122] = "DELETE FROM Alumno WHERE ID=7";
    define_operation(es);
}*/