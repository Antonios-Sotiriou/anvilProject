#include "headers/components/animations.h"

void readBlenderAnimations(animationData *ad, char path[]) {
	printf("path: %s\n", path);
}
void loadModelAnimations(model* m) {
    int path_length = (strlen(m->cname) * 2) + 35; // Plus 1 here for the null termination \0.
    char* dynamic_path = malloc(path_length);
    if (!dynamic_path) {
        debug_log_error(stdout, "char *dynamic_path = malloc(path_length)");
        debug_log_info(stdout, "%s\n", m->cname);
        return;
    }
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
    sprintf_s(dynamic_path, path_length, "models/%s/animations/%s_animations.txt", m->cname, m->cname);
#elif defined(LINUX) || defined(__linux__)
    snprintf(dynamic_path, path_length, "models/%s/animations/%s_animations.txt", m->cname, m->cname);
#endif

    animationData ad = { 0 };
    readBlenderAnimations(&ad, dynamic_path);
    free(dynamic_path);


	//releaseBlenderAnimations(&ad);
}
void releaseBlenderAnimations(animationData *ad) {
	free(ad->anim->location);
	free(ad->anim->rotation_quaternion);
	free(ad->anim->scale);
	free(ad->anim);
}


