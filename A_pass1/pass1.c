    #include"stdlib.h"
    #include"string.h"
    #include"stdio.h"

    struct list
    {
      char label[100],opcode[100],operand[100];
      int c;
    };
    struct s
    {
      char *string;
    };
    struct optab
    {
        char code[100],val[100];

    };
    struct symtab
    {
        char label[100];
        int addr;

    };
    int tc,stc=0,optc=0;
    int locctr,prgrmlen,start;
    struct list l[200];
    struct s temp[10];
    struct symtab st[200];
    struct optab op[100];
    void readinput();
    void runalgo();
    void readoptab();
    int checklabel(char lab[ ]);
    int checkopcode(char opp[]);

    void main()
    {

        readinput();
        readoptab();
        runalgo();
        int q=0;
        printf("Progam Length is %X",prgrmlen);

    }

    void readinput()
    {
        FILE *fp1,*fp2;
        char word[1000],d[]=" \t\n\r\v";
        int count;
        fp2=fopen("optab.txt","r");
        fp1=fopen("sic_in.txt","r");
        if(fp1 == NULL)
            printf("Error\n");

        if(fp2 == NULL)
            printf("Error\n");



        while(fgets(word,100,fp1)!= NULL)
        {

            tc++;
            int count=0;
            char * token = strtok(word,d);
            while(token != NULL)
            {
                count++;
                temp[count].string=token;
                token = strtok(NULL,d);
            }
            if(count == 3)
            {
                strcpy(l[tc].label,temp[1].string);
                strcpy(l[tc].opcode,temp[2].string);
                strcpy(l[tc].operand,temp[3].string);
            }
            else  if(count == 2)
            {

                strcpy(l[tc].opcode,temp[1].string);
                strcpy(l[tc].operand,temp[2].string);
            }
            else if(count == 1)
            {

                strcpy(l[tc].opcode,temp[1].string);
            }
            l[tc].c=count;
        }


    }
    void readoptab()
    {
        FILE *fp2;
        char word[100],d[]=" \t\n\r\v";
        fp2=fopen("optab.txt","r");
        int count;
        while( fgets(word,10,fp2)!= NULL)
        {
            count=0;
            char * token = strtok(word,d);
            while(token != NULL)
            {
                count++;

                temp[count].string=token;

                token = strtok(NULL,d);
            }
            if(count == 2)
            {
               strcpy(op[optc].code,temp[1].string);
                strcpy(op[optc].val,temp[2].string);
                optc++;
            }


        }

    }
    void runalgo()
    {
        int i;

        FILE *fp2,*fp3;
        fp2=fopen("symtab.txt","w");
        fp3=fopen("output.txt","w");
        char a[100];
        for(i=1; i<=tc; i++)
        {   char check;
            if(l[i].label[0] == '.' || l[i].opcode[0] == '.' || l[i].operand[0] == '.')
            {
                printf("Comment Line at line no %d\n",i);
                continue;

            }

            if(!strcmp(l[i].opcode,"START"))
            {
                strcpy(a,l[i].operand);
                sscanf(a,"%X",&locctr);
                start=locctr;


                fprintf(fp3,"%X\t%s\t%s\t%s\n",locctr,l[i].label,l[i].opcode,l[i].operand);
                continue;

            }
            if(strcmp(l[i].opcode,"END"))//run if not END
            {
                int j,flag;
                if(l[i].c == 3 )
                {
                    int c=stc;
                    flag=0;
                    flag=checklabel(l[i].label);
                    if(flag==0)
                    {
                        fprintf(fp2,"%s\t%X\n",l[i].label,locctr);
                        strcpy(st[stc].label,l[i].label);
                        st[stc].addr=locctr;
                        stc++;
                    }
                    if (flag == 1)
                        continue;
                }

            fprintf(fp3,"%X\t%s\t%s\t%s\n",locctr,l[i].label,l[i].opcode,l[i].operand);
                int value;
                flag=0;
                if(!strcmp(l[i].opcode,"RESW") || !strcmp(l[i].opcode,"RESB") || !strcmp(l[i].opcode,"BYTE") || !strcmp(l[i].opcode,"WORD"))
                {

                if(!strcmp(l[i].opcode,"RESB"))
                {

                    value=atoi(l[i].operand);
                    locctr=locctr+value;

                }
                else if(!strcmp(l[i].opcode,"BYTE"))
                {

                    if(l[i].operand[0]=='C')
                    {

                        value=strlen(l[i].operand);
                        locctr=locctr+(value-3);
                    }
                    if(l[i].operand[0]=='X')
                    {

                        value=strlen(l[i].operand);
                        value=value-3;
                        locctr=locctr+(value/2);
                    }

                }
                else if(!strcmp(l[i].opcode,"WORD"))
                {

                    locctr=locctr+3;

                }
                else if(!strcmp(l[i].opcode,"RESW"))
                {

                    value=atoi(l[i].operand);
                    locctr=locctr+(value*3);


                }

                }
                else
                {
                    flag=0;
                    flag=checkopcode(l[i].opcode);
                    if(flag == 0)
                      continue;
                }

            }


        }fprintf(fp3,"%X\t%s\tEND\t%s\n",locctr,l[i].label,l[i].operand);

        prgrmlen=locctr-start;

    }

    int checklabel(char lab[100])
    {
        int j,flag=0;
        for(j=0;j<stc;j++)
        {
            if(!strcmp(st[j].label,lab))
            {
                flag=1;

            }

        }
        if (flag ==1)
        {
            printf("Duplicate Label %s\n",lab);
            return 1;
        }
        else return 0;
    }
    int checkopcode(char opp[100])
    {
        int j,flag=0;
        for(j=0;j<optc;j++)
        {
            if(!strcmp(opp,op[j].code))
                {
                    locctr+=3;
                    flag=1;
                    break;
                }

        }
        if(flag == 0)
        {
            printf("Invalid opcode %s\n",opp);
            return 0;
        }
        else return 1;

    }

