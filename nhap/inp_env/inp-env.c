// #include <stdio.h>
// #include <string.h>
// int main(int argc, char*argv[])
// {
//     // printf("argc = %d\n",argc);
//     // for(int i = 0; i < argc;i++)
//     // {
//     //     printf("argv[%d] = %s\n",i,argv[i]);
//     // }

//     char *check = argv[1];
//     printf("%s\n",check);
//     if(strncmp(check,".chiduc",1) == 0)
//     {
//         printf("Is option\n");
//     }
//     else if(strncmp(check,"-chiduc",1) == 0)
//     {
//         printf("is link\n");
//     }
//     else
//     {
//         printf("invalid form\n");
//     }
//     return 0;
// }


#include <stdio.h>

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

int isFile(const char* name)
{
    DIR* directory = opendir(name);

    if(directory != NULL)
    {
     closedir(directory);
     return 0;
    }

    if(errno == ENOTDIR)
    {
     return 1;
    }

    return -1;
}

int main(void)
{
    const char* file = "/home/duc/workspace/nhap/inp_env/inp-env.c";
    const char* directory = "/home/duc/workspace/nhap/inp_env";

    printf("Is %s a file? %s.\n", file,
     ((isFile(file) == 1) ? "Yes" : "No"));

    printf("Is %s a directory? %s.\n", directory,
     ((isFile(directory) == 0) ? "Yes" : "No"));

    return 0;
}