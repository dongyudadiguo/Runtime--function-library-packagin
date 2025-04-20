#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char const *argv[])
{
    //argv[1] = "exp.c";//argv[1] = "exp.c";
    FILE *in = fopen(argv[1], "r");
    fseek(in,0,SEEK_END);
    int file_size = ftell(in);
    fseek(in,0,SEEK_SET);
    char *buffer = malloc(file_size), *ptr = buffer;
    fread(buffer,file_size,1,in);
    char output_name[256];
    snprintf(output_name, sizeof(output_name), "Runtime-lib-%s", argv[1]);
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
        while (isspace(*(--ptr)));
        *(ptr + 1) = '\0';
        
        char *function_name = NULL;
        char *tmp = NULL;
        char *return_type = NULL;
        char *args[25];
        int count = 0;
        while (1){
            --ptr;
            if (tmp == NULL && function_name && isalpha(*ptr))
            {
                tmp = ptr + 1;
            }else if (tmp == NULL && *ptr == '*')
            {
                if (function_name == NULL)function_name = ptr + 1;
                tmp = ptr + 1;
            }else if (function_name == NULL && isspace(*ptr))
            {
                function_name = ptr + 1;
            }else if (tmp && *ptr == '_')
            {
                tmp = NULL;
            }else if (*ptr == '\n' || isupper(*ptr) || ((ptr - buffer) < 0)){
                while (isspace(*(++ptr)));
                char function_name_bak[256];
                function_name = strcpy(function_name_bak, function_name);
                *tmp = '\0';
                fprintf(out, "void %s__(void){\n", function_name);
                if (strcmp(ptr, "void"))
                {
                    return_type = ptr;
                    fprintf(out, "    *(%s*)(std) = %s( ", return_type, function_name);    
                    args[0] = return_type;
                    args[1] = arg_start;
                    count = 2;
                }else
                {
                    fprintf(out, "    %s( ", function_name);   
                    args[0] = arg_start;
                    count = 1;
                }
                break;
            }
        }
        
        ptr = arg_start;
        int isbreak = 0;
        while (1)
        {
            ++ptr;
            if (*ptr == ','){
                void *tmp = ptr;
                while (isspace(*(++ptr)));
                args[count] = ptr;
                ptr = tmp;
                gt:;
                while (isspace(*(--ptr)));
                while (1)
                {
                    --ptr;
                    if (isspace(*ptr))break;
                    if (*ptr == '*')break;
                }
                *(ptr + 1) = '\0';
                fprintf(out, "*(%s*)(std", args[count - 1]);
                for (size_t i = 0; i < count - 1; i++)
                {
                    fprintf(out, " + sizeof(%s)", args[i]);
                }
                count++;
                if (isbreak)
                {
                    fprintf(out, ")");
                    fprintf(out, ");\n    void *tmp = buffer + ptr;\n    int tmp2 = *(int*)(tmp);\n    if(tmp2 < 0){\n        ptr = - tmp2;\n    }else{\n        imp = fun[tmp2];\n        ptr += sizeof(int);\n    }\n}\n");
                    break;
                }else
                {
                    fprintf(out, "),");
                }
                ptr = tmp + 1;
            }else if (*ptr == ')'){
                isbreak = 1;
                goto gt;
            }
            
        }
    }
}