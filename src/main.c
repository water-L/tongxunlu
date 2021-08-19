#include <stdio.h>
#include <stdlib.h>
#include "data.h"
#include "app.h"

gchar *t;/*存储所操作的文件名*/

gchar *aorc;

int main(gint argc,gchar *argv[])
{
    FILE *fp;
    GtkWidget *main_window;



    gtk_init(&argc,&argv);
    main_window=create_main_window();
    t=calloc(100,sizeof(gchar));
    if((fp=chose_element())==NULL)
        return 0;
    gtk_widget_show_all(main_window);

    gtk_main();
    return 0;
}
