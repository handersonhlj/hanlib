#include <stdio.h>

int main(void)
{
    char filename[10];
    char putbuf[10]={1,2,3,4};
    int i=0;
    i++;
    sprintf(filename, "./%d.pcm", i);
    printf("%s\n", filename);
    FILE *f_pcm = fopen(filename, "wb+"); //mode do not use '
    if(f_pcm == NULL) {
    printf("文件不能打开！\n");exit(0);
}
    if (f_pcm == NULL)
        printf("open error\n");
    fwrite(putbuf, sizeof(char), 10, f_pcm);
    fclose(f_pcm);
    return 0;
}