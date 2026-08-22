#include <libdragon.h>

extern const char **environ;

int main(int argc, char **argv) {
    debug_init_emulog();
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 2, GAMMA_NONE,
                 FILTERS_DISABLED);
    console_init();

    printf("env:\n");

    int i = 0;
    while (1) {
        const char *str = environ[i++];
        if (str == NULL)
            break;
        printf("%s\n", str);
    }

    printf("%d args:\n", argc);

    for (i = 0; i < argc; i++) {
        const char *str = argv[i];
        printf("%p %s\n", str, str);
    }

    while (1)
        ;
}
