#include <stdio.h>
#include <string.h>

/* --- 1. SABLONOK TÁROLÁSA --- */
const char *c_kod[] = { "#include <stdio.h>", "", "int main(void) {", "    printf(\"Hello, C!\\n\");", "    return 0;", "}", NULL };
const char *java_kod[] = { "public class Main {", "    public static void main(String[] args) {", "        System.out.println(\"Hello, Java!\");", "    }", "}", NULL };
const char *py_kod[] = { "#!/usr/bin/env python3", "def main():", "    print(\"Hello, Python!\")", "", "if __name__ == '__main__':", "    main()", NULL };
const char *sh_kod[] = { "#!/bin/bash", "", "echo \"Hello, Bash!\"", NULL };

/* --- 2. ADATSZERKEZET --- */
typedef struct {
    const char *id;
    const char *desc;
    const char *filename;
    const char **lines;
} Template;

// Csak a kért 4 nyelv maradt, magyar leírással
Template templates[] = {
    {"c",    "C forráskód",        "main.c",    c_kod},
    {"java", "Java forráskód",     "Main.java", java_kod},
    {"py",   "Python 3 forráskód", "main.py",   py_kod},
    {"sh",   "Bash forráskód",     "main.sh",   sh_kod}
};

int num_templates = sizeof(templates) / sizeof(templates[0]);

/* --- 3. SÚGÓ KIÍRÁSA (Magyarítva) --- */
void print_help() {
    printf("alap v0.1.9,\n\n");
    printf("Használat: alap <sablon_id> [opció]\n\n");
    printf("Elérhető opciók:\n\n");
    printf("-h, --help      súgó megjelenítése\n");
    printf("-v, --version   verzióinformáció\n");
    printf("--stdout        ne hozzon létre fájlt, kimenet a terminálba\n\n");
    printf("Elérhető sablonok:\n\n");
    
    for (int i = 0; i < num_templates; i++) {
        // A %-9s biztosítja, hogy a kötőjelek pont egy oszlopba essenek
        printf("* %-9s - %s [%s]\n", templates[i].id, templates[i].desc, templates[i].filename);
    }
}

/* --- 4. KÓD GENERÁLÁSA --- */
void generate_code(Template t, int to_stdout) {
    FILE *out = stdout; 
    
    if (!to_stdout) {
        // 1. lépés: Ellenőrizzük, hogy a fájl létezik-e
        FILE *check = fopen(t.filename, "r");
        if (check != NULL) {
            fclose(check);
            // Magyar hibaüzenet, ha a fájl már létezik
            fprintf(stderr, "Hiba: a(z) '%s' fájl már létezik\n", t.filename);
            return;
        }

        // 2. lépés: Fájl létrehozása
        out = fopen(t.filename, "w");
        if (out == NULL) {
            fprintf(stderr, "Hiba: Nem sikerült létrehozni a(z) '%s' fájlt!\n", t.filename);
            return;
        }
    }

    // Kiírjuk a sablont soronként
    for (int i = 0; t.lines[i] != NULL; i++) {
        fprintf(out, "%s\n", t.lines[i]);
    }

    // Fájl lezárása és sikeres zöld üzenet kiírása magyarul
    if (!to_stdout) {
        fclose(out);
        // ANSI escape kódok: \033[32m a zöld kezdete, \033[0m a szín visszaállítása (reset)
        printf("\033[32m# a(z) '%s' sikeresen létrehozva\033[0m\n", t.filename);
    }
}

/* --- 5. FŐPROGRAM --- */
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
            printf("alap v0.1.9\n");
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
        // Ha rossz azonosítót adnak meg
        fprintf(stderr, "Hiba: ismeretlen sablon: '%s'\n", template_id);
    } else {
        // Ha csak egy --stdout parancsot adtak nyelv nélkül
        fprintf(stderr, "Hiba: a sablon_id hiányzik.\n");
    }

    return 1;
}
