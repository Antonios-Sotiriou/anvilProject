#include "headers/components/texture.h"

void loadModelTextureAtlas(model *m) {
	printf("Loading texture atlas for model: %s\n", m->cname);

    int path_length = (strlen(m->cname) * 2) + 27; // Plus 1 here for the null termination \0.
    char *dynamic_path = malloc(path_length);
    if (!dynamic_path) {
        debug_log_error(stdout, "Could not allocate memory for Texture path.");
        return;
    }

    anvil_snprintf(dynamic_path, path_length, "models/%s/%s_texture_atlas.bmp", m->cname, m->cname);

    printf("%s texture atlas path: %s\n", m->cname, dynamic_path);
    BMP bmp;
    readBMP(&bmp, dynamic_path);
    free(dynamic_path);

    glGenTextures(1, &m->texture_atlas);
    glBindTexture(GL_TEXTURE_2D, &m->texture_atlas);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, bmp.info.Width, bmp.info.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bmp.data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glGenerateMipmap(GL_TEXTURE_2D);
}