#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char const *argv[])
{
    //argv[1] = "exp.c";argc = 2;//argv[1] = "exp.c";argc = 2;
    for (int i = 1; i < argc; i++) {
        printf("compiling %s,%d\n",argv[i],argc);
        FILE *in = fopen(argv[i], "rb");
        fseek(in,0,SEEK_END);
        int file_size = ftell(in);
        fseek(in,0,SEEK_SET);
        char *buffer = malloc(file_size), *ptr = buffer;
        fread(buffer,file_size,1,in);
        char output_name[256];
        char *file_name;
        snprintf(output_name, sizeof(output_name), "Runtime-lib-%s",((file_name = strrchr(argv[i], '\\')) ? (file_name += 1) : (file_name = (char*)argv[i])));
        FILE *out = fopen(output_name, "w");
        FILE *def = fopen("Runtime-Define-Package.h", "w");
        snprintf(output_name, sizeof(output_name), "Runtime-lib-statements-%s",file_name);
        FILE *sta = fopen(output_name, "w");
        fprintf(sta, "\n//%s:\n", file_name);
        snprintf(output_name, sizeof(output_name), "Runtime-lib-list-%s",file_name);
        FILE *lst = fopen(output_name, "w");
        fprintf(lst, "#define list-%s", file_name);
        fprintf(def, "#define Package(name, body, size)\\\nvoid name (void){\\\n    body\\\n    int tmp2 = *(int*)(buffer + (ptr += size));\\\n    if(tmp2 > 0){\\\n        imp = fun[tmp2];\\\n    }else{\\\n        stack[stackPtr++] = ptr;\\\n        imp = fun[*(int*)(buffer + (ptr = - tmp2))];\\\n    }\\\n}\n");
        int fist = 1;
        while (1){
            while (1){
                if (*(++ptr) == '(') break;
                if ((ptr - buffer) >= file_size) {
                    fclose(in);
                    fclose(out);
                    goto nextfile;
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
                    break;
                }
            }
            fprintf(out, "Package( %s__,",function_name);
            fprintf(sta, "void %s__(void);\n",function_name);
            if (fist){
                fprintf(lst, " %s",function_name);
                fist = 0;
            }else
            {
                fprintf(lst, ", %s",function_name);
            }
            
            
            while (1){
                if (*ptr == '\n' || ((ptr - buffer) < 0)){
                    ++ptr;
                    if (!strcmp(ptr, "void"))
                    {
                        fprintf(out, "%s(", function_name);  
                        args[0] = "int"; 
                        args[1] = arg_start;
                        count = 2;
                    }else
                    {
                        fprintf(out, "*(%s*)(buffer + ptr + sizeof(int)) = %s(", ptr, function_name);
                        args[0] = "int";
                        args[1] = ptr;
                        args[2] = arg_start;
                        count = 3;
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
                    fprintf(out, " *(%s*)(buffer + ptr", args[count - 1]);
                    for (size_t i = 0; i < count - 1; i++)
                    {
                        fprintf(out, " + sizeof(%s)", args[i]);
                    }
                    
                    if (isbreak)
                    {
                        fprintf(out, "));, sizeof(%s)", args[0]);
                        for (size_t i = 1; i < count; i++)
                        {
                            fprintf(out, " + sizeof(%s)", args[i]);
                        }
                        fprintf(out, ")\n");
                        break;
                    }else
                    {
                        fprintf(out, "),");
                    }
                    count++;
                }else if (*ptr == ';'){
                    isbreak = 1;
                    *(ptr - 1) = '\0';
                    if (!strcmp(arg_start, "void"))
                    {
                        fprintf(out, ");, sizeof(%s)", args[0]);
                        for (size_t i = 1; i < count - 1; i++)
                        {
                            fprintf(out, " + sizeof(%s)", args[i]);
                        }
                        fprintf(out, ")\n");
                        break;
                    }
                    goto end;
                }
                ++ptr;
            }
        }
        nextfile:;
    }
}
