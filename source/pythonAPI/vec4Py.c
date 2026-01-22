#include "headers/pythonAPI/anvilPy.h"

static void vec4Py_Dealloc(vec4Py *self);
static PyObject *vec4Py_New(PyTypeObject *type, PyObject *args, PyObject *kwds);
static int vec4Py_Init(vec4Py *self, PyObject *args, PyObject *kwds);
static PyObject *vec4Py_get(vec4Py *self, void *closure);
static int vec4Py_set(vec4Py *self, PyObject *value, void *closure);
static PyObject* vec4Py_getx(vec4Py* self, void* closure);
static int vec4Py_setx(vec4Py* self, PyObject* value, void* closure);
static PyObject* get_vec4Py(PyTypeObject* type, PyObject* args);
static vec4Py *vec4Py_FromVec4(vec4 *v);

static void vec4Py_Dealloc(vec4Py *self) {
    printf("Called vec4Py_Dealloc()\n");
    printf("reference: %lld\n", self->v->ob_refcnt);
    Py_XDECREF(self->v);
    Py_TYPE(self)->tp_free((PyObject*)self);
};
static PyObject *vec4Py_New(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    printf("Called vec4Py_New()\n");
    vec4Py *self;
    self = (vec4Py*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->v = PyList_New(0);
        for (int i = 0; i < 4; i++) {
            if (PyList_Append(self->v, PyFloat_FromDouble(i + 0.2f))) {
                Py_DECREF(&self->v[i]);
                Py_DECREF(self->v);
            }
        }
    }

    return (PyObject*)self;
}
static int vec4Py_Init(vec4Py *self, PyObject *args, PyObject *kwds) {
    printf("Called vec4Py_Init()\n");
    static char *kwlist[] = { "name", "number_of_children", NULL };
    char *name;
    int number_of_children = 0;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|si", kwlist, &name, &number_of_children)) {
        debug_log_INFO();
        return -1;
    }

    return 0;
};
static PyMemberDef vec4Py_members[] = {
    {"x", Py_T_FLOAT, offsetof(vec4Py, v), 0, "Float array which holds the vector coordinates."},
    //{"y", Py_T_FLOAT, offsetof(vec4Py, v), 0, "Float array which holds the vector coordinates."},
    {NULL}  /* Sentinel */
};
static PyGetSetDef vec4Py_getsetters[] = {
    {"v", (getter)vec4Py_get, (setter)vec4Py_set, "Get or Set vector values.", NULL},
    //{"x", (getter)vec4Py_getx, (setter)vec4Py_setx, "Get or Set vector values.", NULL},
    {NULL}  /* Sentinel */
};
static PyMethodDef vec4Py_methods[] = {
    //{ "x", (PyCFunction)vec4Py_getx, METH_NOARGS, "Object.Return the parent of the Object.\n" },
    {NULL}  /* Sentinel */
};
static PyMethodDef vec4Py_module_methods[] = {
    { "get", (PyCFunction)get_vec4Py, METH_VARARGS, "Return the index vec4Py of the index model. get(int model_index, int mesh_index).\n" },
    {NULL}  /* Sentinel */
};
static PyObject *vec4Py_get(vec4Py *self, void *closure) {
    printf("Called vec4Py_get()\n");
    return Py_NewRef(self->v);
};
static int vec4Py_set(vec4Py* self, PyObject *value, void *closure) {
    printf("Called vec4Py_setx()\n");
    PyErr_SetString(PyExc_TypeError, "Can't change the type of vec4Py Object.");
    return -1;
};
static PyObject *vec4Py_getx(vec4Py *self, void *closure) {
    printf("Called vec4Py_get()\n");
    return Py_NewRef(&self->v[0]);
};
static int vec4Py_setx(vec4Py *self, PyObject *value, void *closure) {
    printf("Called vec4Py_setx()\n");
    PyErr_SetString(PyExc_TypeError, "Can't change the type of vec4Py Object.");
    return -1;
};
static PyObject *get_vec4Py(PyTypeObject *type, PyObject *args) {
    printf("Called Custom_get_vec4Py()\n");
    int model_idx, mesh_idx;
    if (!PyArg_ParseTuple(args, "|ii", &model_idx, &mesh_idx)) {
        return NULL;
    }

    vec4Py *self = { 0 };

    return (PyObject*)self;
}
static PyTypeObject vec4PyType = {
    .ob_base = PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "vec4Py",
    .tp_doc = PyDoc_STR("Four dimensions float Vector"),
    .tp_basicsize = sizeof(vec4Py),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new = vec4Py_New,
    .tp_init = (initproc)vec4Py_Init,
    .tp_dealloc = (destructor)vec4Py_Dealloc,
    .tp_members = vec4Py_members,
    .tp_methods = vec4Py_methods,
    .tp_getset = vec4Py_getsetters,
};
static PyModuleDef vec4PyModule = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "vec4Py",
    .m_doc = "Vector module in four dimensions. Vector coordinates are in float values.",
    .m_size = -1,
    .m_methods = vec4Py_module_methods,
};
static vec4Py *vec4Py_FromVec4(vec4 *v) {
    printf("Called vec4Py_FromVec4()\n");
    vec4Py *self = (vec4Py*)vec4PyType.tp_alloc(&vec4PyType, 0);
    if (!self) {
        Py_DECREF(self);
        debug_log_INFO();
        return NULL;
    } else {
        //self->coords = PyList_New(64);
        //Py_INCREF(self->coords);
    }
    return self;
}
PyMODINIT_FUNC PyInit_vec4Py(void) {
    printf("Called PyInit_vec4Py()\n");
    PyObject *m;
    if (PyType_Ready(&vec4PyType) < 0) {
        printf("CRITICAL:%s:%d:%s()\n", __FILE__, __LINE__, __func__);
        return NULL;
    }

    m = PyModule_Create(&vec4PyModule);
    if (m == NULL) {
        printf("CRITICAL:%s:%d:%s()\n", __FILE__, __LINE__, __func__);
        return NULL;
    }

    if (PyModule_AddObjectRef(m, "New", (PyObject*)&vec4PyType) < 0) {
        Py_DECREF(m);
        printf("CRITICAL:%s:%d:%s()\n", __FILE__, __LINE__, __func__);
        return NULL;
    }

    return m;
}