#include <stdio.h>
#include <string.h>

const char *c_kod[] = { "#include <stdio.h>", "", "int main(void) {", "    printf(\"Hello, C!\\n\");", "    return 0;", "}", NULL };
const char *java_kod[] = { "public class Main {", "    public static void main(String[] args) {", "        System.out.println(\"Hello, Java!\");", "    }", "}", NULL };
const char *py_kod[] = { "#!/usr/bin/env python3", "def main():", "    print(\"Hello, Python!\")", "", "if __name__ == '__main__':", "    main()", NULL };
const char *sh_kod[] = { "#!/bin/bash", "", "echo \"Hello, Bash!\"", NULL };

typedef struct {
    const char *id;
    const char *desc;
    const char *filename;
    const char **lines;
} Template;

Template templates[] = {
    {"c",    "C forráskód",        "main.c",    c_kod},
    {"java", "Java forráskód",     "Main.java", java_kod},
    {"py",   "Python 3 forráskód", "main.py",   py_kod},
    {"sh",   "Bash forráskód",     "main.sh",   sh_kod}
};

int num_templates = sizeof(templates) / sizeof(templates[0]);

void print_help() {
    printf("alap v0.1.9,\n\n");
    printf("Használat: alap <sablon_id> [opció]\n\n");
    printf("Elérhető opciók:\n\n");
    printf("-h, --help      súgó megjelenítése\n");
    printf("-v, --version   verzióinformáció\n");
    printf("--stdout        ne hozzon létre fájlt, kimenet a terminálba\n\n");
    printf("Elérhető sablonok:\n\n");
    
    for (int i = 0; i < num_templates; i++) {
        printf("* %-9s - %s [%s]\n", templates[i].id, templates[i].desc, templates[i].filename);
    }
}

void generate_code(Template t, int to_stdout) {
    FILE *out = stdout; 
    
    if (!to_stdout) {
        FILE *check = fopen(t.filename, "r");
        if (check != NULL) {
            fclose(check);
            fprintf(stderr, "Hiba: a(z) '%s' fájl már létezik\n", t.filename);
            return;
        }

        out = fopen(t.filename, "w");
        if (out == NULL) {
            fprintf(stderr, "Hiba: Nem sikerült létrehozni a(z) '%s' fájlt!\n", t.filename);
            return;
        }
    }

    for (int i = 0; t.lines[i] != NULL; i++) {
        fprintf(out, "%s\n", t.lines[i]);
    }

    if (!to_stdout) {
        fclose(out);
        printf("\033[32m# a(z) '%s' sikeresen létrehozva\033[0m\n", t.filename);
    }
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        print_help();
        return 0;
    }

    int to_stdout = 0;
    char *template_id = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_help();
            return 0;
        } 
        else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
            printf("alap v0.1\n");
            return 0;
        } 
        else if (strcmp(argv[i], "--stdout") == 0) {
            to_stdout = 1;
        } 
        else {
            template_id = argv[i]; 
        }
    }

    if (template_id != NULL) {
        for (int i = 0; i < num_templates; i++) {
            if (strcmp(template_id, templates[i].id) == 0) {
                generate_code(templates[i], to_stdout);
                return 0;
            }
        }
        fprintf(stderr, "Hiba: ismeretlen sablon: '%s'\n", template_id);
    } else {
        fprintf(stderr, "Hiba: a sablon_id hiányzik.\n");
    }

    return 1;
}
