#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

#include "showFile.h"
#include "backup.h"

int main(void)
{
    int select = 0;
    for(;;)
    {
        system("cls");
        do{
        printf("-------------------------------------------------\n");
        printf("The Default Path is : %s\n", SELF_LOAD_DEFAULT_PATH);
        printf("Now The Path is     : %s\n", getBackUpPath());
        printf("-------------------------------------------------\n");
        printf("That is a System Back Up Software for Windows! \n");
        printf("List of the software function : \n");
        printf("1. Back Up \n");
        printf("2. Set Back Up Path \n");
        printf("3. Show Path History\n");
        printf("4. Read Me \n");
        printf("5. Exit \n");
        printf("-------------------------------------------------\n");

        printf("Your Select: ");
        scanf("%d", &select);
        getchar();
        }while(select != 1 && select != 2 &&
               select != 3 && select != 4 &&
               select != 5  );
        system("cls");

        switch(select)
        {
        case 1 :
            printf("Back Up Begin! \n");
            showBUSelect();    // 开始备份
            printf("Back Up End!   \n");
            break;
        case 2 :
            getEnterPath();     // 设置备份存储路径
            break;
        case 3 :
            showPathHistory(); // 查看以往的备份存储路径历史
            break;
        case 4 :
            showReadMe(); 
            break;
        case 5 :
            exit(1);
        default:
            break;
        }/** switch(select) **/
    }/** for(;;) **/
    return 0;
}
