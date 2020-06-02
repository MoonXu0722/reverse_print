#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct keyvalue
{
	char value[50];
	int key;
	struct keyvalue *next;
}parameter;

void connectstring(char *string,struct keyvalue *p)
{
//	printf("key[connect] = %d\n",p->key);
	if (p->key == 1)
	{
		strcat(string,"(int) ");
		strcat(string,"%d\\n");
	}
	else if (p->key == 2)
	{
		strcat(string,"(double) ");
		strcat(string,"%g\\n");
	}
	else if (p->key == 3)
	{
		strcat(string,"(string) ");
		strcat(string,"%s\\n");
	}
}
int parse(struct keyvalue *p)
{
	int num_i = 0;
	int num_dot = 0;
	char *m = p->value;
	int len = strlen(m);
	while(*m != '\0')
	{
		if(*m == '\n' || *m == '\\')
		{
			*m = '\0';
			*m++ = 0;
			len --;
		}

		if(*m >= 48 && *m <= 57)
			num_i ++;
		else if(*m == 46)
			num_dot ++;
		m ++;	
	}
	if(num_i == len)
		p->key = 1;
	else if(num_i + 1 == len && num_dot == 1)
		p->key = 2;
	else
		p->key = 3;
	//printf("len = %d num_i = %d\n",len,num_i);

	return 0;
}

int main(int argc,char **argv)
{
	char sentence[2][256] = {"#include <stdio.h>\nint main()\n{\n","return 0;\n}"};
	char command[1024];
	char buf[1024];
	struct keyvalue *tmp;
	char prefix[50];
	char result[4096];
	char *pointer;
	if(argc != 2)
	{
		printf("usage:xpy [*].ly\n");
		return -1;
	}
	
	FILE *fp = fopen(argv[1],"r");
	if(!fp)
		return -2;
	//int r = fscanf(fp,"%s",buf);
	int r = fread(buf,1,1024,fp);
	fclose(fp);
	if(r <= 0)
	{
		//printf("empty file\n");
		return -3;
	}
	//printf("buf0 = %s\n",buf);
	struct keyvalue *cur,*last,*new;
	
	cur = &parameter;	
	char *p = strtok(buf, " ");
	strcpy(cur->value, p);
	if (!strcmp(cur->value,"print"))
		strcat(command,"printf(\"");
	int i = 0;
	while(1)
	{
		p = strtok(NULL," ");
		if(!p || *p == '\n')
			break;
		//printf("^^^^%d\n",*p);
		//printf("^^^^%d\n",*(p++));
		//printf("enter while\n");
		//printf("##%s\n",p);
		cur->next = (struct keyvalue *)malloc(sizeof(struct keyvalue));
		i ++;
		cur = cur->next;
		cur->next = NULL;
		strcpy(cur->value, p);
		parse(cur);
		//printf("1\n");
		connectstring(command,cur);
		//printf("cur->value = %s cur->key = %d\n",cur->value,cur->key);	
	}
	//printf("i = %d\n",i);
	strcat(command,"\"");
	cur = parameter.next;
	//printf("##value = %s\n",parameter.next->value);
	int k = 0;
	//printf("command = %s\n",command);
	//printf("$$ = %p\n",parameter.next->next->next->next);
	while (cur)
	{
		k++;
		strcat(command,",");
		if(cur->key == 3)
			strcat(command,"\"");
		strcat(command,cur->value);
		if(cur->key == 3)
			strcat(command,"\"");
		tmp = cur->next;
		free(cur);
		cur = tmp;
	}
	//printf("command = %s\n",command);
	//printf("k = %d\n",k);
	strcat(command,");");
	strcat(result,sentence[0]);
	strcat(result,"\t");
	strcat(result,command);
	strcat(result,"\n\t");
	strcat(result,sentence[1]);
	result[strlen(result)] = 0;
	FILE *fp2 = fopen("ly.c","w");
	if(!fp2)
		return -4;
	fwrite(result,1,strlen(result) + 1,fp2);
	fclose(fp2);
	system("gcc ly.c -o ly");
	system("./ly");
	return 0;
}
