#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct DataFormat
{
	int inter;
	char *str;
}typedef DF;

struct Node
{
	struct DataFormat *data;
	struct Node *next;
	struct Node *prev;
}typedef node;

struct Cmd 
{
	char *cmd;	
	int cmd_len;
	void (*ptr)(node **, char *);

}typedef cmd;

void add(node **current_node , char *data);
void del(node **current_node , char *data);
void show(node **hand_node , char *data);

void save(node **hand_node_node , char *file_name);
void load(node **current_node , char *file_name);

void write_node(node **current , char *data);
void init();
int process_data(node **current , char *data);

node *hand_node , *current_node;
 
//Init Command 
cmd Command[6] = {{"add" , 3 , add}
				 ,{"del" , 3 , del}
				 ,{"show" , 4 , show}
				 ,{"save" , 4 , save}
				 ,{"load" , 4 , load}
				 ,{NULL , 0 , NULL}};
				 
int main()
{
	printf("Please input the command and data\n");
	printf("The command format is \"cmd integer , string\"\n");
	printf("Data can input integer and string\n\n\n");

	char *input;
	int command_count = 0;

	init();

	while(1)
	{
		printf("input : ");
		input = (char *)malloc(128);
		memset(input , '\0' , 128);
		fgets(input , 128 , stdin);	
		*strstr(input , "\n") = '\0';

		if(strncmp(input,"exit", 4) == 0)
		{
			printf("bye~~\n");	
			break;
		}
		while(Command[command_count].cmd)
		{
			if(strncmp(input , Command[command_count].cmd , Command[command_count].cmd_len) == 0)
			{
				Command[command_count].ptr(&current_node , input + Command[command_count].cmd_len + 1);
				break;
			}
			command_count++;
		}
		if(command_count == 5)	
		{
			printf("Command don't exist\n");	
		}
		printf("\n\n");
		command_count = 0;
		free(input);
	}
	
	return 0;
}

//Init hand_node
void init()
{
	hand_node = (node *)malloc(sizeof(node));
	hand_node -> data = (DF*)malloc(sizeof(DF)); 	
	hand_node -> data -> str = (char *)malloc(128);
	memset(hand_node -> data -> str , '\0' , 128);

	hand_node -> next = NULL;	
	hand_node -> prev = hand_node;

	current_node = hand_node;
}

void add(node **current , char *data)
{
	int data_flag = 0;

	//Create the new node
	node *new_node = (node *)malloc(sizeof(node));
	new_node -> data = (DF *)malloc(sizeof(DF));

	//New node data init 
	new_node -> next = NULL;
	data_flag = process_data(current , data);
	
	if(data_flag != 0)
	{
		return;	
	}
	
	write_node(current , data);
	//Link node to list
	(*current) -> next = new_node;
	new_node -> prev = (*current);
	(*current) = new_node;
}

//show list content
void show(node **Hand_node , char *data)
{
	node *cur = hand_node;

	while(cur -> next != NULL)
	{
		printf("%s , %d\n",cur -> data -> str , cur -> data -> inter);
		cur = cur -> next;	
	}
}

//del only delet node
//Only delet one parameter in each one;
void del(node **Hand_node , char *data)
{
	node *cur = hand_node;

	while(cur -> next != NULL)
	{
		if(strncmp(cur->data->str , data , strlen(data)) == 0 && strlen(cur->data->str) == strlen(data))
		{
			memset(cur->data->str , '\0' , strlen(cur->data->str));
			break;
		}

		if(cur->data->inter == atoi(data) && cur->data->inter != 0)
		{
			cur->data->inter = 0;	
			break;
		}

		cur = cur -> next;	
	}
	
	if(cur -> next == NULL) 
	{
		printf("This data don't exist");	
		return;
	}

	if(strlen(cur->data->str) == 0 && cur->data->inter == 0)
	{
		node *temp = cur;
		if(cur == hand_node)
		{
			hand_node = hand_node -> next;	
			if(hand_node->next == NULL)
			{
				return;
			}
		}

		cur->prev->next = cur->next;
		free(temp);	
	}
}

//save 
//save data of data_file
//save data of char* length of index file 
void save(node **Hand_node , char *file_name)
{
	node *cur = hand_node;
	int str_len;

	FILE *fp;
	FILE *index = fopen("index" , "w");
	
	if((fp = fopen(file_name , "wb")) == NULL)	
	{
		printf("The file is not exist");
		return;	
	}
		
	while(cur -> next != NULL)
	{
		//Stored char* data length in index file
		str_len = strlen(cur->data->str);
		fwrite(&str_len , sizeof(int) , 1 , index);
		fwrite("\n" , sizeof(char) , 1 ,index);

		fwrite(cur->data->str , sizeof(char) , strlen(cur->data->str) , fp);	
		fwrite("," , sizeof(char) , 1 , fp);
		fwrite(&cur->data->inter , sizeof(int) , 1 , fp);
		fwrite("\n" , sizeof(char) , 1 ,fp);
		
		cur = cur -> next;	
	}
	
	fclose(index);
	fclose(fp);
}	

//load data from data_file
//The data format in the file is fixed
void load(node **current_node , char *file_name)
{
	int str_len = 0 , count = 0 , index_file_size = 0;	

	FILE *index = fopen("index" , "r");
	FILE *fp;
	
	if((fp = fopen(file_name , "r")) == NULL)	
	{
		printf("The file is not exist");
		return;	
	}
	fseek(index , 0 , SEEK_END);
	index_file_size = ftell(index);
	fseek(index , 0 , SEEK_SET);

	while(index_file_size != count)
	{
		//Read each one data's length inside index file for str_len.
		fread(&str_len , sizeof(int) , 1 , index);
		fseek(index , 1 , SEEK_CUR);
			
		//Create a new_node for linlisted
		node *new_node = (node *)malloc(sizeof(node));			
		new_node -> data = (DF *)malloc(sizeof(DF));

		(*current_node) -> data -> str = (char *)malloc(str_len + 1);
		memset((*current_node) -> data -> str , '\0' , str_len + 1);

		//Since data_file read data for new_node
		fread((*current_node) -> data -> str , sizeof(char) , str_len, fp);
		fseek(fp , 1 , SEEK_CUR);	
		fread(&(*current_node) -> data -> inter , sizeof(int) , 1 , fp);
		fseek(fp , 1 , SEEK_CUR);
		
		new_node -> next = NULL;
		(*current_node) -> next = new_node;
		new_node -> prev = (*current_node);
		(*current_node) = new_node;

		count += 5;
	}

	fclose(index);
	fclose(fp);
}

int process_data(node **current , char *data)
{
	int dot_count = 0 , fm1_flag = 0 , fm2_flag = 0;
	char *dot , *ck_fm1;
	
	dot    = strstr(data , ",");
	ck_fm1 = data;

	if((dot == data+strlen(data)-1) || dot == data)
	{
		printf("Formet is flust\n");
		return 1;
	}

	while(ck_fm1 != data + strlen(data) && dot != NULL)
	{
		if(*ck_fm1 == ',')
		{
			ck_fm1++;	
		}

		if(!((*ck_fm1 > 47) && (*ck_fm1 < 59)))	
		{
			if(ck_fm1 < dot)
			{
				fm1_flag = 1;		
			}
			else if(ck_fm1 > dot)
			{
				fm2_flag = 1;
			}
		}
		ck_fm1++;
	}	

	if(dot != NULL)
	{
		if(fm2_flag == fm1_flag )	
		{
			printf("Formet is flust\n");
			return 1;	
		}

		while(strstr(dot , ","))	
		{	
			*dot = '\0';
			dot_count++;
			dot++;
		}
	}

	if(dot_count > 1)
	{
		printf("Too many parameter");
		return 1;	
	}

	return 0;
}
//-----------------------------------------------	

void write_node(node **current , char *data) 
{	
	int char_flag = 0;
	char *sp , *ep , *move_p;

	sp = data; 

	//write
	while(*sp != '\0')
	{
		ep = sp + strlen(sp); 

		move_p = sp;

		while(move_p != ep)
		{
			if(!((*move_p > 47) && (*move_p < 59)))	
			{
				char_flag++;	
				break;
			}
			move_p++;
		}

		if(char_flag != 0)	
		{
			(*current) -> data -> str = (char *)malloc(strlen(sp) + 1);
			memset((*current) -> data -> str , '\0' , strlen(sp) + 1);
			strncpy((*current) -> data -> str , sp , strlen(sp));
		}
		else
		{
			(*current) -> data -> inter = atoi(sp);	
		}
		
		char_flag = 0;
		sp = ep + 1;
	}
	return; 
}
