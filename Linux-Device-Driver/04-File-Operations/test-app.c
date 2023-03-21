#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdio_ext.h>

#define CDEV_PATH "/dev/m-device"

int fd, option;
char write_buf[1024];
char read_buf[1024];
void file_descriptor_open()
{
    
    fd = open(CDEV_PATH, O_RDWR);
    if (fd < 0) {
        printf("Cannot open device file: %s...\n", CDEV_PATH);
        exit(1);
    }
}

void file_descriptor_release()
{
    close(fd);
}
void lseek_device(){
    
    int lseek_offset = 0, seek_value = 0;
    printf("enter the seek offset\n");
    scanf("%d", &lseek_offset);
    printf("seek_offset selected is %d\n", lseek_offset);
    printf("1 for SEEK_SET, 2 for SEEK_CUR and 3 for SEEK_END\n");
    scanf("%d", &seek_value);
    printf("seek value = %d\n", seek_value);

        switch (seek_value) {
        case 1: lseek(fd, lseek_offset, SEEK_SET);
                break;
        case 2: lseek(fd, lseek_offset, SEEK_CUR);
                break;
        case 3: lseek(fd, lseek_offset, SEEK_END);
                break;
        default :
                printf("unknown  option selected, please enter right one\n");
                break;
        }
    
}

void Write_Data(){
    
    printf("Enter the string to write into driver: ");
    scanf("  %[^\t\n]s", write_buf);
    printf("Data Writing ... ");
    write(fd, write_buf, strlen(write_buf)+1);
    printf("Done!\n\n\n");
   
}

void Read_Data(){
    
    printf("Data Reading ... ");
    read(fd, read_buf, 1024);                
    printf("Done!\n");
    printf("Data: %s\n\n\n", read_buf);
    
}

void Lseek_Write(){
    lseek_device();
    Write_Data();
}

void Lseek_Read(){
    lseek_device();
    Read_Data();

}
void printMenu()
{

    printf("****Please Enter the Option******\n");
    printf("        1. Write                 \n");
    printf("        2. Read                  \n");
    printf("        3. Lseek and Write       \n");
    printf("        4. Lseek and Read        \n");
    printf("        5. Lseek                 \n");    
    printf("        6. Exit                  \n");
    printf("*********************************\n");
    printf(">>> ");
}



int main()
{   
    printf("*********************************\n");
    printf("*******Linux From Scratch*******\n\n");


    while(1) {
        printMenu();

        scanf("%d",&option); 
        switch (option) {
            case 1:
                file_descriptor_open();
                Write_Data();
                file_descriptor_release();
                break;

            case 2:
                file_descriptor_open();
                Read_Data();
                file_descriptor_release();
                break;
            case 3:
                file_descriptor_open();
                Lseek_Write();
                file_descriptor_release();
                break;
            case 4:
            file_descriptor_open();
                Lseek_Read();
            file_descriptor_release();
                break;
            case 5:
            file_descriptor_open();
                lseek_device();
            file_descriptor_release();
                break;
            case 6:
                file_descriptor_release();
                exit(1);               
                break;

            default:
                printf("Enter Valid option = %c\n",option);
                break;
        }
    }
    
    

    return 0;
}