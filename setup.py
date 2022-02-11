from distutils.core import setup, Extension
exta=["-Wall","-Wextra","-Wconversion","-Wpedantic", "-std=gnu11"]
plecak = Extension('paker', sources = ['src/code.C'],extra_compile_args=exta)
setup (name = 'Paker',
       version = '1.0',
       description = 'projekt na zaliczenie',
       ext_modules = [plecak,],
       platforms=['POSIX',])