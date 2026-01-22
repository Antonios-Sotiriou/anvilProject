#include "headers/pythonAPI/anvilPy.h"

static void Custom_dealloc(meshPy *self);
static PyObject *Custom_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
static int Custom_init(meshPy *self, PyObject *args, PyObject *kwds);
static PyObject *Custom_getcname(meshPy *self, void *closure);
static int Custom_setcname(meshPy *self, PyObject *value, void *closure);
static PyObject* Custom_getcoords(meshPy *self, void *closure);
static int Custom_setcoords(meshPy* self, PyObject *value, void *closure);
static PyObject *Custom_name(meshPy *self, PyObject* Py_UNUSED(ignored));
static meshPy *meshPy_FromMesh(mesh *m);
static PyObject *Custom_get_meshPy(PyTypeObject *type, PyObject *args);
static void meshPy_Children(meshPy *self, mesh *m);
static void meshPy_Parent(meshPy *self, mesh *m);

scene *SCENE;

static void Custom_dealloc(meshPy *self) {
    printf("Called Custom_dealloc()\n");
    printf("reference: %lld\n", self->cname->ob_refcnt);
    Py_DECREF(self->cname);
    Py_XDECREF(self->children);
    Py_XDECREF(self->parent);
    Py_TYPE(self)->tp_free((PyObject*)self);
};
static PyObject *Custom_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    printf("Called Custom_new()\n");
    meshPy *self;
    self = (meshPy*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->cname = PyUnicode_FromString("Undefined");
        if (self->cname == NULL) {
            Py_DECREF(self);
            debug_log_INFO();
            return NULL;
        }
        printf("Reached Custom New\n");
        self->number_of_children = 0;
        self->parent = NULL;

        self->coords = PyList_New(0);
        for (int i = 0; i < 16; i++) {
            if (PyList_Append(self->coords, PyFloat_FromDouble(0.f))) {
                Py_DECREF(&self->coords[i]);
                Py_DECREF(self->coords);
            }
        }
    }

    return (PyObject*)self;
}
static int Custom_init(meshPy *self, PyObject *args, PyObject *kwds) {
    printf("Called Custom_init()\n");
    static char *kwlist[] = { "name", "number_of_children", NULL };
    char *name;
    int number_of_children = 0;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|si", kwlist, &name, &number_of_children)) {
        debug_log_INFO();
        return -1;
    }

    if (name) {
        printf("Reached Custom Init\n");
        self->cname = PyUnicode_FromString(name);
        self->number_of_children = number_of_children;
        self->parent = NULL;
        //self->coords = PyList_New(64);
        //memset(&self->coords, 1, 64);
        //Py_INCREF(&self->coords);
    }
    return 0;
};
static PyMemberDef Custom_members[] = {
    //{"coords", Py_T_OBJECT_EX, offsetof(meshPy, cd), 0, "Array of 4d vectors representing the position on the world space."},
    //{"cname", Py_T_OBJECT_EX, offsetof(meshPy, cname), 0, "String. The Identification name of the Object"},
    {"number_of_children", Py_T_INT, offsetof(meshPy, number_of_children), 0, "Integer Value. Number of children that the Object owns."},
    {"children", Py_T_OBJECT_EX, offsetof(meshPy, children), 0, "Objects Array. Children of the Object"},
    {"parent", Py_T_OBJECT_EX, offsetof(meshPy, parent), 0, "Object. Parent of the Object"},
    {NULL}  /* Sentinel */
};
static PyGetSetDef Custom_getsetters[] = {
    {"cname", (getter)Custom_getcname, (setter)Custom_setcname, "Get or Set the name.", NULL},
    {"coords", (getter)Custom_getcoords, (setter)Custom_setcoords, "Get or Set the Coordinates.", NULL},

    {NULL}  /* Sentinel */
};
static PyMethodDef Custom_methods[] = {
    //{ "parent", (PyCFunction)meshPy_Parent, METH_NOARGS, "Object.Return the parent of the Object.\n" },
    //{ "name", (PyCFunction)Custom_name, METH_NOARGS, "Return the name of the meshPy\n" },
    {NULL}  /* Sentinel */
};
static PyMethodDef Custom_module_methods[] = {
    { "get", (PyCFunction)Custom_get_meshPy, METH_VARARGS, "Return the index meshPy of the index model. get(int model_index, int mesh_index).\n" },
    {NULL}  /* Sentinel */
};
static PyObject *Custom_getcname(meshPy *self, void *closure) {
    printf("Called Custom_getname()\n");
    return Py_NewRef(self->cname);
};
static int Custom_setcname(meshPy *self, PyObject *value, void *closure) {
    printf("Called Custom_setname()\n");
    if (value == NULL) {
        PyErr_SetString(PyExc_TypeError, "Cannot delete the first attribute");
        return -1;
    }
    if (!PyUnicode_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "The first attribute value must be a string");
        return -1;
    }
    Py_SETREF(self->cname, Py_NewRef(value));
    return 0;
};
static PyObject *Custom_getcoords(meshPy *self, void *closure) {
    int test = closure;
    printf("Called Custom_getcoords(%d)\n", test);
    return Py_NewRef(self->coords);
};
static int Custom_setcoords(meshPy* self, PyObject *value, void *closure) {
    //float new_value = 0.f;
    //PyArg_ParseTuple(value, "i", &new_value);
    printf("Called Custom_setcoords()\n");

    if (!PyFloat_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "Value must be a float");
        return -1;
    }
    Py_SETREF(self->coords, Py_NewRef(value));
    return 0;
};
static PyObject *Custom_name(meshPy *self, PyObject* Py_UNUSED(ignored)) {
    printf("Called Custom_name()\n");
    if (self->cname == NULL) {
        PyErr_SetString(PyExc_AttributeError, "name");
        return NULL;
    }
    return PyUnicode_FromFormat("%s", self->cname);
}
static void meshPy_Children(meshPy *self, mesh *m) {
    if (!m->children) {
        self->children = NULL;
        return;
    }

    self->children = PyList_New(0);
    for (int i = 0; i < self->number_of_children; i++) {
        meshPy *list_entry = meshPy_FromMesh(m->children[i]);
        if (PyList_Append(self->children, (PyObject*)list_entry) != 0) {
            debug_log_INFO();
            Py_DECREF(self->children[i]);
            Py_DECREF(self->children);
        }
        Py_DECREF(list_entry);
        //if (self->children[i]->children) {
        //    meshPy_Children(self, self->children[i]);
        //}
    }
}
static void meshPy_Parent(meshPy *self, mesh *m) {
    if (!m->parent) {
        self->parent = NULL;
        return;
    }

    self->parent = meshPy_FromMesh(m->parent);
}
static PyObject *Custom_get_meshPy(PyTypeObject *type, PyObject *args) {
    printf("Called Custom_get_meshPy()\n");
    int model_idx, mesh_idx;
    if (!PyArg_ParseTuple(args, "|ii", &model_idx, &mesh_idx)) {
        debug_log_INFO();
        return NULL;
    }
    printf("gathering information for %s\n", SCENE->model[model_idx].mesh[mesh_idx].cname);

    meshPy *self = meshPy_FromMesh(&SCENE->model[model_idx].mesh[mesh_idx]);
    meshPy_Children(self, &SCENE->model[model_idx].mesh[mesh_idx]);
    meshPy_Parent(self, &SCENE->model[model_idx].mesh[mesh_idx]);
    //self->coords = PyList_New(64);
    //Py_INCREF(self->coords);
    memcpy(&self->coords, &SCENE->model[model_idx].mesh[mesh_idx].coords, 64);

    return (PyObject*)self;
}
static PyTypeObject meshPyType = {
    .ob_base = PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "meshPy",
    .tp_doc = PyDoc_STR("Private Custom OBJ"),
    .tp_basicsize = sizeof(meshPy),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new = Custom_new,
    .tp_init = (initproc)Custom_init,
    .tp_dealloc = (destructor)Custom_dealloc,
    .tp_members = Custom_members,
    .tp_methods = Custom_methods,
    .tp_getset = Custom_getsetters,
};
static PyModuleDef custommodule = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "meshPy",
    .m_doc = "Example module that creates an extension type.",
    .m_size = -1,
    .m_methods = Custom_module_methods,
};
static meshPy *meshPy_FromMesh(mesh *m) {
    printf("Called meshPy_FromMesh()\n");
    meshPy *self = (meshPy*)meshPyType.tp_alloc(&meshPyType, 0);
    if (!self) {
        Py_DECREF(self);
        debug_log_INFO();
        return NULL;
    } else {
        self->coords = PyList_New(0);
        for (int i = 0; i < 4; i++) {
            for (int y = 0; y < 4; y++) {
                if (PyList_Append(self->coords, PyFloat_FromDouble(m->coords.v[i].m128_f32[y]))) {
                    Py_DECREF(&self->coords[i]);
                    Py_DECREF(self->coords);
                }
            }
        }

        self->q                     = m->q;
        self->scale                 = m->scale;
        self->model_matrix          = m->model_matrix;
        self->cname                 = PyUnicode_FromString(m->cname);
        self->vbo                   = m->vbo;
        self->outer_radius          = m->outer_radius;
        self->length_cname          = m->cname_length;
        self->vbo_indexes           = m->vbo_indexes;
        self->faces_indexes         = m->faces_indexes;
        self->vecs_indexes          = m->vecs_indexes;
        self->vbo_size              = m->vbo_size;
        self->pk                    = m->pk;
        self->asset_type            = m->asset_type;
        self->mesh_type             = m->mesh_type;
        self->visible               = m->visible;
        self->owns_anim             = m->owns_anim;
        self->VAO                   = m->VAO;
        self->VBO                   = m->VBO;
        self->rigid                 = m->rigid;
        self->children              = m->children;
        self->parent                = m->parent;
        self->anim                  = m->anim;
        self->number_of_children    = m->number_of_children;
        self->owns_rigid            = m->owns_rigid;
    }
    return self;
}
PyMODINIT_FUNC PyInit_meshPy(void) {
    printf("Called PyInit_meshPy()\n");
    PyObject *m;
    if (PyType_Ready(&meshPyType) < 0) {
        printf("CRITICAL:%s:%d:%s()\n", __FILE__, __LINE__, __func__);
        return NULL;
    }

    m = PyModule_Create(&custommodule);
    if (m == NULL) {
        printf("CRITICAL:%s:%d:%s()\n", __FILE__, __LINE__, __func__);
        return NULL;
    }

    if (PyModule_AddObjectRef(m, "New", (PyObject*)&meshPyType) < 0) {
        Py_DECREF(m);
        printf("CRITICAL:%s:%d:%s()\n", __FILE__, __LINE__, __func__);
        return NULL;
    }

    return m;
}
const static int initConfig(PyConfig config) {
    PyStatus status;
    PyConfig_InitPythonConfig(&config);

    status = PyConfig_SetBytesString(&config, &config.program_name, PROJECT_NAME);
    if (PyStatus_Exception(status)) {
        printf("CRITICAL:%s:%d:%s()\n", __FILE__, __LINE__, __func__);
        Py_ExitStatusException(status);
    }
    status = PyConfig_SetBytesString(&config, &config.home, PYTHONHOME);
    if (PyStatus_Exception(status)) {
        printf("CRITICAL:%s:%d:%s()\n", __FILE__, __LINE__, __func__);
        Py_ExitStatusException(status);
    }
    /* Read all configuration at once */
    status = PyConfig_Read(&config);
    if (PyStatus_Exception(status)) {
        printf("CRITICAL:%s:%d:%s()\n", __FILE__, __LINE__, __func__);
        Py_ExitStatusException(status);
    }
    status = Py_InitializeFromConfig(&config);
    if (PyStatus_Exception(status)) {
        printf("CRITICAL:%s:%d:%s()\n", __FILE__, __LINE__, __func__);
        Py_ExitStatusException(status);
    }

    PyConfig_Clear(&config);
    return 0;
}

#if defined(WIN32) || defined(_WIN32) || defined(_WIN64) // ################################ WINDOWS #################################################
DWORD WINAPI startPythonAPI(void *args);

/* Create a seperate thread to run the Server. */
HANDLE thread;

void enablePythonAPI(scene *s) {
    thread = CreateThread(NULL, 0, startPythonAPI, s, 0, NULL);
    if (thread == NULL) {
        debug_log_critical(stderr, "HANDLE thread = CreateThread(NULL, 0, startPythonAPI, s, 0, NULL)");
        exit(-1);
    }
}
void disablePythonAPI(void) {
    WaitForSingleObject(thread, 0);
    CloseHandle(thread);
}
static DWORD WINAPI startPythonAPI(void *args) {
    PyImport_AppendInittab("vec4Py", &PyInit_vec4Py);
    PyImport_AppendInittab("meshPy", &PyInit_meshPy);
    scene *s = args;
    SCENE = s;

    PyConfig config = { 0 };
    if (initConfig(config)) {
        printf("Error while Initializing config.\n");
        return 1;
    }

    Py_Initialize();

    PyRun_SimpleString("import vec4Py\n");
    PyRun_SimpleString("import meshPy\n");
    //PyRun_SimpleString("mesh = meshPy.get(0, 3)\n");
    //PyRun_SimpleString("print(mesh.parent.cname)\n");

    return Py_RunMain();
}
#elif defined(LINUX) || defined(__linux__) // ################################ LINUX #################################################
    #include <threads.h>
#endif // ################################ WINDOWS #################################################


