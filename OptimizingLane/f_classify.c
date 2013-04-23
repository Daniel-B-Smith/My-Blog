#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <numpy/arrayobject.h>

#define ROUND_DOWN(x, s) ((x) & ~((s)-1))

static char module_docstring[] = 
  "This module provides a C-implementation of the modified Hui-Walter \
classification function.";

static char f_classify_docstring[] = 
  "This function implements the fuzzy classification function.";

double classify(double x, double c, double b);
inline
double o_classify(double x, double m, double w);

static PyObject* f_classify_classify(PyObject* self, PyObject* args);

static PyObject* f_classify_classify(PyObject* self, PyObject* args) {
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
  double *out = (double *) PyArray_DATA(vecout);
  double *x = (double *) PyArray_DATA(x_array);

  if (b < 1.2E-7) {
    for (iN=0; iN<N; iN++) 
      out[iN] = x[iN] < c ? 0 : 1;
  } else {
    const int stepsize = 8;
    double m = c - b, w = 1/2.0/b;
    for (iN=0; iN<ROUND_DOWN(N, stepsize); iN+=stepsize) {
      out[iN] = o_classify(x[iN], m, w);
      out[iN+1] = o_classify(x[iN+1], m, w);
      out[iN+2] = o_classify(x[iN+2], m, w);
      out[iN+3] = o_classify(x[iN+3], m, w);
      out[iN+4] = o_classify(x[iN+4], m, w);
      out[iN+5] = o_classify(x[iN+5], m, w);
      out[iN+6] = o_classify(x[iN+6], m, w);
      out[iN+7] = o_classify(x[iN+7], m, w);
    }
    for (; iN<N; iN++)
      out[iN] = o_classify(x[iN], m, w);
  }

  Py_DECREF(x_array);

  return Py_BuildValue("N",vecout);
}

static PyMethodDef f_classify_methods[] =
  {
    {"classify", f_classify_classify, METH_VARARGS, f_classify_docstring},
    {NULL, NULL, 0, NULL}
  };

PyMODINIT_FUNC initf_classify(void)
{
  PyObject *m = Py_InitModule3("f_classify", f_classify_methods, 
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

inline
double o_classify(double x, double m, double w) {
  double out = (x-m)*w;
  out = (out < 0) ? 0 : out;
  out = (out > 1) ? 1 : out;
  return out;
}
