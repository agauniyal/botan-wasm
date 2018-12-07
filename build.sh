cd ./botan

CXX=em++ ./configure.py --cc=clang --cpu=llvm --os=emscripten --minimized-build --enable-modules=rsa

Make -j8

cd ..

em++ ./api.cpp -o api.bc

em++ -s ALLOW_MEMORY_GROWTH=1 -s DISABLE_EXCEPTION_CATCHING=0 -s MODULARIZE=1 -s WASM=1 -s EXPORTED_FUNCTIONS="['botan_base64_encode', 'botan_base64_decode']" api.bc botan/libbotan-2.bc

