from distutils.core import setup, Extension
import numpy.distutils.misc_util

dir_include =numpy.distutils.misc_util.get_numpy_include_dirs()

module1 = Extension('classify', sources = ['classify.c'], 
                    include_dirs=dir_include,)
module2 = Extension('classify_bad', sources = ['classify_bad.c'],
                    include_dirs=dir_include,)
module3 = Extension('_classify', sources = ['_classify.c'],
                    include_dirs=dir_include,)
module4 = Extension('f_classify', sources = ['f_classify.c'],
                    include_dirs=dir_include, extra_compile_args=["-O3"])
setup(ext_modules = [module1, module2, module4])
