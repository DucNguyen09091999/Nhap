#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <dirent.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

struct check_env
{
  bool is-option;
  
}check_env;

struct virtual_env
{
  int virtual_argc;
  char *virtual_argv;
}virtual_env;




struct executed_command {
  int options_Num;
  char* options;
  bool is_path_to_dir_passed ;
  char* path_to_dir;

};

int check_is_folder(const char* name)
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

void check_is_folder_link_or_option(int argc, char *argv[], struct executed_command* command_info)
{
  char *check = argv[1];
  if(strncmp(check,"-",1) == 0)
  {
    printf("is option\n");
  }
  else if(check_is_folder(check) == 0)
  {
      printf("is directory\n");
      virtual_env.virtual_argv = check;
      printf("directory link: %s\n",check);

  }
  else
  {
    printf("Invalid\n");
  }
}

void check_is_path_or_current_folder(int argc, char *argv[], struct executed_command* command_info)
{
    if(argc == 1)
    {
      printf("Current folder\n");
      virtual_env.virtual_argv = ".";
      printf("directory link: %s\n",virtual_env.virtual_argv);
    }
    else
    {
      check_is_folder_link_or_option(argc,argv,command_info);
      
    }
 
}

void check_command_info_data(int argc, char *argv[], struct executed_command* command_info)
{
  check_is_path_or_current_folder(argc,argv,command_info);
}


int main(int argc, char *argv[])
{
    DIR *dp;
    struct stat file_stats;
    struct dirent *ep;
    int ret = EXIT_SUCCESS;
    int ret2;

struct executed_command command_info = {0 , NULL, false, NULL};
check_command_info_data(argc, argv, &command_info);

}