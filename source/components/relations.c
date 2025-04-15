#include "headers/components/relations.h"

/* Add a Child mesh < m > to children of target < t > and assign target < t > as the parent of < m >. */
int addChild(mesh *t, mesh *m) {
	if (strncmp(m->cname, t->cname, strlen(t->cname)) == 0) {
		printf("mesh [%s] can't have hes self as a child\n", m->cname);
		return 1;
	}
	if (m->parent && (m->parent == t)) {
		if (strncmp(m->parent->cname, t->cname, strlen(t->cname)) == 0) {
			printf("mesh [%s] is already a child of [%s]\n", m->cname, t->cname);
			return 2;
		}
	} else {
		for (int i = 0; i < m->number_of_children; i++) {
			if (strncmp(t->cname, m->children[i]->cname, strlen(t->cname)) == 0) {
				printf("mesh [%s] can't have hes child [%s] as a parent. Remove mesh child first.\n", m->cname, t->cname);
				return 3;
			}
		}
	}

	if (t->number_of_children == 0) {
		t->children = malloc(8);
		if (t->children) {
			t->children[0] = m;
		}
		t->number_of_children = 1;
		printf("mesh [%s] added to children of [%s]\n", m->cname, t->cname);
	} else {
		mesh **temp_t = realloc(t->children, 8 * (t->number_of_children + 1));
		if (!temp_t) {
			free(t->children);
			printf("Error: mesh* temp_t = realloc(t->children, sizeof(mesh) * (t->number_of_children + 1))");
			return 4;
		} else {
			t->children = temp_t;

			t->children[t->number_of_children] = m;
			t->number_of_children++;
			printf("Reallocating children of [%s] to add [%s]\n", t->cname, m->cname);
		}
	}
	m->parent = t;
	return 0;
}
/* Remove a Child mesh < m > from children of target < t > and reset the parent of < m >. */
int removeChild(mesh *t, mesh *m) {
	//if ( (strncmp(m->parent->cname, t->cname, strlen(m->parent->cname)) != 0) && (strncmp(m->cname, t->cname, strlen(m->cname)) != 0)) {
	if (m == t) {
		printf("mesh [%s] cant have hes self as parent\n", m->cname);
		return 1;
	}
	if (m->parent != t) {
		printf("mesh [%s] not a child of [%s]\n", m->cname, t->cname);
		return 2;
	}
	if (t->number_of_children > 0) {
		int idx = 0, dynamic_inc = 1;
		size_t ch_size = 8 * t->number_of_children;

		mesh **temp_a = malloc(ch_size);
		if (!temp_a) {
			printf("Could not allocate memory for children");
			return 3;
		}
		memcpy(temp_a, t->children, ch_size);
		free(t->children);
		t->children = malloc(8 * (t->number_of_children - 1));

		for (int i = 0; i < t->number_of_children; i++) {
			if (strncmp(temp_a[i]->cname, m->cname, strlen(m->cname)) != 0) {

				mesh **temp_b = realloc(t->children, 8 * dynamic_inc);
				if (!temp_b) {
					printf("Could not reallocate memory for new children array");
					free(temp_a);
					free(t->children);
					return 4;
				} else {
					t->children = temp_b;

					t->children[idx] = temp_a[i];
					idx++;
					dynamic_inc++;
				}
			}
		}
		t->number_of_children = idx;
		free(temp_a);
	}
	m->parent = NULL;
	printf("mesh [%s] removed from children of [%s]\n", m->cname, t->cname);
	return 0;
}


