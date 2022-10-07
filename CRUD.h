#ifndef _LIBRERIA
#define _LIBRERIA

void set_socket(int s);
void send_error(char const *error);
void create_table(char const *name, char const *columns);
void insert_values(char const *table, char const *values);
void select_all(char const *table);
void update_record(char const *table, char const *column, char const *new_value, char const *where);
void delete_record(char const *table, char const *where);

#include "CRUD.c"
#endif