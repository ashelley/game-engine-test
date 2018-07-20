pushd ..\build

del test.ppm

raytracer.exe > test.ppm

popd