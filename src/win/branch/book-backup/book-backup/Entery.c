#include <stdio.h>
#include <stdlib.h>
#include "setPath.h"
#include "backup.h"

extern const char * DEFAULT_TO_PATH; /* setPath.c */

int main(int argc, char* argv[])
{
	int select = 0;
	init_path();
	while (1)
	{
		system("cls");
		do{
			puts("-------------------------------------------------");
			fprintf(stdout, "The Default Path is : %s \n", DEFAULT_TO_PATH);
			fprintf(stdout, "Now The Path is     : %s \n", get_backup_topath());
			puts("-------------------------------------------------");
			puts("That is a System Back Up Software for Windows! ");
			puts("List of the software function : ");
			puts("1. Back Up ");
			puts("2. Set Back Up TO-PATH ");
			puts("3. Show TO-PATH History");
			puts("4. Read Me ");
			puts("5. Exit ");
			puts("-------------------------------------------------");
			puts("Your Select: ");
			
			fscanf(stdin, "%d", &select);
			getchar();
		} while (select < 1 || select > 5);
		system("cls");
		switch (select)
		{			
		case 1:
			sec_main_windows();
			break;
		case 2:
			set_enter_path();
			break;
		case 3:
			show_hist_path();
			break;
		case 4:
			/* Stay For Yourselves */
			break;
		case 5:
			exit(1);
		default:
			break;
		}/** switch(select) **/
	}/** while(1) **/
	system("pause");
	return 0;
}