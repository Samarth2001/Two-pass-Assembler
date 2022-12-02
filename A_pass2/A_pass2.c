#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct inter
{
    char addr[100],label[100],opcode[100],operand[100],objc[100];
    int c;
};

struct symtab
{
    char label[100];
    char addr[100];
};

struct optab
{
    char code[100],val[100];
};

struct s
{
    char *string;
};

struct inst
{
    char oc[100];

};

void readinter();
void readsymtab();
void readoptab();
void genoprogram();
void pass2();
char * getoptab(char op[]);
char * getsymtab(char opr[]);

struct inter io[100];
struct symtab so[100];
struct optab oo[100];
struct s sample[100];
struct inst inso[100];

int stc=0,optc=0,ic=0,inc=1;
char d[]=" \n\t\r\v";
int eaddr,locctr;

void main()
{
    readinter();
    readoptab();
    readsymtab();
    pass2();
    for(int j=1; j<=ic ; j++ )
    {
        printf("%s\t%s\t%s\t%s\t%s\t\n",io[j].addr,io[j].label,io[j].opcode,io[j].operand,io[j].objc);
    }
    genoprogram();
}
void readinter()
{
    FILE *fp;
    char word[100];
    fp=fopen("intermediate.txt","r");
    int count;
    while(fgets(word,100,fp)!= NULL)
    {
        ic++;
        int count=0;
        char *token = strtok(word,d);
        while(token != NULL)
        {
            count++;
            sample[count].string = token;
            token=strtok(NULL,d);
        }
        if( count == 4)
        {
            strcpy(io[ic].label,sample[2].string);
            strcpy(io[ic].addr,sample[1].string);
            strcpy(io[ic].opcode,sample[3].string);
            strcpy(io[ic].operand,sample[4].string);
        }
        if( count == 3)
        {
            strcpy(io[ic].addr,sample[1].string);
            strcpy(io[ic].opcode,sample[2].string);
            strcpy(io[ic].operand,sample[3].string);
        }
        if( count == 2)
        {
            strcpy(io[ic].addr,sample[1].string);
            strcpy(io[ic].opcode,sample[2].string);
        }
    }


}

void readoptab()
{
    FILE *fp;
    fp=fopen("optab.txt","r");
    char word[100];
    int count;
    while(fgets(word,100,fp)!= NULL)
    {

        int count=0;
        char *token = strtok(word,d);
        while(token != NULL)
        {
            count++;
            sample[count].string = token;
            token=strtok(NULL,d);
        }
        if( count == 2)
        {
            optc++;
            strcpy(oo[optc].code,sample[1].string);
            strcpy(oo[optc].val,sample[2].string);

        }
    }
}
void readsymtab()
{
    FILE *fp;
    fp=fopen("SYMTAB.txt","r");
    char word[100];
    int count;
    while(fgets(word,100,fp)!= NULL)
    {

        int count=0;
        char *token = strtok(word,d);
        while(token != NULL)
        {
            count++;
            sample[count].string = token;
            token=strtok(NULL,d);
        }
        if( count == 2)
        {
            stc++;
            strcpy(so[stc].label,sample[1].string);
            strcpy(so[stc].addr,sample[2].string);

        }
    }
}

void pass2()
{
    FILE *fp1,*fp2,*fp3,*fp4;
    fp1=fopen("ll.txt","w");
    fp2=fopen("objcode.txt","w");
    fp3=fopen("ooo.txt","w");
    int opaddr,length,start,base;
    char integer[100];
    fp4=fopen("length.txt","r");
    fscanf(fp4, "%d", &length);
    for (int i=1 ; i<ic; i++)
    {
        if(io[i].label[0] == '.' || io[i].opcode[0] == '.' || io[i].operand[0] == '.')
        {
            printf("Comment Line at line no %d\n",i);
            continue;

        }
        if(!(strcmp(io[i].opcode,"START")))
        {
            start = (int)strtol(io[1].operand,NULL,16);
            fprintf(fp1,"%s\t%s\t%s\n",io[i].label,io[i].opcode,io[i].operand);
            fprintf(fp2,"H^%s^00%x^00%x\n",io[i].label,start,length);
//            fprintf(fp2,"T^%06x^",start);
        }
        else if(!strcmp(io[i].opcode,"BASE"))
        {
            strcpy(opaddr,getsymtab(io[i].operand));
            base=(int)strtol(opaddr,NULL,16);
        }
        else if(strcmp(io[i].opcode,"END"))
        {
            //printf("Value of %s is %s\n",io[i].opcode,getoptab(io[i].opcode));
            char o[100],opaddr[100],oc[100];
            eaddr=(int)strtol(io[i+1].addr,NULL,16);
            strcpy(o,getoptab(io[i].opcode));//search optab for opcode
            if(strcmp(o,"0000"))
            {
                //printf("%s\n",o);
                if(strcmp(io[i].operand,"")) //if there is a symbol in OPERAND
                {
                    //printf("address of %s is %s\n",io[i].operand,getsymtab(io[i].operand));
                    strcpy(opaddr,getsymtab(io[i].operand)); //search symtab

                    if(!strcmp(opaddr,"0000"))
                        printf("Undefined symbol at line %d\n",i);//set error flag

                }//end of if symbol
                else
                {
                    strcpy(opaddr,"0000");

                }
                strcpy(oc,o);
                strcat(oc,opaddr);
                //printf("Oc is %s\n",oc);
                strcpy(io[i].objc,oc);
                fprintf(fp3,"%s\t%s\t%s\t%s\t%s\n",io[i].addr,io[i].label,io[i].opcode,io[i].operand,oc);

            }//end of opcode found
            else
            {
                if(!strcmp(io[i].opcode,"BYTE") || !strcmp(io[i].opcode,"WORD"))
                {
                    if(!strcmp(io[i].opcode,"WORD"))
                    {
                        strcpy(oc,"");
                        sprintf(integer,"%06x",atoi(io[i].operand));
                        strcpy(oc,integer);
                    }
                    else if(!strcmp(io[i].opcode,"BYTE"))
                    {
                        if(io[i].operand[0] == 'C')
                        {
                            strcpy(oc,"");
                            for(int k=2; k< strlen(io[i].operand)-1; k++)
                            {

                                sprintf(integer,"%x",io[i].operand[k]);
                                strcat(oc,integer);
                            }
                        }
                        if(io[i].operand[0] == 'X')
                        {
                            strcpy(oc,"");
                            for(int k=2; k< strlen(io[i].operand)-1; k++)
                            {

                                sprintf(integer,"%c",io[i].operand[k]);
                                strcat(oc,integer);
                            }
                        }



                    }
                    //printf("Oc is %s\n",oc);
                    strcpy(io[i].objc,oc);
                    fprintf(fp3,"%s\t%s\t%s\t%s\t%s\n",io[i].addr,io[i].label,io[i].opcode,io[i].operand,oc);

                }
                else
                {

                    fprintf(fp3,"%s\t%s\t%s\t%s\t\t\n",io[i].addr,io[i].label,io[i].opcode,io[i].operand);
                }
            }

        }



    }
}
void genoprogram()
{
    FILE *fp1,*fp4;
    fp1=fopen("end.txt","w");
    char saddr[1000];
    int length,starting,eaddr,tlen;
    locctr = (int)strtol(io[1].addr,NULL,16);
//    eaddr = (int)strtol(io[ic].addr,NULL,16);
    fp4=fopen("length.txt","r");
    fscanf(fp4, "%d", &length);
    starting=locctr;
    for(int i=1; i<=ic; i++)
    {
        if(strcmp(io[i].opcode,"END"))
        {
            if(!strcmp(io[i].opcode,"START"))
            {
                printf("Starting %X\n",locctr);
                printf("End %X\n",length);
                fprintf(fp1,"H^%06s^%06x^%06x\n",io[i].label,locctr,length);
                printf("H^%06s^%06x^%06x\n",io[i].label,locctr,length);
//                fprintf(fp1,"T^%06x",locctr);
            }
            else
            {
                eaddr= (int)strtol(io[i+1].addr,NULL,16);
                tlen=eaddr-starting;
                if(tlen<=30)
                {
                    if(strcmp(io[i].objc,""))
                    {
                        strcat(saddr,"^");
                        strcat(saddr,io[i].objc);
                    }
                    else
                    {
                        if(strcmp(saddr,""))
                        {
                            fprintf(fp1,"T^%06x",starting);
                            printf("T^%06x",starting);
                            tlen=(int)strtol(io[i].addr,NULL,16)-starting;
                            fprintf(fp1,"^%x%s\n",tlen,saddr);
                            printf("^%x%s\n",tlen,saddr);
                            strcpy(saddr,"");
//                            strcat(saddr,"^");
//                            strcat(saddr,io[i].objc);
                            starting=(int)strtol(io[i].addr,NULL,16);
                        }
                        else
                        {
                            starting=(int)strtol(io[i].addr,NULL,16);
                        }
                    }
                }
                else
                {
                    if(strcmp(saddr,""))
                    {
                        fprintf(fp1,"T^%06x",starting);
                        printf("T^%06x",starting);
                        tlen=(int)strtol(io[i].addr,NULL,16)-starting;
                        fprintf(fp1,"^%x%s\n",tlen,saddr);
                        printf("^%x%s\n",tlen,saddr);
                        strcpy(saddr,"");
                        if(strcmp(io[i].objc,""))
                        {
                            strcat(saddr,"^");
                            strcat(saddr,io[i].objc);
                        }
                        starting=(int)strtol(io[i].addr,NULL,16);
                    }
                    else
                    {
                        starting=(int)strtol(io[i].addr,NULL,16);
                    }
                }
            }
        }
        else
        {
            fprintf(fp1,"E^%06x\n",locctr);
            printf("E^%06x\n",locctr);
        }

    }
}

char * getoptab(char op[100])
{
    int flag=0;
    for(int j=1; j<=optc ; j++ )
    {

        if(!strcmp(op,oo[j].code))
        {
            int addr=atoi(oo[j].val);
            return (oo[j].val);
            flag=1;
        }
    }
    if (flag==0)
    {

        return "0000";
    }

}
char * getsymtab(char opr[100])
{
    int flag=0,addr=0;


    //printf("In symtab get\n");
    for(int j=1; j<=stc ; j++ )
    {


        if(!strcmp(opr,so[j].label))
        {
            addr=atoi(so[j].addr);
            flag = 1;

            return so[j].addr;
            break;
        }
    }
    if(flag == 0)
        return "0000";

}

