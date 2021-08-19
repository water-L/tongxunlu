#ifndef APP_H_INCLUDED
#define APP_H_INCLUDED

#include <gtk/gtk.h>
#include <string.h>

/*中文编码转换*/
#define PRC(x) (g_locale_to_utf8(x,-1,NULL,NULL,NULL))
/*最长输入长度*/
#define MAXLENGTH 50

enum {
    COLUMN_NAME,
    COLUMN_TELE,
    COLUMN_EMAIL,
    COLUMN_LOCA,
    COLUMN_WECHAT,
    N_COLUMN
};

struct current_st{
    int id;
    gchar name_st[100];
    gchar tele_st[100];
    gchar email_st[100];
    gchar loca_st[100];
    gchar wechat_st[100];
};

struct search_st{
    int id[20];
};

/*用于窗口创建的函数，位于win.c中*/
GtkWidget *create_main_window();
GtkWidget *create_add_dialog();
GtkWidget *create_del_dialog();

void quit(GtkWidget *window,gpointer data);
gboolean delete_event(GtkWidget *window,GdkEvent *event,gpointer data);
void dialog_add_button_clicked(GtkDialog *dialog,gint response,gpointer userdata);
void dialog_cha_button_clicked(GtkDialog *dialog,gint response,gpointer userdata);
void dialog_del_button_clicked(GtkDialog *dialog,gint response,gpointer userdata);
void added_active(GtkWidget *widget,gpointer user_data);
void chang_active(GtkWidget *widget,gpointer user_data);
void delet_active(GtkWidget *widget,gpointer user_data);
void search_active(GtkWidget *widget,gpointer userdata);
FILE *chose_element();

#endif // APP_H_INCLUDED
