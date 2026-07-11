if [ -z "${LD_LIBRARY_PATH}" ]; then
   LD_LIBRARY_PATH=../lib; export LD_LIBRARY_PATH
else
   LD_LIBRARY_PATH=../lib:$LD_LIBRARY_PATH; export LD_LIBRARY_PATH
fi

