if [ -d "build" ]
then
    rm -r build
fi

mkdir build
cd build

CC=gcc-10 CXX=g++-10 cmake -DCMAKE_CXX_STANDARD=$1 -DCMAKE_BUILD_TYPE=${2-Debug} -DCMAKE_CXX_FLAGS=${3-"-Wall"} -DCOVERAGE=${4-0} ..

make -j12 all