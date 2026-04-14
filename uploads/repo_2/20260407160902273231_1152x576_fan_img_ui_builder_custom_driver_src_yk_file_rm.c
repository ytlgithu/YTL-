
#include "../apps.h"
#ifdef YK_USE_UI_BUILDER

#define BINFILE   "data/myfile.bin"  //norfalsh保存的文件路径

#else

#include <direct.h>  // 添加此行以修复 getcwd 声明缺失的问题
#include <sys/stat.h>
#include <errno.h>


char BINFILE[256] = "data/myfile.bin";  // 路径可动态修改


// 自动创建路径（支持多级目录）
int create_directories(const char *path) {
    char *p = NULL;
    char *path_copy = NULL;
    char *last_slash = NULL;

    // 分配内存并复制路径
    path_copy = strdup(path);
    if (!path_copy) {
        perror("strdup");
        return -1;
    }

    // 替换 Windows 路径分隔符为标准格式
    for (p = path_copy; *p; p++) {
        if (*p == '\\') *p = '/';
    }

    // 从第一个斜杠开始遍历路径
    last_slash = path_copy;
    while ((last_slash = strchr(last_slash, '/')) != NULL) {
        // 提前终止字符串，形成当前子路径
        *last_slash = '\0';

        // 如果路径非空，尝试创建目录
        if (strlen(path_copy) > 0) {
#if defined(_WIN32) || defined(_WIN64)
            if (mkdir(path_copy) != 0 && errno != EEXIST) {
#else
            if (mkdir(path_copy, 0777) != 0 && errno != EEXIST) {
#endif
                fprintf(stderr, "Failed to create directory: %s\n", path_copy);
                free(path_copy);
                return -1;
            }
        }

        // 恢复字符，继续下一个目录
        *last_slash = '/';
        last_slash++;
    }

    // 将传入的路径复制给 BINFILE
    strncpy(BINFILE, path, sizeof(BINFILE) - 1);
    BINFILE[sizeof(BINFILE) - 1] = '\0';  // 确保字符串结束

    printf("BINFILE has been set as: %s\n", BINFILE);

    free(path_copy);
    return 0;
}
#endif  //YK_USE_UI_BUILDER


#if yk_file_rm_h


static uint32_t flash_read_Data(uint32_t addr,uint8_t* databuf,uint32_t len)
{
    char rfilename[256];
	int8_t ret = 0;
	uint32_t reallen = 0;
    memset(rfilename,0,256);	
	//把路径赋值给rfilename
    strncpy(rfilename, BINFILE, sizeof(rfilename) - 1);  // 使用 strncpy 进行安全复制
	printf("rfilename=%s\n",rfilename);
    if(rfilename[0]!=0)
	{
		FILE*   datafp=NULL;
		datafp = fopen(rfilename,"rb");
		if(datafp==NULL)
		{
			printf("open myfile.bin fail\n");
			//fclose(datafp);
	        goto __err;
		}
		ret =fseek(datafp,addr,SEEK_SET);
		if(ret==0){//寻址成功
			reallen = fread(databuf,1,len,datafp);
		}
		else{
			printf("The location exceeds the file size,addr=0x%04lx\n",addr);
		}
		fclose(datafp);
	}
__err:	
	return reallen;
}

static uint32_t flash_write_Data(uint32_t addr,uint8_t* databuf,uint32_t len)
{	
    char wfilename[256] = {0};
	int8_t ret = 0;
	uint32_t reallen = 0;

	//把路径赋值给wfilename
    strncpy(wfilename, BINFILE, sizeof(wfilename) - 1);  // 使用 strncpy 进行安全复制
	printf("wfilename=%s\n",wfilename);
	if(wfilename[0]!=0)
	{
		FILE*   datafp=NULL;
		datafp = fopen(wfilename,"r+b");
		if(datafp == NULL){
        //首先认为文件没有创建
			datafp = fopen(wfilename,"wb");	
		}
		if(datafp==NULL)
		{
			printf("create yk_data.bin fail\n");
			fclose(datafp);
	        goto __err;
		}
		ret = fseek(datafp,addr,SEEK_SET);
		//printf("ret=%d\n",ret);
		if(ret!=0){//如果写入的地址超过了文件的总大小,先用0扩大文件,然后再写入数据
			uint32_t endlen,offset;
			void *data_p;
			fseek(datafp,0,SEEK_END);
			endlen = ftell(datafp);
			offset = addr-endlen;
			data_p = malloc(offset);
			memset(data_p,0, offset);
			fwrite(data_p,1,offset,datafp);
			free(data_p);
			//printf("endlen=%d offset=%d \n",endlen,offset);
		}
		reallen = fwrite(databuf,1,len,datafp);
		fclose(datafp);
	}
__err:
	return reallen;	
}


//文件写入
uint32_t yk_f_file_w(uint8_t* databuf,uint32_t len)
{
    printf("yk_f_file_w\n");
    uint32_t reallen = 0;
    reallen = flash_write_Data(0,databuf,len);//写数据到bin
    return reallen;
}


//文件读取
uint32_t yk_f_file_r(uint8_t* databuf,uint32_t len)
{
    printf("yk_f_file_r\n");
    uint32_t reallen = 0;
    reallen = flash_read_Data(0,databuf,len);//读数据
    return reallen;
}


//文件读写初始化
void yk_f_file_rw_init(uint8_t* databuf,uint32_t len)
{
    printf("yk_f_file_rw_init\n");
	//打印当前工作目录
	char cwd[256];
	if (getcwd(cwd, sizeof(cwd))) {
		printf("Current working directory: %s\n", cwd);
	} else {
		perror("getcwd() error");
	}
	
    //文件读测试
    uint32_t reallen = 0;
    reallen = yk_f_file_r(databuf,len);
    printf("reallen == %ld \n",reallen);

    //文件不存在时,创建文件并写入数据
    if (reallen == 0)
    {
		#ifndef YK_USE_UI_BUILDER
		if (create_directories(BINFILE) != 0) {
			printf("无法创建路径，请检查权限或路径是否合法。\n");
		}
		#endif
        //文件写测试
        yk_f_file_w(databuf,len);
    }
    
    // //打印读取数据
    // for (uint8_t i = 0; i < len; i++)
    // {
    //     printf("databuf[%d] == %d \n",i,databuf[i]);
    // }
}


#endif