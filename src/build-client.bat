set CommonCompilerFlags=-Od -Zi 
set CommonLinkerFlags= -incremental:no -opt:ref user32.lib Ole32.lib d2d1.lib Gdi32.lib

pushd ..\build

del *.pdb > NUL 2> NUL

cl %CommonCompilerFlags% ..\src\win32_client.cpp /link %CommonLinkerFlags%

popd