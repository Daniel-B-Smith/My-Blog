#include <Python.h>
 
/* Docstrings */
static char module_docstring[] =
  "This module provides an interface for calculating Fibonacci numbers in C.";
static char fib_docstring[] =
  "Calculate the largest Fibonacci number less than N.";
static char dumb_docstring[] =
  "A dumb way of calculating the N'th Fibonacci number.";
 
/* Available functions */
static PyObject *fib_fib(PyObject *self, PyObject *args);
static PyObject *fib_dumb(PyObject *self, PyObject *args);
 
static int dumb_fib(int n);

/* Module specification */
static PyMethodDef module_methods[] = {
    {"fib", fib_fib, METH_VARARGS, fib_docstring},
    {"dumb", fib_dumb, METH_VARARGS, dumb_docstring},
    {NULL, NULL, 0, NULL}
};
 
/* Initialize the module */
PyMODINIT_FUNC init_fib(void)
{
    PyObject *m = Py_InitModule3("_fib", module_methods, module_docstring);
    if (m == NULL)
        return;
}
 
static PyObject *fib_fib(PyObject *self, PyObject *args)
{
  int n, a, b, c;
 
  /* Parse the input tuple */
  if (!PyArg_ParseTuple(args, "i", &n))
    return NULL;
  
  a = 0;
  b = 1;
  while (b < n) {
    c = a + b;
    a = b;
    b = c;
  }
  
  /* Build the output tuple */
  PyObject *ret = Py_BuildValue("i", b);
  return ret;
}

static PyObject *fib_dumb(PyObject *self, PyObject *args)
{
  int n, b;
 
  /* Parse the input tuple */
  if (!PyArg_ParseTuple(args, "i", &n))
    return NULL;
  
  b = dumb_fib(n);
  
  /* Build the output tuple */
  PyObject *ret = Py_BuildValue("i", b);
  return ret;
}

static int dumb_fib(int n)
{
  if (n==1) 
    return 1;
  else if (n==2)
    return 1;
  else
    return dumb_fib(n-1) + dumb_fib(n-2);
}
