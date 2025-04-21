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

    while (1){
        while (1){
            if (*(++ptr) == '(') break;
            if ((ptr - buffer) >= file_size) {
                fclose(in);
                fclose(out);
                return 0;   
            }
        }
        char *arg_start = ptr + 1;
        *(ptr) = '\0';
        while (1){
            if (tmp == NULL && *ptr == '*')
            {
                tmp = ptr + 1;
            }else if (tmp == NULL && isspace(*ptr))
            {
                tmp = ptr + 1;
            }else if (*ptr == '\n' || ((ptr - buffer) < 0)){
                fprintf(out, "void %s__(void){\n", tmp);
                char function_name_bak[256];
                function_name = strcpy(function_name_bak, tmp);
                *tmp = '\0';
                ++ptr;
                if (!strcmp(ptr, "void"))
                {
                    fprintf(out, "    %s(", function_name);   
                    args[0] = arg_start;
                    count = 1;
                }else
                {
                    return_type = ptr;
                    fprintf(out, "    *(%s*)(std) = %s(", return_type, function_name);    
                    args[0] = return_type;
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
                ptr = tmp;
                end:;

                while (isspace(*(--ptr)) || *ptr == '*');
                *(ptr - 1) = '\0';
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
                ptr = tmp + 1;
            }else if (*ptr == ')'){
                isbreak = 1;
                while (isspace(*(--ptr)));
                *(ptr + 1) = '\0';
                if (!strcmp(arg_start, "void"))
                {
                    goto no_param;
                }
                goto end;
            }
        }
        ++ptr;
    }
}
