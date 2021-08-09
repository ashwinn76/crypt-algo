if [ "$1" -eq "1" ]; then
  if [ -d "build" ]; then
    rm -r build
  fi
fi

if [ ! -d "build" ]; then
  mkdir build
fi

cd build

CC=gcc CXX=g++ cmake -DCMAKE_CXX_STANDARD=$2 -DCMAKE_BUILD_TYPE=${3-Debug} -DCMAKE_CXX_FLAGS=${4-"-Wall"} ../cpp ../build

make -j12 all
