# First, prepare the botan lib
cd ./botan

# This prepares the botan lib for use. It is minified as much as possible, therefore every module 
# that we want to use has to be included in the --enable-modules option
CXX=em++ ./configure.py --cc=clang --cpu=llvm --os=emscripten --minimized-build --enable-modules=rsa,auto_rng,system_rng

# Make the botan lib. -j8 allows 8 threads to be spawned, making it faster 
Make -j8

# Up again
cd ..

# Link our API file with botan
em++ \
  --bind `# This tells the compiler that we use Embind functions, see https://kripken.github.io/emscripten-site/docs/porting/connecting_cpp_and_javascript/embind.html `\
  -std=c++11 `# Use c++ version 11, as it is needed for building botan functions` \
  -Ibotan/build/include  `# Include the botan header files, so the compiler can properly link our API with the botan functions`\
  -c `# Tells emcc to generate LLVM bitcode (which can then be linked with other bitcode files), instead of compiling all the way to JavaScript.`\
  api.cpp botan/libbotan-2.a `# Use our api file and the previously built botan lib as input`

# This will have created two files: api.o and libbotan-2.o

# Build the .js and .wasm files
em++ \
  --bind `# Again, tell the compiler that Embind functions are somewhere inside the code`\
  -s ALLOW_MEMORY_GROWTH=1 `# this allows for dynamic memory allocation, which is okay with wasm since it produces little to no overhead. see https://kripken.github.io/emscripten-site/docs/optimizing/Optimizing-Code.html`\
  -s DISABLE_EXCEPTION_CATCHING=0 `# emscripten disables exception catching per default to make wasm faster, since wasm has no support for exceptions yet. This reenables exceptions for us, so a thrown exception does not terminate the application. See https://kripken.github.io/emscripten-site/docs/optimizing/Optimizing-Code.html#optimizing-code-exception-catching`\
  -s MODULARIZE=1 `# By default, emscripten operates on a global Module object. That clutters global namespace. This option allows modularized usage of the emscripten module through a function call.`\
  -s WASM=1 `# output the code as a .wasm file.`\
  api.o libbotan-2.o `# Use the two previously created files as input`

# you will find two files, a.out.js and a.out.wasm