/********************************
* ������������Ĵ��ں�������� *
********************************/

#include <gtk/gtk.h>
#include "app.h"

extern gchar *aorc;

GtkWidget *treeview;
GtkWidget *appbar;
GtkWidget *name_entry;
GtkWidget *tele_entry;
GtkWidget *email_entry;
GtkWidget *loca_entry;
GtkWidget *wechat_entry;

static GtkWidget *app;

/************************************
* ������ָ�����ƵĿؼ���ӵ������� *
************************************/
void add_widget_with_label(GtkContainer *box,gchar *caption,GtkWidget *widget)
{
    GtkWidget *label=gtk_label_new(caption);
    GtkWidget *hbox=gtk_hbox_new(TRUE,4);
    gtk_container_add(GTK_CONTAINER(hbox),label);
    gtk_container_add(GTK_CONTAINER(hbox),widget);
    gtk_container_add(box,hbox);
}

/*************
* ���������� *
*************/
GtkWidget *create_main_window()
{
    GtkWidget *vbox;
    GtkWidget *hbox1,*hbox2;
    GtkWidget *label;
    GtkWidget *entry;
    GtkWidget *search_button;
    GtkWidget *add_button;
    GtkWidget *change_button;
    GtkWidget *delete_button;
    GtkWidget *scrolledwindow;
    GtkCellRenderer *renderer;

    /*����һ�����б��⣬�̶�λ�úʹ�С�Ĵ���*/
    app=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(app),PRC("ͨѶ¼"));
    gtk_window_set_position(GTK_WINDOW(app),GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(app),540,480);

    /*��ӡ��޸ĺ�ɾ������*/
    add_button=gtk_button_new_with_label(PRC("���"));
    change_button=gtk_button_new_with_label(PRC("�޸�"));
    delete_button=gtk_button_new_with_label(PRC("ɾ��"));

    /*��������*/
    label=gtk_label_new(PRC("����"));
    entry=gtk_entry_new_with_max_length(MAXLENGTH);
    search_button=gtk_button_new_with_label(PRC("��ʼ"));

    scrolledwindow=gtk_scrolled_window_new(NULL,NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwindow),
                                   GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);

    /*���д��ڽ���*/
    vbox=gtk_vbox_new(FALSE,0);
    hbox1=gtk_hbox_new(TRUE,0);
    hbox2=gtk_hbox_new(FALSE,0);

    /*����������ʾ�ؼ�*/
    gtk_box_pack_start(GTK_BOX(vbox),hbox1,FALSE,FALSE,5);
    gtk_box_pack_start(GTK_BOX(vbox),hbox2,FALSE,FALSE,5);
    gtk_box_pack_start(GTK_BOX(hbox1),add_button,TRUE,FALSE,5);
    gtk_box_pack_start(GTK_BOX(hbox1),change_button,TRUE,FALSE,5);
    gtk_box_pack_start(GTK_BOX(hbox1),delete_button,TRUE,FALSE,5);
    gtk_box_pack_start(GTK_BOX(hbox2),label,FALSE,FALSE,5);
    gtk_box_pack_start(GTK_BOX(hbox2),entry,TRUE,TRUE,6);
    gtk_box_pack_start(GTK_BOX(hbox2),search_button,FALSE,FALSE,5);
    gtk_box_pack_start(GTK_BOX(vbox),scrolledwindow,TRUE,TRUE,0);
    gtk_container_add(GTK_CONTAINER(app),vbox);

    treeview=gtk_tree_view_new();
    renderer=gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview),
                                                COLUMN_NAME,PRC("����"),
                                                renderer,"text",COLUMN_NAME,
                                                NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview),
                                                COLUMN_TELE,PRC("�绰"),
                                                renderer,"text",COLUMN_TELE,
                                                NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview),
                                                COLUMN_EMAIL,PRC("e-mail"),
                                                renderer,"text",COLUMN_EMAIL,
                                                NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview),
                                                COLUMN_LOCA,PRC("��ַ"),
                                                renderer,"text",COLUMN_LOCA,
                                                NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview),
                                                COLUMN_WECHAT,PRC("΢��"),
                                                renderer,"text",COLUMN_WECHAT,
                                                NULL);

    gtk_tree_view_set_search_column(GTK_TREE_VIEW(treeview),COLUMN_NAME);

    gtk_container_add(GTK_CONTAINER(scrolledwindow),treeview);

    /*��������Ļص�����*/
    g_signal_connect(GTK_OBJECT(add_button),"clicked",
                     GTK_SIGNAL_FUNC(added_active),NULL);
    g_signal_connect(GTK_OBJECT(change_button),"clicked",
                     GTK_SIGNAL_FUNC(chang_active),NULL);
    g_signal_connect(GTK_OBJECT(delete_button),"clicked",
                     GTK_SIGNAL_FUNC(delet_active),NULL);
    g_signal_connect(GTK_OBJECT(search_button),"clicked",
                     GTK_SIGNAL_FUNC(search_active),entry);

    g_signal_connect(GTK_OBJECT(app),"delete_event",
                     GTK_SIGNAL_FUNC(delete_event),NULL);
    g_signal_connect(GTK_OBJECT(app),"destroy",
                     GTK_SIGNAL_FUNC(quit),NULL);

    return app;
}

/**********************************
* ����������Ӻ͸�����ϵ�˵ĵ��� *
**********************************/
GtkWidget *create_add_dialog()
{
    name_entry=gtk_entry_new_with_max_length(MAXLENGTH);
    tele_entry=gtk_entry_new_with_max_length(MAXLENGTH);
    email_entry=gtk_entry_new_with_max_length(MAXLENGTH);
    loca_entry=gtk_entry_new_with_max_length(MAXLENGTH);
    wechat_entry=gtk_entry_new_with_max_length(MAXLENGTH);

    GtkWidget *dialog;

    dialog=gtk_dialog_new_with_buttons(" ",app,
                                       GTK_DIALOG_DESTROY_WITH_PARENT,
                                       GTK_STOCK_OK,
                                       GTK_RESPONSE_ACCEPT,
                                       GTK_STOCK_CANCEL,
                                       GTK_RESPONSE_REJECT,
                                       NULL);

    add_widget_with_label(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox),
                          PRC("����"),name_entry);
    add_widget_with_label(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox),
                          PRC("�绰"),tele_entry);
    add_widget_with_label(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox),
                          PRC("e-mail"),email_entry);
    add_widget_with_label(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox),
                          PRC("��ַ"),loca_entry);
    add_widget_with_label(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox),
                          PRC("΢��"),wechat_entry);

    g_signal_connect(GTK_DIALOG(dialog),"response",
                    GTK_SIGNAL_FUNC(dialog_add_button_clicked),NULL);

    return dialog;
}

/****************************
* ��������ɾ����ϵ�˵ĵ��� *
****************************/
GtkWidget *create_del_dialog()
{
    name_entry=gtk_entry_new_with_max_length(MAXLENGTH);

    GtkWidget *dialog;

    dialog=gtk_dialog_new_with_buttons("Delete",app,
                                       GTK_DIALOG_DESTROY_WITH_PARENT,
                                       GTK_STOCK_OK,
                                       GTK_RESPONSE_ACCEPT,
                                       GTK_STOCK_CANCEL,
                                       GTK_RESPONSE_REJECT,
                                       NULL);

    add_widget_with_label(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox),
                          PRC("����"),name_entry);

    g_signal_connect(GTK_DIALOG(dialog),"response",
                     GTK_SIGNAL_FUNC(dialog_del_button_clicked),NULL);

    return dialog;
}

/********************
*ѡ��Ҫʹ�õ�ͨѶ¼*
********************/
FILE *chose_element()
{
    FILE *f;
    GtkWidget *chose;
    int result;
    gchar *a,title_c[100]={'\0'};
    GtkWidget *title;

    chose=gtk_dialog_new_with_buttons(PRC("��ʼ"),app,
                                       GTK_DIALOG_DESTROY_WITH_PARENT,
                                       GTK_STOCK_OK,
                                       GTK_RESPONSE_ACCEPT,
                                       GTK_STOCK_CANCEL,
                                       GTK_RESPONSE_REJECT,
                                       NULL);

    gtk_window_set_position(GTK_WINDOW(chose),GTK_WIN_POS_CENTER);
    title=gtk_entry_new_with_max_length(MAXLENGTH);

    add_widget_with_label(GTK_CONTAINER(GTK_DIALOG(chose)->vbox),
                          PRC("ѡ������ͨѶ¼"),title);

    gtk_widget_show_all(chose);
    result=gtk_dialog_run(GTK_DIALOG(chose));
    if(result==GTK_RESPONSE_ACCEPT){
        a=gtk_entry_get_text(GTK_ENTRY(title));
        strcpy(title_c,a);
        strcat(title_c,".txt");
    }
    gtk_widget_destroy(chose);

    return create_file(title_c);
}
