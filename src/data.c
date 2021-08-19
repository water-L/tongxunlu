#include "data.h"

extern gchar *t;

/**************
*错误提示信息*
**************/
void warn(gchar *s)
{
    GtkWidget *warning;
    gint re;
    warning=gtk_message_dialog_new(NULL,GTK_DIALOG_MODAL,
                                   GTK_MESSAGE_ERROR,
                                   GTK_BUTTONS_CLOSE,
                                   PRC(s));
    gtk_window_set_position(GTK_WINDOW(warning),GTK_WIN_POS_CENTER);
    re=gtk_dialog_run(GTK_DIALOG(warning));
    gtk_widget_destroy(warning);

    return;
}

/***********************************
*创建或打开文本文件，若失败则报错*
***********************************/
FILE *create_file(gchar title_c[100])
{
    FILE *f;
    GtkWidget *warning;
    gint re;
    char a[2];

    strcpy(t,title_c);
    if(strlen(t)==0)
        return NULL;
    if((f=fopen(t,"ab"))==NULL){
        warn("无法打开或创建通讯录");
        return NULL;
    }
    else{
        rewind(f);
        fclose(f);
        return f;
    }
}

/****************
*添加联系人内容*
****************/
void add_element(gchar name[100],gchar tele[100],gchar email[100],
                 gchar loca[100],gchar wechat[100])
{
    GtkWidget *warning;
    FILE *f;
    gint re;
    long file_pos=-1L,l;
    int n=0,item=0;
    gchar temp[100]={'\0'};

    if(strlen(name)==0){
        warn("请输入姓名");
        return;
    }

    if((f=freopen(t,"rb+",stdout))==NULL){
        warn("打开指定文件出现错误");
        return ;
    }

    rewind(f);
    file_pos=find_name(f,name,&n);
    /*新联系人*/
    if(file_pos==-1L){
        rewind(f);
        fseek(f,0L,SEEK_END);
        n++;
        if(strlen(tele)==0)
            tele[0]='*';
        if(strlen(email)==0)
            email[0]='*';
        if(strlen(loca)==0)
            loca[0]='*';
        if(strlen(wechat)==0)
            wechat[0]='*';
        printf("%d %s %s %s %s %s\n",n,name,tele,email,loca,wechat);
        fclose(f);
    }
    /*已有联系人*/
    else{
        warn("联系人已存在");
    }
    return;
}

/****************
*查找结果的个数*
****************/
gint find_element(gchar search_string[200],struct search_st *res)
{
    FILE *f;
    gchar c_str[100]={'\0'};
    int n,i,j,k,l,length;
    gint id,num=0;
    long e;

    if((f=fopen(t,"rb+"))==NULL){
        warn("无法打开或创建通讯录");
        return NULL;
    }
    rewind(f);

    l=0;

    while(fscanf(f,"%d",&id)==1){
        i=0;
        j=0;
        fscanf(f,"%s",c_str);
        while(search_string[j]){
            if(search_string[j]==c_str[i]){
                j++;
            }
            i++;
            if(c_str[i]=='\0')
                break;
        }

        if(j==strlen(search_string))
            n=1;
        else
            n=0;
        fscanf(f,"%*[^\n]");
        if(n){
            if(l>20)break;
            res->id[l]=id;
            l++;
            num++;
        }
    }

    fclose(f);

    return num;
}

/**************
*打印查找结果*
**************/
int get_element(int id[20],struct current_st* element)
{
    FILE *f;
    long e;
    gint n_id;
    gchar tem[100]={'\0'};
    int i=0,n=0,nod;

    if((f=fopen(t,"rb+"))==NULL){
        warn("无法打开或创建通讯录");
        return NULL;
    }

    rewind(f);

    while(id[i]==0)
            i++;

    while(fscanf(f,"%d",&n_id)==1){
        if(n_id==id[i]){
            if(n==0){
                element->id=id[i];
                fscanf(f,"%s",element->name_st);
                fscanf(f,"%s",element->tele_st);
                fscanf(f,"%s",element->email_st);
                fscanf(f,"%s",element->loca_st);
                fscanf(f,"%s",element->wechat_st);
                nod=i;
            }
            else{
                fscanf(f,"%s",tem);
                if(strcmp(tem,element->name_st)<0){
                    element->id=id[i];
                    strcpy(element->name_st,tem);
                    fscanf(f,"%s",element->tele_st);
                    fscanf(f,"%s",element->email_st);
                    fscanf(f,"%s",element->loca_st);
                    fscanf(f,"%s",element->wechat_st);
                    nod=i;
                }
                else
                    fscanf(f,"%*[^\n]");
            }
            i++;
            while(id[i]==0)
                    i++;
            n++;
        }
        else
            fscanf(f,"%*[^\n]");
    }
    fclose(f);

    return nod;
}

/****************************************************
*判断所修改的联系人是否存在&寻找所修改联系人的位置*
****************************************************/
long find_name(FILE *f,gchar name[100],int *n)
{
    long e;
    gchar com[100]={'\0'},w[300]={'\0'};

    fseek(f,0L,SEEK_END);
    e=ftell(f);
    rewind(f);

    if(!e)
        return -1L;

    fscanf(f,"%d",n);
    fscanf(f,"%s",com);

    while(1){
        if(strcmp(name,com)==0)
            return ftell(f);
        fgets(w,sizeof(w),f);
        if(ftell(f)==e)
            break;
        fscanf(f,"%d",n);
        fscanf(f,"%s",com);
    }

    return -1L;
}

/****************
*修改联系人信息*
****************/
void cha_element(gchar name[100],gchar tele[100],gchar email[100],
                 gchar loca[100],gchar wechat[100])
{
    FILE *f,*tempfile;
    int n=0,le;
    long file_pos,lo,e;

    if((f=freopen(t,"rb+",stdout))==NULL){
        warn("打开指定文件出现错误");
        return ;
    }

    fseek(f,0L,SEEK_END);
    e=ftell(f);
    rewind(f);
    file_pos=find_name(f,name,&n);
    /*若联系人不存在，则提示*/
    if(file_pos==-1L){
        warn("无该联系人");
        return;
    }
    /*若联系人存在，则修改*/
    tempfile=tmpfile();
    rewind(f);
    lo=file_pos+1L;

    while(ftell(f)!=lo){
        fprintf(tempfile,"%c",fgetc(f));
    }

    lo=change(f,tempfile,tele);
    fprintf(tempfile," ");
    fseek(f,lo,SEEK_SET);
    lo=change(f,tempfile,email);
    fprintf(tempfile," ");
    fseek(f,lo,SEEK_SET);
    lo=change(f,tempfile,loca);
    fprintf(tempfile," ");
    fseek(f,lo,SEEK_SET);
    lo=change(f,tempfile,wechat);
    fseek(f,lo,SEEK_SET);

    while(ftell(f)!=e){
        fprintf(tempfile,"%c",fgetc(f));
    }

    fclose(f);
    f=fopen(t,"wb+");
    fseek(tempfile,0L,SEEK_END);
    e=ftell(tempfile);
    rewind(tempfile);

    while(ftell(tempfile)!=e)
        fprintf(f,"%c",fgetc(tempfile));
    fclose(tempfile);
    fclose(f);

    return;
}

long change(FILE *f,FILE *tempfile,gchar a[100])
{
    gchar tmp[100]={'\0'};
    long le;

    fscanf(f,"%s",tmp);
    le=strlen(tmp);
    tmp[le]='\0';

    if(a[0]=='\0'){
        fprintf(tempfile,"%s",tmp);
        return ftell(f);
    }
    else{
        fprintf(tempfile,"%s",a);
        return ftell(f);
    }
}

/************
*删除联系人*
************/
void del_element(gchar name[100])
{
    FILE *f,*tempfile;
    GtkWidget *warning;
    int n=0;
    long file_pos,lo,le,e;
    gchar w[300];

    if((f=freopen(t,"rb+",stdout))==NULL){
        warn("打开指定文件出现错误");
        return ;
    }

    rewind(f);
    file_pos=find_name(f,name,&n);
    /*若无该联系人，则提示*/
    if(file_pos==-1L){
        warn("无该联系人");
        return;
    }
    /*若有该联系人，则删去*/
    else{
        file_pos=file_pos-strlen(name)-2L;
        fseek(f,file_pos,SEEK_SET);
        lo=ftell(f);
        fseek(f,0L,SEEK_END);
        e=ftell(f);

        tempfile=tmpfile();
        rewind(f);

        while(ftell(f)!=lo){
            fprintf(tempfile,"%c",fgetc(f));
        }

        fgets(w,sizeof(w),f);

        while(ftell(f)!=e){
            fscanf(f,"%d",&n);
            n--;
            fgets(w,sizeof(w),f);
            le=strlen(w);
            w[le]='\0';
            fprintf(tempfile,"%d%s",n,w);
        }

        fclose(f);
        f=fopen(t,"wb+");
        fseek(tempfile,0L,SEEK_END);
        e=ftell(tempfile);
        rewind(tempfile);

        while(ftell(tempfile)!=e)
            fprintf(f,"%c",fgetc(tempfile));
        fclose(tempfile);
        fclose(f);

        return;
    }
}


