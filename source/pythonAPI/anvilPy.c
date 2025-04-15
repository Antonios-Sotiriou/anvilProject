#include "headers/pythonAPI/anvilPy.h"

//static void Custom_dealloc(meshPy *self);
//static PyObject *Custom_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
//static int Custom_init(meshPy *self, PyObject *args, PyObject *kwds);
//static PyMemberDef Custom_members;
//static PyObject *Custom_getname(meshPy *self, void *closure);
//static int Custom_setname(meshPy *self, PyObject *value, void *closure);
//static PyGetSetDef Custom_getsetters;
//static PyObject *Custom_name(meshPy *self, PyObject* Py_UNUSED(ignored));
//static meshPy *meshPy_FromMesh(mesh *m);
//static PyObject *Custom_get_meshPy(PyTypeObject *type, PyObject *args);
//static PyMethodDef Custom_methods;
static PyTypeObject meshPyType;
//static PyMethodDef Custom_module_methods;
//static PyModuleDef custommodule;

static void Custom_dealloc(meshPy *self) {
    printf("Called Custom_dealloc()\n");
    printf("reference: %lld\n", self->cname->ob_refcnt);
    Py_DECREF(self->cname);
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
        self->number_of_children = 0;
        self->parent = NULL;
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
        self->cname = PyUnicode_FromString(name);
        self->number_of_children = number_of_children;
        self->parent = NULL;
    }
    return 0;
};
static PyMemberDef Custom_members[] = {
    {"cname", Py_T_OBJECT_EX, offsetof(meshPy, cname), 0, "String. The Identification name of the Object"},
    {"number_of_children", Py_T_INT, offsetof(meshPy, number_of_children), 0, "Integer Value. Number of children that the Object owns."},
    {"children", Py_T_OBJECT_EX, offsetof(meshPy, children), 0, "Objects Array. Children of the Object"},
    {"parent", Py_T_OBJECT_EX, offsetof(meshPy, parent), 0, "Object. Parent of the Object"},
    {NULL}  /* Sentinel */
};
static PyObject *Custom_getname(meshPy *self, void *closure) {
    printf("Called Custom_getname()\n");
    return Py_NewRef(self->cname);
};
static int Custom_setname(meshPy *self, PyObject *value, void *closure) {
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
static PyGetSetDef Custom_getsetters[] = {
    {"getset_name", (getter)Custom_getname, (setter)Custom_setname, "Get or Set the name.", NULL},

    {NULL}  /* Sentinel */
};
static PyObject *Custom_name(meshPy *self, PyObject *Py_UNUSED(ignored)) {
    printf("Called Custom_name()\n");
    if (self->cname == NULL) {
        PyErr_SetString(PyExc_AttributeError, "name");
        return NULL;
    }
    return PyUnicode_FromFormat("%s", self->cname);
}
static meshPy *meshPy_FromMesh(mesh *m) {
    printf("Creating %s\n", m->cname);
    meshPy *self;
    self = (meshPy*)meshPyType.tp_alloc(&meshPyType, 0);
    if (self != NULL) {
        self->cname = PyUnicode_FromString(m->cname);
        if (!self->cname) {
            Py_DECREF(self->cname);
            Py_DECREF(self);
            debug_log_INFO();
            return NULL;
        }

        self->number_of_children = m->number_of_children;

        //if (!m->parent)
        //    self->parent = NULL;
    }

    return self;
}
static PyObject *childrenPy_FromMesh(mesh *m) {
    PyObject *temp_list = PyList_New(0);
    if (!temp_list) {
        Py_DECREF(temp_list);
    } else {
        for (int i = 0; i < m->number_of_children; i++) {
            //PyObject *list_entry = Py_BuildValue("O", meshPy_FromMesh(m->children[i]));
            if (PyList_Append(temp_list, (PyObject*)meshPy_FromMesh(m->children[i])) != 0) {
                debug_log_INFO();
                //Py_DECREF(list_entry);
                Py_DECREF(temp_list);
            }
            //Py_DECREF(list_entry);
            printf("Added %s to %s\n", m->children[i]->cname, m->cname);
        }
    }
    return temp_list;
}
static PyObject *Custom_get_meshPy(PyTypeObject *type, PyObject *args) {
    printf("Called Custom_get_meshPy()\n");
    int model_idx, mesh_idx;
    if (!PyArg_ParseTuple(args, "|ii", &model_idx, &mesh_idx)) {
        debug_log_INFO();
        return NULL;
    }
    printf("gathering information for %s\n", SCENE.model[model_idx].mesh[mesh_idx].cname);

    //meshPy *self = { 0 };
    meshPy *self = meshPy_FromMesh(&SCENE.model[model_idx].mesh[mesh_idx]);
    if (!self)
        Py_DECREF(self);
    if (SCENE.model[model_idx].mesh[mesh_idx].number_of_children)
        self->children = childrenPy_FromMesh(&SCENE.model[model_idx].mesh[mesh_idx]);
    //if (self->parent != NULL) {
        //PyObject *temp_parent = Py_BuildValue("O", meshPy_FromMesh(SCENE.model[model_idx].mesh[mesh_idx].parent));
        self->parent = (PyObject*)meshPy_FromMesh(SCENE.model[model_idx].mesh[mesh_idx].parent);
        //Py_DECREF(temp_parent);
    //}

    return (PyObject*)self;
}
static PyMethodDef Custom_methods[] = {
    { "get", (PyCFunction)Custom_get_meshPy, METH_VARARGS, "Return the meshPy with the given name. Name must be string.\n" },
    //{ "name", (PyCFunction)Custom_name, METH_NOARGS, "Return the name of the meshPy\n" },

    {NULL}  /* Sentinel */
};
static PyTypeObject meshPyType = {
    .ob_base = PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "meshPy.meshPy",
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
static PyMethodDef Custom_module_methods[] = {
    { "get", (PyCFunction)Custom_get_meshPy, METH_VARARGS, "Return the meshPy with the given name. Name must be string.\n" },
    //{ "cname", (PyCFunction)Custom_name, METH_NOARGS, "Return the name of the meshPy\n" },

    {NULL}  /* Sentinel */
};
static PyModuleDef custommodule = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "meshPy",
    .m_doc = "Example module that creates an extension type.",
    .m_size = -1,
    .m_methods = Custom_module_methods,
};
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

void enablePythonAPI(void) {
    thread = CreateThread(NULL, 0, startPythonAPI, NULL, 0, NULL);
    if (thread == NULL) {
        debug_log_critical(stderr, "HANDLE thread = CreateThread(NULL, 0, startTCPServer, NULL, 0, NULL)");
        exit(-1);
    }
}
void disablePythonAPI(void) {
    WaitForSingleObject(thread, 0);
    CloseHandle(thread);
}
static DWORD WINAPI startPythonAPI(void *args) {
    PyImport_AppendInittab("meshPy", &PyInit_meshPy);

    PyConfig config = { 0 };
    if (initConfig(config)) {
        printf("Error while Initializing config.\n");
        return 1;
    }

    Py_Initialize();

    PyRun_SimpleString("import meshPy\n");
    PyRun_SimpleString("mesh = meshPy.get(0, 3)\n");

    return Py_RunMain();
}
#elif defined(LINUX) || defined(__linux__) // ################################ LINUX #################################################
    #include <threads.h>
#endif // ################################ WINDOWS #################################################


