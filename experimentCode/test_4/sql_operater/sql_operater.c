/*
 * sql_operater.c
 *
 *  Created on: 2016-11-28
 *      Author: THTF
 */

#include "sqlite3.h"
#include <stdio.h>
#include <string.h>



/************************************************************************
 *
 * 闄ゅ幓瀛楃涓插紑澶寸殑绌虹櫧绗�
 *
************************************************************************/

void ltrim(char *s){
    char *p;
    p = s;
    while(*p == ' ' || *p == '\t'){*(p++);}
    strcpy(s,p);
}

/************************************************************************
 *
 * 闄ゅ幓瀛楃涓叉湯灏剧殑绌虹櫧绗�
 *
************************************************************************/

static void rtrim(char *s){
    int i;
    i = strlen(s)-1;
    while((s[i] == ' ' || s[i] == '\t') && i >= 0){i--;};
    s[i+1] = '\0';
}


/************************************************************************
 *
 * 闄ゅ幓瀛楃涓插紑澶村拰鏈熬鐨勭┖鐧界
 *
************************************************************************/
void trim(char *s){
    ltrim(s);
    rtrim(s);
}

/************************************************************************
 *
 * 璇诲彇杈撳叆瀛楃
 *
************************************************************************/
void getString(char *arr) {
    char a = getchar();
    int i  = 0;
    while (a !='\n') {
        arr[i++] = a;
        a = getchar();
    }
    arr[i]='\0';
    trim(arr);
}

int stringCat(char * arr1, int len1, char *des, int from) {
	int i, j;
	j = from;
	for (i = 0; i < len1; ++i) {
		des[j++] = arr1[i];
	}
	des[j] = '\0';
	return j;
}




#define MAX_LEN 255

/**************************************************
 *
 * 鍗曡瘝缁撴瀯浣�
 *
***************************************************/
typedef struct word_Node {
    char word[MAX_LEN];
    struct word_Node *next;
    int w_len;
} word_Node, *word_LinkList;
/**************************************************
 *
 * 鍒涘缓涓�涓埄鐢ㄥ熬鎻掓硶鐨勫崟閾捐〃
 *
***************************************************/
void createWordLinkList(word_LinkList *wl) {
    (*wl) = (word_LinkList)malloc(sizeof(word_Node));
    if ((*wl) == NULL) {
        printf("LinkList creation failed!\n");
        exit(0);
    }
    (*wl)->next = NULL;
    (*wl)->w_len = 0;
}
/**************************************************
 * 鍔熻兘鍒╃敤灏炬彃娉曟彃鍏ユ暣涓崟璇�
 *
**************************************************/
void insert(word_LinkList l, const char *w, int len) {
    word_Node * node = (word_Node *) malloc(sizeof(word_Node));
    if (node == NULL) {
        printf("word_Node allocation failed!\n");
        exit(0);
    }
    strcpy(node->word, w);
    node->w_len = strlen(w);
    node->next = NULL;
    word_Node * prev = l;
    while (prev->next != NULL) {
        prev = prev->next;
    }
    prev->next = node;
}
/****************************************
 *
 * 娓呴櫎閾捐〃鎵�鏈夌殑鑺傜偣骞讹紝閲婃斁绌洪棿
 *
*****************************************/
void clear_WordLinkList(word_LinkList *l) {
    word_Node * next;
    word_Node * cur;
    next = (*l)->next;
    while (next != NULL) {
        cur = next;
        next = next->next;
        free(cur);
        cur = NULL;
    }
    (*l)->next = NULL;
}

/**************************************************
 *
 * 鍒犻櫎鏁翠釜閾捐〃
 *
***************************************************/
void delete_WordLinkList(word_LinkList *l) {
    if (*l != NULL) {
        clear_WordLinkList(l);
        free(*l);
        *l = NULL;
    }
}

/**************************************************
 *
 *璇诲彇涓�涓妭鐐圭殑鍗曡瘝锛屽苟杩斿洖涓嬩竴涓妭鐐圭殑鎸囬拡
 *
***************************************************/
word_Node * readWord(word_Node *cur, char arr[], int *len) {
    strcpy(arr, cur->word);
    *len = cur->w_len;
    return cur->next;
}
/******************************************
 *
 *鍒ゆ柇涓�涓瓧绗︽槸鍚﹀湪涓�涓瓧绗︿覆涓�
 *
 *******************************************/
int isInArray(char c, char *arr, int len) {
    int i;
    for (i = 0; i < len; ++i) {
        if (arr[i] == c) {
            return 1;
        }
    }
    return 0;
}

/**************************************************
 *
 * 杈撳嚭鏁翠釜閾捐〃璁板綍鐨勫崟璇�
 *
***************************************************/
void printWordLinkList(word_LinkList l) {
    word_Node * cur = l->next;
    while (cur != NULL) {
        printf("%s ", cur->word);
        cur = cur->next;
    }
    printf("\n");
}

/**************************************************
 *
 * 杩斿洖鏁翠釜閾捐〃鐨勯暱搴�
 *
***************************************************/
int len_WordLinkList(word_LinkList l) {
    word_Node * cur = l->next;
    int len = 0;
    while (cur != NULL) {
        ++len;
        cur = cur->next;
    }
    return len;
}

/**************************************************
 *
 * 璇诲叆涓�涓瓧绗︿覆锛屽苟灏嗗崟璇嶅垎绂伙紝鎻掑叆鍗曢摼琛�
 *
***************************************************/
void readString(word_LinkList l, char *arr, int len_arr) {
    int i;
    char specialCS[10] = ",\"?;'!";
    char word[MAX_LEN];
    int j = 0;
    for (i = 0; i < len_arr; ++i) {
        if (isInArray(arr[i], specialCS, 6) == 1) {
            if (j != 0) {
                // 娣诲姞鍗曡瘝
                word[j] = '\0';
                insert(l, word, j);
                // 娣诲姞鏍囩偣绗﹀彿
                j = 0;
            }
            // 娣诲姞鏍囩偣绗﹀彿
            word[j++] = arr[i];
            word[j] = '\0';
            insert(l, word, j);
            j = 0;
        } else if (arr[i] == ' ') {
            if (j != 0) {
                word[j] = '\0';
                insert(l, word, j);
                j = 0;
            }
        } else {
            word[j++] = arr[i];
            if (i == strlen(arr) - 1) {
                word[j] = '\0';
                insert(l, word, j);
            }
        }
    }
}













void sqlInsert (sqlite3 *db, const char *name, const char *id,  const char *age, const char *gender)
{
	//  "INSERT INTO TABLE_NAME VALUES (value1,value2,value3,...valueN)";
	char des[255] = "INSERT INTO stu_table VALUES ('";

	stringCat(name, strlen(name), des, strlen(des));
	stringCat("',", 2, des, strlen(des));
	stringCat(id, strlen(id), des, strlen(des));
	stringCat(",", 1, des, strlen(des));
	stringCat(age, strlen(age), des, strlen(des));
	stringCat(",'", 2, des, strlen(des));
	stringCat(gender, strlen(gender), des, strlen(des));
	stringCat("')", 2, des, strlen(des));
	char *errmsg;
	sqlite3_exec(db, des, 0, 0,
			&errmsg);
	/*sqlite3_exec(db, "insert into d_table(name) values ('demo2')", 0, 0,
			&errmsg);
	sqlite3_exec(db, "insert into d_table(name) values ('demo3')", 0, 0,
			&errmsg);*/
}
void sqlDelete (sqlite3 *db, const char * snd, const char * rd)
{
	char *errmsg;
	// "delete  from  d_table  where name = 'demo2'"
	char des[255] = "delete  from  stu_table  where ";
	stringCat(snd, strlen(snd), des, strlen(des));
	stringCat(" = ", 3, des, strlen(des));
	if (strcmp(snd, "AGE") == 0 || strcmp(snd, "ID") == 0) {
		stringCat(rd, strlen(rd), des, strlen(des));
	} else {
		stringCat("'", 1, des, strlen(des));
		stringCat(rd, strlen(rd), des, strlen(des));
		stringCat("'", 1, des, strlen(des));
	}
	sqlite3_exec(db, des, 0, 0,
				&errmsg);

}
static int dbInfo (void *para, int column, char **column_value, char
**column_name)
{
	int i;
	for (i = 0; i < column; ++i) {
		printf("%-10s",column_value[i]);
	}
	printf("\n");
	return (0);
}
void printHeader() {
	printf("Name      ID        AGE     GENDER\n");
}
void show(sqlite3 *db) {
	char *errmsg;
	printHeader();
	sqlite3_exec(db, "select * from stu_table", dbInfo, 0, &errmsg);
	printf("------------------------------------------\n");
}
void sqlSelect (sqlite3 *db, const char * snd, const char * rd)
{
	char *errmsg;
		// "delete  from  d_table  where name = 'demo2'"
		char des[255] = "select * from  stu_table  where ";
		stringCat(snd, strlen(snd), des, strlen(des));
		stringCat(" = ", 3, des, strlen(des));
		if (strcmp(snd, "AGE") == 0 || strcmp(snd, "ID") == 0) {
			stringCat(rd, strlen(rd), des, strlen(des));
		} else {
			stringCat("'", 1, des, strlen(des));
			stringCat(rd, strlen(rd), des, strlen(des));
			stringCat("'", 1, des, strlen(des));
		}
		printHeader();
	sqlite3_exec(db, des, dbInfo, 0, &errmsg);
	printf("------------------------------------------\n");
}
void sqlCreate (sqlite3 *db)
{
   char *errmsg;
   // "create table stu_table(ID integer primary key autoincrement,name nvarchar(32))"
   const char * s = "create table stu_table(NAME nvarchar(32), ID integer primary key, AGE integer, GENDER nvarchar(32))";
   sqlite3_exec(db, s, NULL,NULL, &errmsg);
}



/**
 *
 *
 * **/
void printHelp() {
    printf("show   - show all the students information\n");
    printf("insert - insert one student information as follow\n");
    printf("         insert NAME ID AGE GENDER\n");
    printf("delete - delete one students information as follow\n");
    printf("         delete NAME(xx) ID(xx) AGE(xx) GENDER(xx)\n");
    printf("search - search students information as follow\n");
    printf("         search NAME XX or ID xx or AGE xx or GENDER XX\n");
    printf("exit   - exit current software\n");
    printf("?      - print this help menu\n");
    printf("help   - print this help menu\n");
}








/***************************************************************************
***
** 函数名称: main
** 功能描述: main 函数
****************************************************************************
**/
int main (int argc, char *argv[])
{
sqlite3 *db;
int rv = 0;
rv = sqlite3_open("stu.db", &db);
if (rv) {
	sqlite3_close(db);
	return (-1);
}
/*sqlCreate(db);
sqlInsert(db);
sqlSelect(db);
printf("--------after delete------------\n");
sqlDelete(db);
sqlSelect(db);
sqlite3_close(db);*/
sqlCreate(db);
char s[100];
word_LinkList l;
createWordLinkList(&l);
while(1) {
    getString(s);
    readString(l, s, strlen(s));
    char *firstWord;
    int len = len_WordLinkList(l);
    if (len > 0) {
        firstWord = l->next->word;
    }
    if (len == 1) {
        if (strcmp(firstWord, "?") == 0 || strcmp(firstWord, "help") == 0) {
            printHelp();
        } else if (strcmp(firstWord, "exit") == 0) {
            break;
        } else if (strcmp(firstWord, "show") == 0) {
            show(db);
        }
    } else if (strcmp(firstWord, "insert") == 0) {
        if (len == 5) {
        	int len;
        	char name[25];
        	char id[25];
        	char age[25];
        	char gender[25];
        	word_Node * cur = l->next->next;
        	cur = readWord(cur, name, &len);
        	cur = readWord(cur, id, &len);
        	cur = readWord(cur, age, &len);
        	readWord(cur, gender, &len);
        	sqlInsert(db, name, id, age, gender);
        }
    } else if (strcmp(firstWord, "search") == 0) {
        if (len == 3){
                		int len;
                        char snd[25];
                        char rd[25];
                        word_Node * cur = l->next->next;
                        cur = readWord(cur, snd, &len);
                        readWord(cur, rd, &len);
                        sqlSelect(db, snd, rd);
                }
    } else if (strcmp(firstWord, "delete") == 0) {
        if (len == 3){
        		int len;
                char snd[25];
                char rd[25];
                word_Node * cur = l->next->next;
                cur = readWord(cur, snd, &len);
                readWord(cur, rd, &len);
                sqlDelete(db, snd, rd);
        }
    }
    clear_WordLinkList(&l);
}


return (0);
}
