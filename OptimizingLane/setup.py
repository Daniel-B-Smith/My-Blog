from distutils.core import setup, Extension
import numpy.distutils.misc_util

module1 = Extension('classify', sources = ['classify.c'],
                    include_dirs=numpy.distutils.misc_util.get_numpy_include_dirs(),)

module2 = Extension('classify_bad', sources = ['classify_bad.c'],
                    include_dirs=numpy.distutils.misc_util.get_numpy_include_dirs(),)

module3 = Extension('_classify', sources = ['_classify.c'],
                    include_dirs=numpy.distutils.misc_util.get_numpy_include_dirs(),)
 
setup(ext_modules = [module1, module2, module3])
