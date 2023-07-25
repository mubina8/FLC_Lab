#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include <stdbool.h>

FILE *f1,*f2,*f3,*f4,*f5,*f6,*fp,*fp2,*fp3;
char s[100];
char arr[100];
char brr[100];
int swap[10000];
char c[1000][100];
char str[10000][10000];
int cur=0;
char b[100000];
char ar[10000],br[10000];
int sz[1000];
int xy;
struct table
{
    char *name, *idType, *dataType, *scope,*value;

};

struct table data[1000];
int k;


void insert()
{
    int index=1;
    //char c;
    f1 = fopen("idpair.txt","r");

    int j;

    fscanf(f1,"%[^\n]",b);

    fclose(f1);

    k = 0;

    for(int i=0; i<strlen(b); i++)
    {
        if(b[i]=='[')
        {
            j= 0;
            i++;
            while(b[i]!=']' && b[i]!=' ')
            {
                c[k][j++]  = b[i++];
            }
            c[k][j]  = '\0';
            k++;
            if(b[i]==' ')
            {
                j= 0;
                i++;
                while(b[i]!=']')
                {
                    c[k][j++]  = b[i++];
                }
                c[k][j]  = '\0';
                k++;
            }

        }
    }


    int z;


    int it = 0;
    char * scope = "global";
    for(int i=0; i<k; i++)
    {

        if(strcmp(c[i],"Id")==0)
        {

            if(strcmp(c[i+2],"(")==0)
            {
                if(strcmp(c[i-1],"int")==0 || strcmp(c[i-1],"double")==0 || strcmp(c[i-1],"float")==0 || strcmp(c[i-1],"char")==0 )
                {

                    swap[it++]  = cur;
                    data[cur].name=c[i+1];
                    data[cur].idType="func";
                    data[cur].dataType=c[i-1];
                    data[cur].scope=scope;
                    data[cur].value="";
                    scope = c[i+1];
                    i+= 2;
                    cur++;
                }
                else
                {
//[float][id x1][=][3.125][;][double][id f1][(][int][id x][)][{][double][id z][;][id z][=][0.01][;][return][id z]
//[;][}][int][id main][(][void][)][{][int][id n1][;][double][id z][;][id n1][=][25][;][id z][=][id f1][(][id n1][)][;]

                    int check  = search(c[i+1],"func","global");
                    if(check!=-1)
                        swap[it++] = check;
                    else printf("\nID %s is not in global scope\n",c[i+1]);
                    i+= 2;
                }
            }
            else if(strcmp(c[i+2],"=")==0)
            {
                if(strcmp(c[i-1],"int")==0 || strcmp(c[i-1],"double")==0 || strcmp(c[i-1],"float")==0 || strcmp(c[i-1],"char")==0 )
                {
                    if(search(c[i+1],"var",scope)==-1)
                    {
                        swap[it++]  = cur;
                        data[cur].name=c[i+1];
                        data[cur].idType="var";
                        data[cur].dataType=c[i-1];
                        data[cur].scope=scope;
                        if(c[i+3][0]>='0' && c[i+3][0]<='9' || c[i+3][0]=='.')
                        {
                            data[cur].value=c[i+3];
                        }
                        else
                        {
                            printf("\nID %s is invalid number\n",c[i+1]);
                        }
                        cur++;


                    }
                    else
                    {
                        printf("\nID %s already in %s scope\n",c[i+1],scope);
                    }
                }
                else
                {
                    int check = search(c[i+1],"var",scope);
                    if(check==-1)
                    {
                        printf("\nID %s is not in %s scope\n",c[i+1],scope);
                    }
                    else
                    {
                        if(c[i+3][0]>='0' && c[i+3][0]<='9' || c[i+3][0]=='.')
                        {
                            update(check,c[i+3]);
                        }

                        swap[it++]  = check;
                    }
                }
                i+=2;
            }
            else if(strcmp(c[i+2],";")==0 || strcmp(c[i+2],",")==0 || strcmp(c[i+2],")")==0 )
            {
                if(strcmp(c[i-1],"int")==0 || strcmp(c[i-1],"double")==0 || strcmp(c[i-1],"float")==0 || strcmp(c[i-1],"char")==0 )
                {
                    if(search(c[i+1],"var",scope)==-1)
                    {
                        swap[it++]  = cur;
                        data[cur].name=c[i+1];
                        data[cur].idType="var";
                        data[cur].dataType=c[i-1];
                        data[cur].scope=scope;
                        data[cur].value="";
                        cur++;

                    }
                    else printf("\nID %s already in %s scope\n",c[i+1],scope);
                    i+= 2;
                }
                else
                {
                    if(search(c[i+1],"var",scope)==-1)
                    {
                        printf("\nID %s is not in %s scope\n",c[i+1],scope);
                    }
                    else swap[it++]  = search(c[i+1],"var",scope);
                    i+= 2;
                }
            }
        }
        else if(strcmp(c[i],"}")==0)
        {
            scope = "global";
        }
    }

    xy=it;

}
void update(int cur,char* val)
{
    data[cur].value  = val;
}


int search(char* name,char* id,char* scope)
{
    int i=0;

    for(i=0; i<cur; i++)
    {
        if(strcmp(data[i].name,name)==0 && strcmp(data[i].idType,id)==0 && strcmp(data[i].scope,scope)==0 )
        {
            return i;
        }
    }
    return -1;
}

void display()
{
    printf("\nID\tName\tType\tData Type\tScope\tValue\n");
    printf("---------------------------------------------------------\n");

    int i=0;
    for(i=0; i<cur; i++)
    {
        printf("%d\t%s\t%s\t%s\t\t%s\t%s\n",i+1,data[i].name,data[i].idType,data[i].dataType,data[i].scope,data[i].value);


    }



    printf("\n\n");



}






bool isOperator(char* c)
{
    if (c[0] == '+' || c[0] == '-' || (strlen(c)==2 && c[0]=='<' && c[1]=='=') || c[0] == '*' ||
            c[0] == '/' || c[0] == '>' || c[0] == '<' ||
            c[0] == '=')
        return (true);
    return (false);
}

bool isSep(char* c)
{
    if(c[0]==';' || c[0]==',' || c[0]=='\'')
        return true;
    return false;
}

bool isPar(char* c)
{
    if(c[0]==')' || c[0]=='(' || c[0]=='}' || c[0]=='{' )
        return true;
    return false;
}

int isSep1(char c)
{
    if(c==';' || c==',' || c=='\'')
        return 1;
    return 0;
}
int isOp(char c)
{
    if(c=='+' || c=='-' || c=='*' || c=='/' || c=='>' || c=='<' || c=='!' || c=='=')
        return 1;
    return 0;
}
int isDigit1(char c)
{
    if(c>='0' && c<='9')
        return 1;
    return 0;
}
int isPar1(char c)
{
    if(c==')' || c=='(')
        return 1;
    return 0;
}

int isBrc(char c)
{
    if(c=='}' || c=='{')
        return 1;
    return 0;
}

int isSepa(char* c)
{
    if(strlen(c)==1 && isSep1(c[0]))
        return 1;
    return 0;
}


int isOper(char* c)
{
    if(strlen(c)>2)
        return 0;
    if(strlen(c)==1)
    {
        if(isOp(c[0]))
            return 1;
        return 0;
    }
    if(isOp(c[0]) && c[1]=='=')
        return 1;
    return 0;
}

int isParen(char* c)
{
    if(strlen(c)==1 && isPar1(c[0]))
        return 1;
    return 0;
}
int isBrace(char* c)
{
    if(strlen(c)==1 && isBrc(c[0]))
        return 1;
    return 0;
}

bool isDigit(char c)
{
    if(c>='0' && c<='9')
        return true;
    return false;
}

int isKey(char *str)
{
    int s=0;
    if(
        (!strcmp(str, "char")) || (!strcmp(str, "float")) || (!strcmp(str, "double")) ||
        (!strcmp(str, "int"))  || (!strcmp(str, "bool")) || (!strcmp(str, "return")) || (!strcmp(str, "void")) || (!strcmp(str, "if"))  || (!strcmp(str, "else")) || (!strcmp(str, "for")) )
    {
        s=1;
    }
    return s;
}



int isNum(char *lex)
{
    int i, l, s;
    i=0;
    if(isDigit(lex[i]))
    {
        s=1;
        i++;
    }
    else if(lex[i]=='.')
    {
        s=2;
        i++;
    }
    else s=0;
    l=strlen(lex);
    if(s==1)
        for(; i<l; i++)
        {
            if(isDigit(lex[i]))  s=1;
            else if(lex[i]=='.')
            {
                s=2;
                i++;
                break;
            }
            else
            {
                s=0;
                break;
            }
        }
    if(s==2)
        if(isDigit(lex[i]))
        {
            s=3;
            i++;
        }
        else
            s=0;
    if(s==3)
        for(; i<l; i++)
        {
            if(isDigit(lex[i]))  s=3;
            else
            {
                s=0;
                break;
            }
        }
    if(s==3) s=1;
    return s;
}

int isId(char* c)
{
    int i=0;
    int x = 0;
    for(i=0; i<strlen(c); i++)
    {
        if((c[i]>='a' && c[i]<='z') || (c[i]>='A' && c[i]<='Z') || c[i]=='_')
        {

            x = 1;
            continue;
        }
        else if(isDigit(c[i])==true)
        {
            if(x==0)
                return 0;
        }
        else return 0;
    }
    return 1;
}


void detecting_error()
{
    int i,n,t,k,j,x=0,y=0,m;
    fp = fopen("input.txt","r");
    fp2 = fopen("intermediate ouput.txt","w");
    fp3 = fopen("error.txt","w");
    i = 1;


    int c1 = 0, c2  = 0;
    while(fgets(str[i],500,fp))
    {
        int sz = strlen(str[i]);
        c1 = 0;
        for(j=0; j<sz; j++)
        {

            if(j+1<sz && str[i][j]=='/' && str[i][j+1]=='/')
            {
                c1 = 1;
                str[i][j]  = str[i][j+1]  = ' ';
                j++;
            }
            else if(j+1<sz && str[i][j]=='/' && str[i][j+1]=='*')
            {
                c2 = 1;
                str[i][j] = str[i][j+1]  =' ';
                j++;
            }
            else if(j+1<sz && str[i][j]=='*' && str[i][j+1]=='/')
            {
                c2 = 0;
                str[i][j]  = str[i][j+1] = ' ';
                j++;
            }
            else if(c1 || c2)
            {
                str[i][j]  = ' ';
            }

        }
        i++;
    }
    int totalsz = i;




    for(k=1; k<totalsz; k++)
    {

        int flag  =0;
        int n = strlen(str[k]);


        strcpy(ar,str[k]);
        j = 0;
        int sepOn = 0;
        for(i=0; i<n; i++)
        {

            if(isSep1(ar[i])==1)
            {
                if(ar[i]=='\'')
                {
                    if(sepOn)
                    {
                        sepOn = 0;
                        br[j++]  = ' ';
                        br[j++]  = ar[i];
                    }
                    else
                    {
                        br[j++]  = ar[i];
                        br[j++]  = ' ';
                        sepOn = 1;
                    }
                }
                else
                {
                    br[j++]  = ' ';
                    br[j++]  = ar[i];
                }
                br[j++]  = ' ';


            }
            else if(isOp(ar[i])==1)
            {
                if(ar[i+1]=='=')
                {
                    br[j++]  = ' ';
                    br[j++]  = ar[i];
                    br[j++]  = ar[i+1];
                    i++;
                    br[j++]  = ' ';
                }
                else
                {
                    br[j++]  = ' ';
                    br[j++]  = ar[i];
                    br[j++]  = ' ';
                }
                flag  =1;
            }
            else if(isDigit1(ar[i])==1)
            {
                if(flag==0)
                {
                    flag = 1;
                    br[j++]  = ' ';
                }
                br[j++]  = ar[i];
            }
            else if(isPar1(ar[i])==1)
            {
                br[j++]  = ' ';
                br[j++]  = ar[i];
                if(ar[i]!=')')
                    br[j++]  = ' ';
            }
            else if(isBrc(ar[i])==1)
            {
                br[j++]  = ' ';
                br[j++]  = ar[i];
                if(ar[i]!='}')
                    br[j++]  = ' ';
            }
            else
            {
                flag  =1;
                br[j++]  = ar[i];
            }
        }
        br[j-1]  = ' ';
        br[j] = '\0';

        strcpy(str[k],br);

    }



    for(k=1; k<totalsz; k++)
    {
        strcpy(ar,str[k]);
        br[0]  = '\0';
        j = 0;
        for(i=0; i<strlen(ar); i++)
        {
            if(ar[i]==' ')
            {
                br[j++]  = ' ';
                while(i<strlen(ar) && ar[i]==' ')
                    i++;
                i--;
            }
            else br[j++]  =ar[i];

        }
        br[j]  = '\0';
        strcpy(str[k],br);



    }




    int fb = 0,sb = 0;
    int isIf = 0,semicolonIf=0;
    for(k=1; k<=totalsz; k++)
    {
        j = 0;
        int forloop = 0;
        int pt = 0;
        fprintf(fp2,"%d ",k);
        printf("%d ",k);
        strcpy(br,str[k]);


        char pre[100] = {'-','1'};
        for(i=0; i<strlen(br); i++)
        {


            if(br[i]==' ')
            {


                ar[j++] = '\0';

                if(strlen(ar)==0)
                {
                    j  =0;
                    continue;
                }

                if(isSepa(ar)==1)
                {
                    fprintf(fp2,"sep %s ",ar);
                    printf("sep %s ",ar);
                    if(strcmp(ar,";")==0)
                    {
                        semicolonIf++;
                        if(forloop==2 || forloop==3)
                            forloop++;
                        else if(forloop>=4)
                        {
                            fprintf(fp3,"Only two semicolon in for loop at line %d\n",k);
                        }
                        else if(strcmp(ar,pre)==0)
                        {
                            fprintf(fp3,"Duplicate token at line %d\n",k);
                        }
                    }
                    else if(strcmp(ar,pre)==0)
                    {
                        fprintf(fp3,"%Duplicate token at line %d\n",k);
                    }


                }
                else if(isKey(ar)==1)
                {

                    fprintf(fp2,"kw %s ",ar);
                    printf("kw %s ",ar);
                    if(strcmp(ar,"if")==0)
                    {
                        isIf  = 1;
                        semicolonIf = 0;
                    }
                    else if(strcmp(ar,"else")==0)
                    {
                        if(isIf==0 || semicolonIf!=1)
                        {
                            fprintf(fp3,"'else' without a previous 'if' at line %d\n",k);
                        }
                        else isIf  = 0;
                    }
                    else if(strcmp(ar,"for")==0)
                    {
                        forloop = 1;
                    }
                    if(strcmp(ar,pre)==0)
                    {
                        fprintf(fp3,"Duplicate token at line %d\n",k);
                    }


                }
                else if(isId(ar)==1)
                {


                    fprintf(fp2,"id %s ",ar);
                    printf("id %s ",ar);
                    if(strcmp(ar,pre)==0)
                    {
                        fprintf(fp3,"Duplicate token at line %d\n",k);
                    }

                }
                else if(isOper(ar)==1)
                {
                    fprintf(fp2,"op %s ",ar);
                    printf("op %s ",ar);

                }
                else if(isParen(ar)==1)
                {
                    fprintf(fp2,"par %s ",ar);
                    printf("par %s ",ar);
                    if(strcmp(ar,"(")==0)
                    {
                        fb++;
                        if(forloop==1)
                            forloop++;
                    }
                    else
                    {
                        if(forloop>0 && forloop<4)
                        {
                            fprintf(fp3,"Expected ; in for loop at line %d\n",k);
                        }
                        else if(fb==0)
                            fprintf(fp3,"Misplaced ) at line %d\n",k);
                        else
                        {
                            fb--;
                            if(forloop>0)
                                forloop = 0;
                        }
                    }


                }
                else if(isBrace(ar)==1)
                {
                    fprintf(fp2,"brc %s ",ar);
                    printf("bra %s ",ar);
                    if(strcmp(ar,"{")==0)
                        sb++;
                    else
                    {

                        if(sb==0)
                            fprintf(fp3,"Misplaced } at line %d\n",k);
                        else sb--;
                    }

                }
                else if(isNum(ar)==1)
                {
                    fprintf(fp2,"num %s ",ar);
                    printf("num %s ",ar);

                }
                else
                {
                    fprintf(fp2,"unkn %s ",ar);
                    printf("unkn %s ",ar);
                    fprintf(fp3,"Undeclared/Unknown %s at line %d\n",ar,k);

                }
                j= 0;

                strcpy(pre,ar);

            }
            else
            {
                ar[j++]  = br[i];
            }

        }
        fprintf(fp2,"\n");
        printf("\n");
        sz[k] =  pt;
    }
    if(sb>0)
        fprintf(fp3,"Expected } at line %d\n",k-1);
    if(fb>0)
        fprintf(fp3,"Expected ) at line %d\n",k-1);





  fclose(fp);
  fclose(fp2);
  fclose(fp3);

}

//Assignement 5 functions

char strexpression[1000];
int pos =0;
int f = 0;

void Exp();
void stat();
void extn();
void relop();

void X()
{
    if(pos+2<strlen(strexpression) && strexpression[pos]=='b' && strexpression[pos+1]=='b')
    {
        pos+=2;
        X();
        return;
    }
    if(pos+2<strlen(strexpression) && strexpression[pos]=='b' && strexpression[pos+1]=='c')
    {
        pos+=2;
        X();
        return;
    }
    f = 1;
    return;
}

void A()
{
    if(strlen(strexpression)<2)
    {
        f=  0;
        return;
    }
    if(strexpression[pos]!='a')
    {
        f = 0;
        return;
    }
    pos++;
    X();
    if(f==1)
    {
        if(pos+1==strlen(strexpression) && strexpression[pos]=='d')
        {
            pos++;
            f = 1;
            return;
        }
        else
        {
            f = 0;
            return;
        }
    }
    else
    {
        f = 0;
        return;
    }
}

void ID()
{
    if(pos<strlen(strexpression) && strexpression[pos]>='a' && strexpression[pos]<='e')
    {
        pos++;
        f  =1;
        return;
    }
    f = 0;
    return;
}

void NUM()
{
    if(pos<strlen(strexpression) && strexpression[pos]>='0' && strexpression[pos]<='9')
    {
        pos++;
        f  =1;
        return;
    }
    f = 0;
    return;
}

void Factor()
{
    ID();
    if(f==1)
        return;
    NUM();
    if(f==1)
        return;
    if(pos<strlen(strexpression) && strexpression[pos]=='(')
    {
        pos++;
        Exp();
        if(f==1)
        {
            if(pos<strlen(strexpression) && strexpression[pos]==')')
            {
                pos++;
                f = 1;
                return;
            }
            f = 0;
            return;
        }
        f = 0;
        return;

    }
    f =0;
    return;
}

void Term()
{
    Factor();
    if(pos==strlen(strexpression) || f==0)
    {
        return;
    }
    if(strexpression[pos]=='*' || strexpression[pos]=='/')
    {
        pos++;
        Factor();
    }
}

void Exp()
{
    Term();
    if(pos==strlen(strexpression) || f==0)
    {
        return;
    }
    if(strexpression[pos]=='+' || strexpression[pos]=='-')
    {
        pos++;
        Term();

    }
}
void init()
{
    f = pos = 0;
}

void expn()
{
    Exp();
    if(f==1)
    {
        extn();
        return;
    }
    return;
}

void extn()
{
    int temp = pos;
    relop();
    if(f==1)
    {
        Exp();
        if(f==1)
        {
            return;
        }
    }
    pos = temp;
    f= 1;
    return;
}

void extn1()
{
    int temp = pos;
    if(pos+3<strlen(strexpression) && strexpression[pos]=='e' && strexpression[pos+1]=='l' && strexpression[pos+2]=='s' && strexpression[pos+3]=='e')
    {
        pos+=4;
        stat();
        if(f==1)
        {
            return;
        }

    }
    pos= temp;
    f =1;
    return;
}

void relop()
{
    if(pos+1<strlen(strexpression) && strexpression[pos]=='=' && strexpression[pos+1]=='=')
    {
        f = 1;
        pos+=2;
        return;
    }

    if(pos+1<strlen(strexpression) && strexpression[pos]=='!' && strexpression[pos+1]=='=')
    {
        f = 1;
        pos+=2;
        return;
    }
    if(pos+1<strlen(strexpression) && strexpression[pos]=='<' && strexpression[pos+1]=='=')
    {
        f = 1;
        pos+=2;
        return;
    }
    if(pos+1<strlen(strexpression) && strexpression[pos]=='>' && strexpression[pos+1]=='=')
    {
        f = 1;
        pos+=2;
        return;
    }
    if(pos<strlen(strexpression) && strexpression[pos]=='>')
    {
        f = 1;
        pos++;
        return;
    }
    if(pos<strlen(strexpression) && strexpression[pos]=='<')
    {
        f = 1;
        pos++;
        return;
    }
    f = 0;
    return;
}

void asgn_stat()
{
    ID();
    if(f==1)
    {
        if(pos<strlen(strexpression) && strexpression[pos]=='=')
        {
            pos++;
            expn();
            return;

        }
        f = 0;
        return;
    }
    return;
}

void loop_stat()
{
    int temp = pos;
    if(pos+5<strlen(strexpression) && strexpression[pos]=='w' && strexpression[pos+1]=='h' && strexpression[pos+2]=='i' && strexpression[pos+3]=='l' && strexpression[pos+4]=='e' && strexpression[pos+5]=='(')
    {
        pos+= 6;
        expn();
        if(f==1 && pos<strlen(strexpression) && strexpression[pos]==')')
        {
            pos++;
            stat();
            if(f==1)
            {
                return;
            }
        }

    }
    pos = temp;
    if(pos+3<strlen(strexpression) && strexpression[pos]=='f' && strexpression[pos+1]=='o' && strexpression[pos+2]=='r' && strexpression[pos+3]=='(')
    {
        pos+=4;
        asgn_stat();
        if(f==1)
        {
            if(pos<strlen(strexpression) && strexpression[pos]==';')
            {
                pos++;
                expn();
                if(f==1)
                {
                    if(pos<strlen(strexpression) && strexpression[pos]==';')
                    {
                        pos++;
                        asgn_stat();
                        if(pos<strlen(strexpression) && strexpression[pos]==')')
                        {
                            pos++;
                            stat();
                            if(f==1)
                                return;
                        }
                    }
                }
            }
        }

    }
    f=  0;
    return;
}

void dscn_stat()
{
    if(pos+2<strlen(strexpression) && strexpression[pos]=='i' && strexpression[pos+1]=='f' && strexpression[pos+2]=='(')
    {
        pos+=3;
        expn();
        if(f==1)
        {
            if(pos<strlen(strexpression) && strexpression[pos]==')')
            {
                pos++;
                stat();
                if(f==1)
                {
                    extn1();
                    return;
                }
                else
                {
                    return;
                }
            }
            else
            {
                f = 0;
                return;
            }
        }
        else
        {
            return;
        }
    }
    f = 0;
    return;
}

void stat()
{
    int temp  =pos;
    asgn_stat();
    if(f==1)
        return;
    pos = temp;
    dscn_stat();
    if(f==1)
        return;
    pos=temp;
    loop_stat();
    if(f==1)
        return;
    f = 0;
    return;
}
int main()
{

    char a,input,size=0;
    char file[10000],arr[10000];

    printf("Enter the code:\n");

    /*
    f1=fopen("input.txt","w");



    while(1)
    {
        scanf("%c",&input);
        if(input=='`')
            break;
        fputc(input,f1);
    }


    fclose(f1);

    */

    f1=fopen("input.txt","r");
    f2=fopen("result.txt","w");


    //assignment 1 -------------------------------------------

    printf("ASSIGNMENT 1:\n\n");


    while((a=fgetc(f1))!=EOF)
    {
        if(a=='\n')
        {
            size++;
        }

        if(a=='/')
        {

            char c,d,e,f;
            if((c=fgetc(f1))=='/')
            {
                while((c=fgetc(f1))!=EOF)
                {
                    if(c=='\n')

                    {
                        size++;
                        break;
                    }
                }

            }
            else if(c=='*')
            {
                while((c=fgetc(f1))!=EOF)
                {
                    if(c=='\n')
                    {
                        size++;
                    }
                    if(c=='*')
                    {
                        d=fgetc(f1);
                        if(d=='/')
                        {
                            size++;
                            break;
                        }
                    }
                }




            }
            else
            {
                fputc(a,f2);
                fputc(c,f2);
                printf("%c",a);
                printf("%c",c);

            }



        }
        else if(a=='\n')
        {
            continue;
        }
        else
        {
            fputc(a,f2);
            printf("%c",a);
        }


    }
    printf("\n\n");

    //printf("%d",size);
    fclose(f1);
    fclose(f2);


    //assignmet 2&3-----------------------------------

    f5=fopen("result.txt","r");
    f6=fopen("seplexeme.txt","w");

    while((a=fgetc(f5))!=EOF)
    {

        if(a == '+' || a == '*' ||
                a == '/' || a == ',' || a == ';' || a == '>' ||
                a == '<' || a == '=' || a == '(' || a == ')' ||
                a == '[' || a == ']' || a == '{' || a == '}')
        {
            fputc(' ',f6);
            fputc(a,f6);
            fputc(' ',f6);
        }
        else
        {
            fputc(a,f6);
        }


    }


    fclose(f5);
    fclose(f6);

    f3=fopen("seplexeme.txt","r");

    int store=0;

    while((a=fgetc(f3))!=EOF)
    {
        file[store++]=a;

    }

    fclose(f3);
    int k=0;

    for(int i=0; file[i]!='\0'; i++)
    {

        if(file[i]==' ' && file[i+1]==' ')
            continue;
        else
            arr[k++]=file[i];


    }
    f4=fopen("seplexeme2.txt","w");

    for(int i=0; file[i]!='\n'; i++)
    {
        fprintf(f4,"%c",arr[i]);
    }

    fclose(f4);

    f6=fopen("seplexeme2.txt","r");
    f4=fopen("tokenized.txt","w");

    printf("ASSIGNMENT 2:\n\n");

    char string[100000];
    int y=0;
    while((a=fgetc(f6))!=EOF)
    {
        if(a!=' ')
        {
            string[y++]=a;
        }
        else
        {
            string[y]='\0';
            y=0;


            if(isKey(string))
            {
                printf("[kw %s]",string);
                fprintf(f4, "[kw %s] ", string);
            }
            else if(isId(string)==1)
            {
                printf("[Id %s]",string);
                fprintf(f4, "[Id %s] ", string);

            }
            else if(isNum(string)==1)
            {
                printf("[Num %s]",string);
                fprintf(f4, "[Num %s] ", string);

            }
            else if(isPar(string)==true)
            {
                printf("[par %s]",string);
                fprintf(f4, "[par %s] ", string);

            }
            else if(isOperator(string)==true)
            {
                printf("[op %s]",string);
                fprintf(f4, "[op %s] ", string);
            }
            else if(isSep(string)==true)
            {
                printf("[sp %s]",string);
                fprintf(f4, "[sp %s] ", string);
            }
            else
            {
                printf("[unkn %s]",string);
                fprintf(f4, "[unkn %s] ", string);

            }

        }

    }
    printf("\n\n");


//[kw float] [id x1] [op =] [num 3.125] [sep ;] [kw double] [id f1] [par (] [kw int] [id x] [par )] [brc
//{] [kw double] [id z] [sep ;] [id z] [op =] [num 0.01] [sep ;] [kw return] [id z] [sep ;] [brc }] [kw
//int] [id main] [par (] [kw void] [par )] [brc {] [kw int] [id n1] [sep ;] [kw double] [id z] [sep ;] [id
//n1] [op =] [num 25] [sep ;] [id z] [op =] [id f1] [par (] [id n1] [par )] [sep ;]


    fclose(f6);
    fclose(f4);

    char c,str[1000000],*t;
    f1 = fopen("tokenized.txt", "r");
    f2 = fopen("idpair.txt", "w");

    fscanf(f1,"%[^\n]",str);

    t= strtok(str," ");

    printf("ASSIGNMENT 3:\n\n");

    printf("step 1: \n\n");
    while(t != NULL)
    {

        if(strcmp(t,"[kw") ==0|| strcmp(t,"[op")==0 ||strcmp(t,"[Num")==0 || strcmp(t,"[sp")==0 ||
                strcmp(t,"[unkn")==0 || strcmp(t,"[par")==0 )
        {
            printf("[");
            fputc('[',f2);
        }
        else
        {
            printf("%s",t);
            fprintf(f2,"%s",t);

            if(strcmp(t,"[Id") ==0)
            {
                printf(" ");
                fputc(' ',f2);
            }

        }
        t= strtok(NULL," ");


    }
    fclose(f1);
    fclose(f2);

    printf("\n\n");

    printf("step 2 - Table: \n\n");

    insert();
    display();


    printf("step 4 - Replacing: \n\n");

    f1 = fopen("syntexAnalysis.txt", "w");

    xy=0;
    //[float][id x1][=][3.125][;][double][id f1][(][int][id x][)][{][double][id z][;][id z][=][0.01][;][return][id z]
//[;][}][int][id main][(][void][)][{][int][id n1][;][double][id z][;][id n1][=][25][;][id z][=][id f1][(][id n1][)][;]
    for(int i=0; i<strlen(b); i++)
    {
        if(b[i]=='[')
        {
            printf("%c",b[i]);
            fputc(b[i],f1);
        }

        else
        {
            while(b[i]!=']' && b[i]!=' ')
            {
                printf("%c",b[i]);
                fputc(b[i],f1);
                i++;
            }
            if(b[i]==' ')
            {
                printf("%c",b[i]);
                fputc(b[i],f1);
                i++;

                while(b[i]!=']')
                {
                    i++;
                }

                printf("%d",swap[xy]+1);
                fputc(swap[xy]+1+48,f1);
                xy++;





            }
            printf("%c",b[i]);
            fputc(b[i],f1);


        }


    }





    fclose(f1);

    //assignment 4------------------------



    printf("\n\nASSIGNMENT 4:\n\n");

    printf("\n\n");

    printf("step 1 - Intermediate Output: \n\n");


    detecting_error();


    printf("\n\n");

    printf("step 2 - Errors: \n\n");


    f1 = fopen("error.txt", "r");


    while((a=fgetc(f1))!=EOF)
    {
        printf("%c",a);
    }

// Assignment 5
    printf("\nEnter an Expression\n");
    scanf("%s",strexpression);
    init();
    A();
    printf("1 = ");
    if(f  && pos==strlen(strexpression))
        printf("String matched\n");
    else printf("String not matched\n");


    init();
    Exp();
    printf("2 = ");
    if(f==1 && pos==strlen(strexpression))
        printf("String matched\n");
    else printf("String not matched\n");



    init();
    stat();
    printf("3 = ");
    if(f==1 && pos==strlen(strexpression))
        printf("String matched\n");
    else printf("String not matched\n");



    return 0;



}
