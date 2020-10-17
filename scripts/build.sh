if [ "$1" -eq "1" ]; then
  if [ -d "build" ]; then
    rm -r build
    mkdir build
  fi
fi

cd build

CC=gcc-10 CXX=g++-10 cmake -DCMAKE_CXX_STANDARD=$2 -DCMAKE_BUILD_TYPE=${3-Debug} -DCMAKE_CXX_FLAGS=${4-"-Wall"} ..

make -j12 all