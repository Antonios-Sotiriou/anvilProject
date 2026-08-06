#include "headers/components/texture.h"
#include "headers/components/internal_libraries/bmp.h"
#include "headers/flags.h"

#include <stdio.h>

void loadModelTextureAtlas(model *m) {
    int path_length = (strlen(m->cname) * 2) + 27; // Plus 1 here for the null termination \0.
    char *dynamic_path = malloc(path_length);
    if (!dynamic_path) {
        debug_log_error(stdout, "Could not allocate memory for Texture path.");
        return;
    }

    anvil_snprintf(dynamic_path, path_length, "models/%s/%s_texture_atlas.bmp", m->cname, m->cname);

    BMP bmp;
    readBMP(&bmp, dynamic_path);
    free(dynamic_path);

    glGenTextures(1, &m->texture_atlas);
    glBindTexture(GL_TEXTURE_2D, m->texture_atlas);

#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, bmp.info.Width, bmp.info.Height, 0, GL_BGRA, GL_UNSIGNED_BYTE, bmp.data);
#else
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, bmp.info.Width, bmp.info.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, bmp.data);
#endif
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    releaseBMP(&bmp);
}
void loadDefaultModelTexture(model *m) {
    char dynamic_path[35];
    anvil_snprintf(dynamic_path, 35, "textures/default_texture_atlas.bmp");

    BMP bmp;
    readBMP(&bmp, dynamic_path);

    glGenTextures(1, &m->texture_atlas);
    glBindTexture(GL_TEXTURE_2D, m->texture_atlas);

#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, bmp.info.Width, bmp.info.Height, 0, GL_BGRA, GL_UNSIGNED_BYTE, bmp.data);
#else
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, bmp.info.Width, bmp.info.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, bmp.data);
#endif

    glBindTexture(GL_TEXTURE_2D, 0);

    releaseBMP(&bmp);
}