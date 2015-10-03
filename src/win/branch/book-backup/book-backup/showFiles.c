#if 0
#include "showFiles.h"
static jmp_buf alloc_jmp;
static jmp_buf get_hd_jmp;
void show_structure(const char * from_dir_name, int depth)
{
	/* ������� setjmp��longjmp���»�ȡ���HANDLE�ı��� */
	int times = 0;
	/** ����ʱ��ȡ�ļ��У��ļ���Ϣ�ĵı�Ҫ����,Ϊ��WIN API�����������������ʹ���շ巨 **/
	HANDLE fileHandle;    
	WIN32_FIND_DATAA fileData;
	LARGE_INTEGER fileSize;

	size_t length = strlen(from_dir_name);
	char * dir_buf = make_path(from_dir_name);  //·��
	char * dir_file_buf = make_path(from_dir_name); //�ļ�
	if (dir_buf == NULL || dir_file_buf == NULL)
		return;
	adjust_path(dir_buf, dir_file_buf); /* ����·�����ļ���ʽ����׼��ʽ */
	repl_str(dir_buf);
	repl_str(dir_file_buf);
//	printf("After adjust \n%s \n%s\n", dir_buf, dir_file_buf);
	/* ��ʼ���� Windows API ��ȡ·���µ��ļ����ļ�����Ϣ */
	setjmp(get_hd_jmp);
	fileHandle = FindFirstFileA(dir_buf, &fileData);
	if (fileHandle == INVALID_HANDLE_VALUE) /* ����޷���ȡ����������޴��������˳� */
	{
		fprintf(stderr, "The Handle getting Failure! \n");
		if (times++ < TRY_TIMES)
			longjmp(get_hd_jmp, 0);
		return;
	}
	do{
		char * tmp_dir_file_buf = make_path(dir_file_buf);
		if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			fprintf(stderr, "%*s%s\t<DIR>\t\n", depth, "", fileData.cFileName);
			if (strcmp(fileData.cFileName, ".") == 0 ||  /* . �� .. ���ǵ�ǰ�ļ��к���һ���ļ���, ����������ϵͳ��һ�� */
				strcmp(fileData.cFileName, "..") == 0)
				continue;
			strcat(tmp_dir_file_buf, fileData.cFileName); /* ���ļ������ӵ���ǰ�ļ���·��֮���γ��ļ�·�� */
			show_structure(tmp_dir_file_buf, depth + 4);
		}
		else
		{
			fileSize.LowPart = fileData.nFileSizeLow;   /* �����С */
			fileSize.HighPart = fileData.nFileSizeHigh;
			fprintf(stderr, "%*s%s    \t%ld bytes\t\n", depth, "", fileData.cFileName, 
				                         fileSize.QuadPart);
		}
		rele_path(tmp_dir_file_buf);
	} while (FindNextFileA(fileHandle, &fileData));
	FindClose(fileHandle);
	rele_path(dir_buf);
	rele_path(dir_file_buf);
	return;
}

char * make_path(const char * src)
{
	int times = 0;
	size_t len_of_src = strlen(src);
	size_t len_of_dst = MIN_PATH_NAME;
	
	if (len_of_src > MIN_PATH_NAME - 10) /* \\?\ //* 8���ַ� */
	{
		len_of_dst = LARGEST_PATH_NAME;
		if (len_of_src > LARGEST_PATH_NAME - 10)
		{
			fprintf(stderr, "The Path Name is larger than 32767, Which is not Support!\n%s", src);
			return NULL;
		}
	}	
	setjmp(alloc_jmp); /* alloc_jmp to here */
	char * loc_buf = malloc(len_of_dst + 1);
	if (loc_buf == NULL)
	{
		fprintf(stderr, "ERROR OCCUR When malloc the memory at %s\n Try the %d th times", __LINE__, times+1);
		if (times++ < TRY_TIMES)
			longjmp(alloc_jmp, 0); /* alloc_jmp from here */
		return NULL;
	}
	//sprintf(loc_buf, "\\\\?\\%s", src); /* ǰ���Ѿ�ȷ�������ᳬ����Χ���ʲ���ʹ�ù��� snprintf */
	strcpy(loc_buf, src);
	return loc_buf;
}

void adjust_path(char * src_path, char * src_file)
{
	size_t length = strlen(src_path); /* ���������ĳ����ڴ˺�������֮ǰ�ض�һ�� */
	if (length == 1) /* �������Ϊ�����û�������Ǹ�Ŀ¼����� ����: C */
	{
		strcat(src_file, ":/");
	}
	else if (src_path[length - 1] != '\\' && src_path[length - 1] != '/')
	{
		strcat(src_file, "/");
	}
	else
	{
		src_path[length - 1] = '/';
	}
	strcpy(src_path, src_file);
	strcat(src_path, "/*");
	return;
}
#if 0
void repl_str(char * src)
{
	size_t length = strlen(src);
	for (size_t i = 0; i <= length; ++i)
	{
		if (src[i] == '/')
			src[i] = '\\';
	}
	return;
}
#endif
void rele_path(char * src)
{
	free(src);
	src = NULL;
	return;
}
#endif