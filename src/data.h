#ifndef DATA_H_INCLUDED
#define DATA_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "app.h"

/*最大显示数*/
#define MAXRESULT 10

/*数据处理函数，位于data.c中*/
gint find_element(gchar search_string[200],struct search_st *res);

int get_element(int id[20],struct current_st *element);

void add_element(gchar name[100],gchar tele[100],gchar email[100],
                 gchar loca[100],gchar wechat[100]);
void cha_element(gchar name[100],gchar tele[100],gchar email[100],
                 gchar loca[100],gchar wechat[100]);
void del_element(gchar name[100]);
void warn(gchar *sentence);

FILE *create_file(gchar title_c[100]);

long find_name(FILE *f,gchar name[100],int *n);
long change(FILE *f,FILE *tempfile,gchar a[100]);


#endif // DATA_H_INCLUDED
