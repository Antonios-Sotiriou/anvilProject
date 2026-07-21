#include "headers/components/texture.h"

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
    printf("texture_atlas: %d\n", m->texture_atlas);
    glBindTexture(GL_TEXTURE_2D, m->texture_atlas);

#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, bmp.info.Width, bmp.info.Height, 0, GL_BGR, GL_UNSIGNED_BYTE, bmp.data);
#else
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, bmp.info.Width, bmp.info.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, bmp.data);
#endif
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    releaseBMP(&bmp);
}