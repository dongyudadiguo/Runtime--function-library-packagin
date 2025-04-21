#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char const *argv[])
{
    //argv[1] = "exp.c";//argv[1] = "exp.c";
    FILE *in = fopen(argv[1], "rb");
    fseek(in,0,SEEK_END);
    int file_size = ftell(in);
    fseek(in,0,SEEK_SET);
    char *buffer = malloc(file_size), *ptr = buffer;
    fread(buffer,file_size,1,in);
    char output_name[256];
    char *file_name;
    snprintf(output_name, sizeof(output_name), "Runtime-lib-%s",((file_name = strrchr(argv[1], '\\')) ? file_name + 1 : argv[1]));
    FILE *out = fopen(output_name, "w");
    FILE *list = fopen("list.txt", "a");
    fprintf(list, "\n%s\n", output_name);
    while (1){
        while (1){
            if (*(++ptr) == '(') break;
            if ((ptr - buffer) >= file_size) {
                fclose(in);
                fclose(out);
                return 0;   
            }
        }
        *(ptr) = '\0';
        char *arg_start = ptr + 1;
        char *args[25];
        int count = 0;
        char *function_name = NULL;
        char str[256];
        while (1)
        {
            --ptr;
            if (*ptr == '*')
            {
                strcpy(str, ptr + 1);
                function_name = str;
                *(ptr + 1) = '\0';
                break;
            }else if (isspace(*ptr))
            {
                *ptr = '\0';
                function_name = ptr + 1;
                fprintf(list, " %s,", function_name);
                break;
            }
        }
        
        while (1){
            if (*ptr == '\n' || ((ptr - buffer) < 0)){
                fprintf(out, "void %s__(void){\n", function_name);
                ++ptr;
                if (!strcmp(ptr, "void"))
                {
                    fprintf(out, "    %s(", function_name);   
                    args[0] = arg_start;
                    count = 1;
                }else
                {
                    fprintf(out, "    *(%s*)(std) = %s(", ptr, function_name);    
                    args[0] = ptr;
                    args[1] = arg_start;
                    count = 2;
                }
                break;
            }
            --ptr;
        }
        ptr = arg_start;
        int isbreak = 0;
        while (1)
        {
            if (*ptr == ','){
                args[count] = ptr + 2;
                end:;
                char *ptr2 = ptr;
                while (!(isspace(*(--ptr2)) || *ptr2 == '*'));
                *(ptr2 + 1) = '\0';
                fprintf(out, " *(%s*)(std", args[count - 1]);
                for (size_t i = 0; i < count - 1; i++)
                {
                    fprintf(out, " + sizeof(%s)", args[i]);
                }
                count++;
                if (isbreak)
                {
                    fprintf(out, ")");
                    no_param:;
                    fprintf(out, ");\n    void *tmp = buffer + ptr;\n    int tmp2 = *(int*)(tmp);\n    if(tmp2 < 0){\n        ptr = - tmp2;\n    }else{\n        imp = fun[tmp2];\n        ptr += sizeof(int);\n    }\n}\n");
                    break;
                }else
                {
                    fprintf(out, "),");
                }
            }else if (*ptr == ')'){
                isbreak = 1;
                *ptr = '\0';
                if (!strcmp(arg_start, "void"))
                {
                    goto no_param;
                }
                goto end;
            }
            ++ptr;
        }
    }
}
