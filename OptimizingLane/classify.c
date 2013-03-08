#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <numpy/arrayobject.h>

static char module_docstring[] = 
  "This module provides a C-implementation of the modified Hui-Walter \
classification function.";

static char classify_docstring[] = 
  "This function implements the fuzzy classification function.";

double classify(double x, double c, double b);

static PyObject* classify_classify(PyObject* self, PyObject* args);

static PyObject* classify_classify(PyObject* self, PyObject* args) {
  double c, b;
  int iN;
  PyObject *x_obj;

  if (!PyArg_ParseTuple(args, "Odd", &x_obj, &c, &b))
    return NULL;

  PyArrayObject *x_array = 
    (PyArrayObject *) PyArray_FROM_OTF(x_obj, NPY_DOUBLE, NPY_IN_ARRAY);
  if (x_array == NULL) {
    Py_XDECREF(x_array);
    return NULL;
  }

  int nd = PyArray_NDIM(x_array);
  if (nd != 1) {
    Py_XDECREF(x_array);
    PyErr_SetString(PyExc_TypeError,"Array is wrong shape!\n");
    return NULL;
  }
  npy_intp N = PyArray_DIM(x_array, 0);

  npy_intp shape[] = {N};
  PyArrayObject *vecout = (PyArrayObject *) PyArray_SimpleNew(1, shape,  
							      PyArray_DOUBLE);
  /*if (PyArray_AsCArray(&vecout, (void *) &OUT, shape, nd, 
		       PyArray_DescrFromType(PyArray_DOUBLE)) < 0) {
    PyErr_SetString(PyExc_TypeError, "Error obtaining C array.\n");
    return NULL;
    }
  if (PyArray_AsCArray(&x_array, (void *) &x, shape, nd, 
		       PyArray_DescrFromType(PyArray_DOUBLE)) < 0) {
    PyErr_SetString(PyExc_TypeError, "Error obtaining C array.\n");
    return NULL;
    }*/

  double *OUT = (double *) PyArray_DATA(vecout);
  double *x = (double *) PyArray_DATA(x_array);

  for (iN=0; iN<N; iN++) {
      OUT[iN] = classify(x[iN], c, b);
  }

  Py_DECREF(x_array);

  return Py_BuildValue("N",vecout);
}

static PyMethodDef classify_methods[] =
  {
    {"classify", classify_classify, METH_VARARGS, classify_docstring},
    {NULL, NULL, 0, NULL}
  };

PyMODINIT_FUNC initclassify(void)
{
  PyObject *m = Py_InitModule3("classify", classify_methods, 
			       module_docstring);
  if (m==NULL) 
    return;

  import_array();
}

double classify(double x, double c, double b) {
  if (x<(c-b))
    return 0;
  else if (x>(c+b))
    return 1;
  else {
    if (b>1E-7)
      return (x-c+b)/2/b;
    else
      return 0.5;
  } 
}
