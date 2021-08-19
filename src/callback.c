/*******************
*创建各种回调函数*
*******************/
#include <gtk/gtk.h>
#include "app.h"
#include "data.h"

extern GtkWidget *treeview;
extern GtkWidget *app;
extern GtkWidget *appbar;
extern GtkWidget *name_entry;
extern GtkWidget *tele_entry;
extern GtkWidget *email_entry;
extern GtkWidget *loca_entry;
extern GtkWidget *wechat_entry;
extern gchar *aorc;

static GtkWidget *added_dialog;
static GtkWidget *dele_dialog;

/*********
*关闭应用*
*********/
void quit(GtkWidget *window,gpointer data)
{
    //database_end();
    gtk_main_quit();
}

/*************
*判断是否退出*
*************/
gboolean confirm()
{
    gint result;
    GtkWidget *dialog;
    dialog=gtk_message_dialog_new(NULL,GTK_DIALOG_MODAL,
                                  GTK_MESSAGE_QUESTION,
                                  GTK_BUTTONS_YES_NO,
                                  PRC("确定退出？"));
    gtk_window_set_position(GTK_WINDOW(dialog),GTK_WIN_POS_CENTER);
    result=gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);

    return (result==GTK_RESPONSE_YES);
}

gboolean delete_event(GtkWidget *window,GdkEvent *event,gpointer data)
{
    return !confirm();
}

/****************
*添加通讯录成员*
****************/
void dialog_add_button_clicked(GtkDialog *dialog,gint response,gpointer userdata)
{
    const gchar *name_const;
    const gchar *tele_const;
    const gchar *email_const;
    const gchar *loca_const;
    const gchar *wechat_const;
    gchar name[100]={'\0'};
    gchar tele[100]={'\0'};
    gchar email[100]={'\0'};
    gchar loca[100]={'\0'};
    gchar wechat[100]={'\0'};
    gchar *s;
    gint *id;

    if(response==GTK_RESPONSE_ACCEPT){
        name_const=gtk_entry_get_text(GTK_ENTRY(name_entry));
        tele_const=gtk_entry_get_text(GTK_ENTRY(tele_entry));
        email_const=gtk_entry_get_text(GTK_ENTRY(email_entry));
        loca_const=gtk_entry_get_text(GTK_ENTRY(loca_entry));
        wechat_const=gtk_entry_get_text(GTK_ENTRY(wechat_entry));

        strcpy(name,name_const);
        strcpy(tele,tele_const);
        strcpy(email,email_const);
        strcpy(loca,loca_const);
        strcpy(wechat,wechat_const);

        if(strcmp(aorc,"change"))
            add_element(name,tele,email,loca,wechat);
        else
            cha_element(name,tele,email,loca,wechat);
    }

    added_dialog=NULL;
    gtk_widget_destroy(GTK_WIDGET(dialog));
}

/****************
*删除通讯录成员*
****************/
void dialog_del_button_clicked(GtkDialog *dialog,gint response,gpointer userdata)
{
    const gchar *name_const;

    gchar name[100]={'\0'};

    gchar *s;
    gint *id;

    if(response==GTK_RESPONSE_ACCEPT){
        name_const=gtk_entry_get_text(GTK_ENTRY(name_entry));

        strcpy(name,name_const);

        del_element(name);
    }

    dele_dialog=NULL;
    gtk_widget_destroy(GTK_WIDGET(dialog));
}

/****************
*填充GtkTreeView*
****************/
void search_active(GtkWidget *button,gpointer userdata)
{
    struct current_st element;
    struct search_st res;
    gint res1,res2,res3;
    gchar track_title[110];
    const gchar *search_string_const;
    gchar search_string[200]={'\0'};
    gchar search_text[200]={'\0'};
    gint i=0,j=0;
    int nod;

    GtkTreeStore *tree_store;
    GtkTreeIter parent_iter,child_iter;

    /*寻找输入的联系人*/
    search_string_const=gtk_entry_get_text(GTK_ENTRY(userdata));
    strcpy(search_string,search_string_const);
    res1=find_element(search_string,&res);

    tree_store=gtk_tree_store_new(N_COLUMN,G_TYPE_STRING,G_TYPE_STRING,
                                  G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING);

    /*显示搜索结果*/
    while(i<res1&&i</*MAX_RESULT*/20){
        nod=get_element(res.id,&element);
        res.id[nod]=0;
        i++;
        gtk_tree_store_append(tree_store,&parent_iter,NULL);
        gtk_tree_store_set(tree_store,&parent_iter,
                           COLUMN_NAME,element.name_st,
                           COLUMN_TELE,element.tele_st,
                           COLUMN_EMAIL,element.email_st,
                           COLUMN_LOCA,element.loca_st,
                           COLUMN_WECHAT,element.wechat_st,
                           -1);
    }

    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview),GTK_TREE_MODEL(tree_store));
}

/************
*创建输入框*
************/
void added_active(GtkWidget *dialog,gpointer user_data)
{
    if(added_dialog!=NULL)
        return;

    aorc="add";
    added_dialog=create_add_dialog();
    gtk_widget_show_all(added_dialog);
}

void chang_active(GtkWidget *widget,gpointer user_data)
{
    if(added_dialog!=NULL)
        return;

    aorc="change";
    added_dialog=create_add_dialog();
    gtk_widget_show_all(added_dialog);
}

void delet_active(GtkWidget *dialog,gpointer user_data)
{
    if(dele_dialog!=NULL)
        return;

    dele_dialog=create_del_dialog();
    gtk_widget_show_all(dele_dialog);
}

/**********
*关闭程序*
**********/
gboolean close_app(GtkWidget *window,gpointer data)
{
    gboolean exit;

    if((exit=confirm()))
        quit(NULL,NULL);
    return exit;
}
