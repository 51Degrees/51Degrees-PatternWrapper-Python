/**
 * Copyright (C) 2013 51Degrees.mobi Limited
 *
 * See README.rst and LICENSE.txt for more details.
 */

#include <Python.h>
#include "lib/51Degrees.mobi.h"

#define OUTPUT_BUFFER_LENGTH 50000

struct module_state {
    PyObject *error;
};

#if PY_MAJOR_VERSION >= 3
#define GETSTATE(m) ((struct module_state*)PyModule_GetState(m))
#else
#define GETSTATE(m) (&_state)
static struct module_state _state;
#endif

static PyObject *py_init(PyObject *self, PyObject *args)
{
    // Input property names.
    const char *properties;

    // Parse input arguments: property names (string).
    if (!PyArg_ParseTuple(args, "s", &properties)) {
        return NULL;
    }

    // Init matcher.
    if (init(properties) != 0) {
        PyErr_SetString(PyExc_RuntimeError, "Failed to initialize C wrapper.");
        return NULL;
    }

    // Done!
    Py_RETURN_NONE;
}

static PyObject *py_match(PyObject *self, PyObject *args)
{
    // Input workset.
    Workset *ws = NULL;

    // Input user agent.
    const char *userAgent;

    // Detected device.
    const Device *device;

    // Output CSV buffer.
    char output[OUTPUT_BUFFER_LENGTH];

    // Parse input arguments: user agent (string).
    if (!PyArg_ParseTuple(args, "s", &userAgent)) {
        return NULL;
    }

    // Check user agent string length.
    if (strlen(userAgent) < MAXBUFFER) {
        // Match user agent.
        ws = createWorkset();
        strcpy(ws->input, userAgent);
        device = getDevice(ws);
        freeWorkset(ws);

        // Fetch properties.
        if (device != NULL) {
            if (processDeviceCSV(device, output, OUTPUT_BUFFER_LENGTH) < 0) {
                PyErr_SetString(PyExc_RuntimeError, "Failed to process device CSV.");
                return NULL;
            } else {
                return  Py_BuildValue("s", output);
            }
        }
        else {
            Py_RETURN_NONE;
        }
    } else {
        PyErr_SetString(PyExc_RuntimeError, "User agent string too long.");
        return NULL;
    }
}

static PyMethodDef wrapperMethods[] =
{
     {"init", (PyCFunction)py_init, METH_VARARGS, "Initializes wrapper."},
     {"match", (PyCFunction)py_match, METH_VARARGS, "Matches user agent string."},
     {NULL, NULL, 0, NULL}
};

#if PY_MAJOR_VERSION >= 3
static int _fiftyone_degrees_mobile_detector_lite_pattern_wrapper_traverse(PyObject *m, visitproc visit, void *arg)
{
    Py_VISIT(GETSTATE(m)->error);
    return 0;
}

static int _fiftyone_degrees_mobile_detector_lite_pattern_wrapper_clear(PyObject *m)
{
    Py_CLEAR(GETSTATE(m)->error);
    return 0;
}

static struct PyModuleDef wrapperDefinition =
{
        PyModuleDef_HEAD_INIT,
        "_fiftyone_degrees_mobile_detector_lite_pattern_wrapper",
        NULL,
        sizeof(struct module_state),
        wrapperMethods,
        NULL,
        _fiftyone_degrees_mobile_detector_lite_pattern_wrapper_traverse,
        _fiftyone_degrees_mobile_detector_lite_pattern_wrapper_clear,
        NULL
};

#define INITERROR return NULL
#else
#define INITERROR return
#endif

#if PY_MAJOR_VERSION >= 3
PyObject *PyInit__fiftyone_degrees_mobile_detector_lite_pattern_wrapper(void)
#else
void init_fiftyone_degrees_mobile_detector_lite_pattern_wrapper(void)
#endif
{
#if PY_MAJOR_VERSION >= 3
    PyObject *module = PyModule_Create(&wrapperDefinition);
#else
    PyObject *module = Py_InitModule("_fiftyone_degrees_mobile_detector_lite_pattern_wrapper", wrapperMethods);
#endif

    if (module == NULL) {
        INITERROR;
    }
    struct module_state *st = GETSTATE(module);

    st->error = PyErr_NewException("_fiftyone_degrees_mobile_detector_lite_pattern_wrapper.Error", NULL, NULL);
    if (st->error == NULL) {
        Py_DECREF(module);
        INITERROR;
    }

#if PY_MAJOR_VERSION >= 3
    return module;
#endif
}
