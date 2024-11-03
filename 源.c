#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<winsock.h>
#include<mysql.h>
#include<string.h>
#include<ctype.h>

#define DEFAULT_SERVER "127.0.0.1"
#define DEFAULT_USERNAME "root"
#define DEFAULT_PASSWORD "123456"
#define DEFAULT_DATABASE "S_T_U202312348"

int strcasecmp(const char* str1, const char* str2);
int str_is_digit(char* str);
void finish_with_error(MYSQL* conn);
void scanf_right_istr(char istr[], const char* name);
void scanf_right_choose(int* choose);
void scanf_right_dept(MYSQL* conn, char* dept);
void update(MYSQL* conn, char table[]);
void insert(MYSQL* conn, char table[]);
void delete(MYSQL* conn);
void select_info(MYSQL* conn, char dept[], int choose);
void choose_1(MYSQL* conn);
void choose_2(MYSQL* conn);
void choose_3(MYSQL* conn);
void choose_4(MYSQL* conn);
void choose_5(MYSQL* conn);
void choose_6(MYSQL* conn);

int main()
{

	MYSQL* conn = mysql_init(NULL);
	if (conn == NULL)
	{
		printf("数据库初始化失败\n");
		exit(1);
	}
	if (!mysql_real_connect(conn, DEFAULT_SERVER, DEFAULT_USERNAME, DEFAULT_PASSWORD, DEFAULT_DATABASE, 0, NULL, 0))
	{
		printf("连接失败：");
		finish_with_error(conn);
	}
	if (mysql_query(conn, "set names gbk"))
		printf("%s\n", mysql_error(conn));
	printf("数据库：%s连接成功！\n", DEFAULT_DATABASE);
	while (1)
	{
		int choose;
		printf("+---------------+\n");
		printf("|0：退出数据库  |\n");
		printf("+---------------+\n");
		printf("|1：学生信息维护|\n");
		printf("+---------------+\n");
		printf("|2：课程信息维护|\n");
		printf("+---------------+\n");
		printf("|3：成绩信息维护|\n");
		printf("+---------------+\n");
		printf("|4：统计学生信息|\n");
		printf("+---------------+\n");
		printf("|5：学生成绩排名|\n");
		printf("+---------------+\n");
		printf("|6：检索学生信息|\n");
		printf("+---------------+\n");
		printf("输入选项：");
		scanf_right_choose(&choose);
		switch (choose)
		{
		case 0:
			return 0;
		case 1:
		{
			choose_1(conn);
			break;
		}
		case 2:
		{
			choose_2(conn);
			break;
		}
		case 3:
		{
			choose_3(conn);
			break;
		}
		case 4:
		{
			choose_4(conn);
			break;
		}
		case 5:
		{
			choose_5(conn);
			break;
		}
		case 6:
		{
			choose_6(conn);
			break;
		}
		default:
		{
			printf("未知选项\n");
			continue;
		}
		}
	}
	return 0;
}

int strcasecmp(const char* str1, const char* str2) //忽略大小写的字符串比较
{
	while (*str1 && *str2) 
	{
		char c1 = tolower((unsigned char)*str1);
		char c2 = tolower((unsigned char)*str2);
		if (c1 != c2) 
			return c1 - c2;
		str1++;
		str2++;
	}
	return tolower((unsigned char)*str1) - tolower((unsigned char)*str2);
}

int str_is_digit(char* str)
{
	if (strspn(str, "0123456789") == strlen(str))
		return 1;
	else
		return 0;
}

void finish_with_error(MYSQL* conn)
{
	printf("%s\n", mysql_error(conn));
	mysql_close(conn);
	exit(1);
}

void scanf_right_istr(char istr[], const char* name)
{
	int a = strcmp(name, "先行课");
	int b = strcasecmp(istr, "NULL");
	while (1)
	{
		scanf("%s", istr);
		if (str_is_digit(istr) || (!strcmp(name, "先行课") && !strcasecmp(istr, "NULL")))
			break;
		printf("输入由数字组成的%s", name);
	}
}

void scanf_right_choose(int* choose)
{
	while (scanf("%d", choose) != 1)
	{
		printf("无效输入，请输入一个数字。\n");
		while (getchar() != '\n'); // 清空输入缓冲区
		printf("输入选项：");
		continue;
	}
	while (getchar() != '\n'); // 清空输入缓冲区
}

void scanf_right_dept(MYSQL* conn, char* dept)
{
	while (1)
	{
		printf("输入你要检索的系：");
		scanf("%s", dept);
		MYSQL_RES* res;
		MYSQL_ROW row;
		int right_dept = 1;
		if (mysql_query(conn, "select distinct sdept from student"))
			finish_with_error(conn);
		res = mysql_store_result(conn);
		while ((row = mysql_fetch_row(res)) != NULL)
		{
			if (!strcasecmp(row[0], dept))
				break;
		}
		if (row == NULL)
		{
			printf("%s系不存在,请输入正确的系名\n", dept);
			continue;
		}
		break;
	}
}

void update(MYSQL* conn, char table[])
{
	if (!strcasecmp(table, "student"))
	{
		char sno[10], sname[21], ssex[3], sage[3], sdept[21], scholarship[3];
		printf("输入学号：");
		scanf_right_istr(sno, "学号"); //得到正确格式的学号
		while (1) //此循环用来处理未知选项输入的情况
		{
			printf("+---------------+\n");
			printf("|0：返回上级操作|\n");
			printf("+---------------+\n");
			printf("|1：重新输入学号|\n");
			printf("+---------------+\n");
			printf("|2：学生姓名    |\n");
			printf("+---------------+\n");
			printf("|3：学生性别    |\n");
			printf("+---------------+\n");
			printf("|4：学生年龄    |\n");
			printf("+---------------+\n");
			printf("|5：学生院系    |\n");
			printf("+---------------+\n");
			printf("|6：奖学金      |\n");
			printf("+---------------+\n");
			int choose;
			printf("输入你要修改的信息：");
			scanf_right_choose(&choose);
			char operation[200] = "update student set ";
			switch (choose)
			{
			case 0:
				return;
			case 1:
			{
				printf("输入学号：");
				scanf_right_istr(sno, "学号");
				continue;
			}
			case 2:
			{
				printf("输入姓名的修改值：");
				scanf("%s", sname);
				strcat(operation, "sname='");
				strcat(operation, sname);
				strcat(operation, "'");
				break;
			}
			case 3:
			{
				printf("输入性别的修改值：");
				scanf("%s", ssex);
				strcat(operation, "ssex='");
				strcat(operation, ssex);
				strcat(operation, "'");
				break;
			}
			case 4:
			{
				printf("输入年龄的修改值：");
				scanf_right_istr(sage, "年龄");
				strcat(operation, "sage=");
				strcat(operation, sage);
				break;
			}
			case 5:
			{
				printf("输入院系的修改值：");
				scanf("%s", sdept);
				strcat(operation, "sdept='");
				strcat(operation, sdept);
				strcat(operation, "'");
				break;
			}
			case 6:
			{
				printf("输入奖学金的修改值：");
				scanf("%s", scholarship);
				strcat(operation, "scholarship='");
				strcat(operation, scholarship);
				strcat(operation, "'");
				break;
			}
			default:
			{
				printf("未知选项，请输入0-6\n");
				continue;
			}
			}
			strcat(operation, " where sno='");
			strcat(operation, sno);
			strcat(operation, "'");
			if (mysql_query(conn, operation))
				printf("学生信息修改出错：%s\n", mysql_error(conn)); //输出错误信息
			else
				printf("学生信息修改成功！\n");
		}
	}
	if (!strcasecmp(table, "course"))
	{
		char cno[5], cname[41], cpno[5], ccredit[3];
		printf("输入课程号：");
		scanf_right_istr(cno, "课程号");
		while (1)
		{
			printf("+-----------------+\n");
			printf("|0：返回上级操作  |\n");
			printf("+-----------------+\n");
			printf("|1：重新输入课程号|\n");
			printf("+-----------------+\n");
			printf("|2：课程名        |\n");
			printf("+-----------------+\n");
			printf("|3：课程的先行课  |\n");
			printf("+-----------------+\n");
			printf("|4：课程的学分    |\n");
			printf("+-----------------+\n");
			int choose;
			printf("输入选项：");
			scanf_right_choose(&choose);
			char operation[200] = "update course set ";
			switch (choose)
			{
			case 0:
				return;
			case 1:
			{
				printf("输入课程号：");
				scanf_right_istr(cno, "课程号");
				continue;
			}
			case 2:
			{
				printf("输入课程名的修改值：");
				scanf("%s", cname);
				strcat(operation, "cname='");
				strcat(operation, cname);
				strcat(operation, "'");
				break;
			}
			case 3:
			{
				printf("输入课程先行课的修改值：");
				scanf_right_istr(cpno, "先行课");
				strcat(operation, "cpno=");
				if (strcasecmp(cpno, "NULL"))
				{
					strcat(operation, "'");
					strcat(operation, cpno);
					strcat(operation, "'");
				}
				else
					strcat(operation, cpno);
				break;
			}
			case 4:
			{
				printf("输入学分的修改值：");
				scanf_right_istr(ccredit, "学分");
				strcat(operation, "ccredit=");
				strcat(operation, ccredit);
				break;
			}
			default:
			{
				printf("未知选项，请输入0-4\n");
				continue;
			}
			}
			strcat(operation, " where cno='");
			strcat(operation, cno);
			strcat(operation, "'");
			if (mysql_query(conn, operation))
			{
				printf("课程信息修改出错：%s\n", mysql_error(conn));
				continue;
			}
			else
				printf("课程信息修改成功！\n");
		}
	}
	if (!strcasecmp(table, "sc"))
	{
		char sno[10], cno[5], grade[3];
		printf("输入学号：");
		scanf_right_istr(sno, "学号");
		printf("输入课程号：");
		scanf_right_istr(cno, "课程号");
		printf("输入成绩的修改值：");
		scanf_right_istr(grade, "成绩");
		char operation[200] = "update sc set grade=";
		strcat(operation, grade);
		strcat(operation, " where sno='");
		strcat(operation, sno);
		strcat(operation, "' and cno='");
		strcat(operation, cno);
		strcat(operation, "'");
		if (mysql_query(conn, operation))
			printf("成绩修改出错：%s\n", mysql_error(conn));
		else
			printf("成绩修改成功！\n");
	}
}

void insert(MYSQL* conn, char table[])
{
	if (!strcasecmp(table, "student"))
	{
		char operation[200] = "INSERT INTO Student VALUES ('";
		char Sno[20], Sname[20], Ssex[3], Sage[3], Sdept[20], Schorlarship[3];
		printf("输入学生的学号：");
		scanf_right_istr(Sno, "学号");
		printf("输入学生的姓名：");
		scanf("%s", Sname);
		printf("输入学生的姓别：");
		scanf("%s", Ssex);
		printf("输入学生的年龄：");
		scanf_right_istr(Sage, "年龄");
		printf("输入学生的院系：");
		scanf("%s", Sdept);
		printf("学生是否有奖学金：");
		scanf("%s", Schorlarship);
		strcat(operation, Sno);
		strcat(operation, "','");
		strcat(operation, Sname);
		strcat(operation, "','");
		strcat(operation, Ssex);
		strcat(operation, "',");
		strcat(operation, Sage);
		strcat(operation, ",'");
		strcat(operation, Sdept);
		strcat(operation, "','");
		strcat(operation, Schorlarship);
		strcat(operation, "')");
		if (mysql_query(conn, operation) == 0)
			printf("学生信息：%s插入成功\n", &operation[27]);
		else
			printf("学生信息插入失败：\n%s\n", mysql_error(conn));
	}
	if (!strcasecmp(table, "course"))
	{
		char operation[200] = "INSERT INTO Course VALUES ('";
		char Cno[5], Cname[41], Cpno[5], Ccredit[3];
		printf("输入课程的课程号：");
		scanf_right_istr(Cno, "课程号");
		printf("输入课程的名字：");
		scanf("%s", Cname);
		printf("输入课程的先行课的课程号（无则输入NULL）：");
		scanf_right_istr(Cpno, "先行课");
		printf("输入课程的学分：");
		scanf_right_istr(Ccredit, "学分");
		strcat(operation, Cno);
		strcat(operation, "','");
		strcat(operation, Cname);
		strcat(operation, "',");
		if(strcasecmp(Cpno, "NULL"))
		{
			strcat(operation, "'");
			strcat(operation, Cpno);
			strcat(operation, "',");
		}
		else
		{
			strcat(operation, Cpno);
			strcat(operation, ",");
		}
		strcat(operation, Ccredit);
		strcat(operation, ")");
		if (mysql_query(conn, operation) == 0)
			printf("课程信息：%s插入成功\n", &operation[26]);
		else
			printf("课程信息插入失败：\n%s\n", mysql_error(conn));
	}
	if (!strcasecmp(table, "sc"))
	{
		char operation[200] = "INSERT INTO SC VALUES ('";
		char Sno[10], Cno[5], Grade[4];
		printf("输入学号：");
		scanf_right_istr(Sno, "学号");
		printf("输入课程号：");
		scanf_right_istr(Cno, "课程号");
		printf("输入成绩：");
		scanf_right_istr(Grade, "成绩");
		strcat(operation, Sno);
		strcat(operation, "','");
		strcat(operation, Cno);
		strcat(operation, "',");
		strcat(operation, Grade);
		strcat(operation, ")");
		if (mysql_query(conn, operation) == 0)
			printf("成绩信息：%s插入成功\n", &operation[22]);
		else
			printf("信息插入失败：\n%s\n", mysql_error(conn));
	}
}

void delete(MYSQL* conn)
{
	char operation[] = "delete from course where cno not in (select temp.cno from (select distinct cno from sc union select distinct cpno from course)temp where temp.cno != 'NULL')";
	if (mysql_query(conn, operation) == 0)
		printf("信息删除成功\n");
	else
		printf("信息删除失败：%s\n", mysql_error(conn));
}

void select_info(MYSQL* conn, char dept[], int choose)
{
	char operation[512];
	MYSQL_RES* res;
	MYSQL_ROW row;
	int student_num;
	if (choose == 5)
	{
		strcpy(operation, "SELECT COUNT(*) FROM Student WHERE Sdept='");
		strcat(operation, dept);
		strcat(operation, "'");
		if (mysql_query(conn, operation))
		{
			printf("Error:%s\n", mysql_error(conn));
			return;
		}
		res = mysql_store_result(conn);
		row = mysql_fetch_row(res);
		student_num = atoi(row[0]);
	}
	switch (choose)
	{
	case 2:
	{
		strcpy(operation, "SELECT AVG(Grade) FROM SC,Student WHERE SC.Sno=Student.Sno AND Sdept='");
		strcat(operation, dept);
		strcat(operation, "'");
		break;
	}
	case 3:
	{
		strcpy(operation, "SELECT MAX(Grade) FROM SC,Student WHERE SC.Sno=Student.Sno AND Sdept='");
		strcat(operation, dept);
		strcat(operation, "'");
		break;
	}
	case 4:
	{
		strcpy(operation, "SELECT MIN(Grade) FROM SC,Student WHERE SC.Sno=Student.Sno AND Sdept='");
		strcat(operation, dept);
		strcat(operation, "'");
		break;
	}
	case 5:
	{
		strcpy(operation, "SELECT COUNT(DISTINCT SC.Sno) FROM SC,Student WHERE SC.Sno=Student.Sno and Grade>=90 AND Sdept='");
		strcat(operation, dept);
		strcat(operation, "'");
		break;
	}
	case 6:
	{
		strcpy(operation, "SELECT COUNT(DISTINCT SC.Sno) FROM SC,Student WHERE SC.Sno=Student.Sno and (Grade<60 OR Grade = NULL) AND Sdept='");
		strcat(operation, dept);
		strcat(operation, "'");
		break;
	}
	}
	if (mysql_query(conn, operation))
	{
		printf("Error：%s", mysql_error(conn));
		return;
	}
	res = mysql_store_result(conn);
	row = mysql_fetch_row(res);
	switch (choose)
	{
	case 2:
	{
		printf("%s系平均成绩为：%s\n", dept, row[0]);
		return;
	}
	case 3:
	{
		printf("%s系最好成绩为：%s\n", dept, row[0]);
		return;
	}
	case 4:
	{
		printf("%s系最差成绩为：%s\n", dept, row[0]);
		return;
	}
	case 5:
	{
		int good_student_num = atoi(row[0]);
		printf("%s系优秀率为：%.1lf%%\n", dept, 100 * good_student_num /(double)student_num);
		return;
	}
	case 6:
	{
		printf("%s系不及格人数为：%s\n", dept, row[0]);
		return;
	}
	}
}

void choose_1(MYSQL* conn)
{
	while (1)
	{
		int choose;
		printf("+---------------+\n");
		printf("|0：返回上级操作|\n");
		printf("+---------------+\n");
		printf("|1：增加学生信息|\n");
		printf("+---------------+\n");
		printf("|2：修改学生信息|\n");
		printf("+---------------+\n");
		printf("输入选项：");
		scanf_right_choose(&choose);
		if (choose == 0)
			return;
		if (choose != 1 && choose != 2)
			printf("未知选项\n");
		else if (choose == 1)
			insert(conn, "student");
		else if (choose == 2)
			update(conn, "Student");
	}
}

void choose_2(MYSQL* conn)
{
	while (1)
	{
		int choose;
		printf("+---------------+\n");
		printf("|0：返回上级操作|\n");
		printf("+---------------+\n");
		printf("|1：增加课程信息|\n");
		printf("+---------------+\n");
		printf("|2：修改课程信息|\n");
		printf("+---------------+\n");
		printf("|3：删除课程    |\n");
		printf("+---------------+\n");
		printf("输入选项：");
		scanf_right_choose(&choose);
		if (choose == 0)
			return;
		else if (choose != 1 && choose != 2 && choose != 3)
			printf("未知选项\n");
		if (choose == 1)
			insert(conn, "course");
		else if (choose == 2)
			update(conn, "Course");
		else
			delete(conn, "Course");
	}
}

void choose_3(MYSQL* conn)
{
	while (1)
	{
		int choose;
		printf("+---------------+\n");
		printf("|0：返回上级操作|\n");
		printf("+---------------+\n");
		printf("|1：录入学生成绩|\n");
		printf("+---------------+\n");
		printf("|2：修改学生成绩|\n");
		printf("+---------------+\n");
		printf("输入选项：");
		scanf_right_choose(&choose);
		if (choose == 0)
			return;
		else if (choose != 1 && choose != 2)
			printf("未知选项\n");
		else if (choose == 1)
			insert(conn, "sc");
		else if (choose == 2)
			update(conn, "sc");
	}
}

void choose_4(MYSQL* conn)
{
	char dept[20];
	scanf_right_dept(conn, dept);
	while (1)
	{
		printf("+-----------------------+\n");
		printf("|0：返回上级操作        |\n");
		printf("+-----------------------+\n");
		printf("|1：重新选择系          |\n");
		printf("+-----------------------+\n");
		printf("|2：统计学生的平均成绩  |\n");
		printf("+-----------------------+\n");
		printf("|3：统计学生的最好成绩  |\n");
		printf("+-----------------------+\n");
		printf("|4：统计学生的最差成绩  |\n");
		printf("+-----------------------+\n");
		printf("|5：统计学生的优秀率    |\n");
		printf("+-----------------------+\n");
		printf("|6：统计学生的不及格人数|\n");
		printf("+---------------------+\n");
		char choose[10];
		printf("输入你要统计的信息：");
		scanf("%s", choose);
		if (strspn(choose, "0123456") != strlen(choose))
		{
			printf("存在未知选项：%c\n", choose[strspn(choose, "0123456")]);
			continue;
		}
		if (strstr(choose, "0"))
			return;
		if (strstr(choose, "1"))
		{
			printf("输入你要检索的系：\n");
			scanf("%s", dept);
			continue;
		}
		if (strstr(choose, "2"))
			select_info(conn, dept, 2);
		if (strstr(choose, "3"))
			select_info(conn, dept, 3);
		if (strstr(choose, "4"))
			select_info(conn, dept, 4);
		if (strstr(choose, "5"))
			select_info(conn, dept, 5);
		if (strstr(choose, "6"))
			select_info(conn, dept, 6);
	}
}

void choose_5(MYSQL* conn)
{
	char dept[20];
	scanf_right_dept(conn, dept);
	MYSQL_RES* res;
	MYSQL_ROW row;
	MYSQL_FIELD* field;
	char operation[512] = "select sno,sname,ssex,sage,cno,cname,grade from student join sc using(sno) join course using(cno) where sdept='";
	strcat(operation, dept);
	strcat(operation, "'");
	strcat(operation, " order by grade desc");
	if (mysql_query(conn, operation))
		finish_with_error(conn);
	res = mysql_store_result(conn);
	int num_fields;
	num_fields = mysql_num_fields(res);
	for (int i = 0; i < num_fields; i++)
	{
		field = mysql_fetch_field_direct(res, i);
		printf("%-10s", field->name);
	}
	printf("\n");
	while ((row = mysql_fetch_row(res)) != NULL)
	{
		for (int i = 0; i < num_fields; i++)
			printf("%-10s", row[i]);
		printf("\n");
	}
}

void choose_6(MYSQL* conn)
{
	MYSQL_RES* res;
	MYSQL_ROW row;
	MYSQL_FIELD* field;
	while (1)
	{
		printf("输入学生的学号：");
		char sno[10];
		scanf("%s", sno);
		char operation[512] = "select * from student left join sc using(sno) where sno='";
		strcat(operation, sno);
		strcat(operation, "'");
		if (mysql_query(conn, operation))
			finish_with_error(conn);
		res = mysql_store_result(conn);
		if ((row = mysql_fetch_row(res)) != NULL)
		{
			int num_fields;
			num_fields = mysql_num_fields(res);
			for (int i = 0; i < num_fields; i++)
			{
				field = mysql_fetch_field_direct(res, i);
				printf("%-13s", field->name);
			}
			printf("\n");
			do
			{
				for (int i = 0; i < num_fields; i++)
					printf("%-13s", row[i]);
				printf("\n");
			} while (((row = mysql_fetch_row(res)) != NULL));
			break;
		}
		else
		{
			printf("请输入正确的学号\n");
			continue;
		}
	}
}
