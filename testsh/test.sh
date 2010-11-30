#!/bin/bash

../bin/kftclient 127.0.0.1 1234 ../test/10mb ../test/recv 1000 5 >> ../doc/results
echo "Done with 1"
../bin/kftclient 127.0.0.1 1234 ../test/10mb ../test/recv 1000 5 >> ../doc/results
echo "Done with 2"
../bin/kftclient 127.0.0.1 1234 ../test/10mb ../test/recv 1000 5 >> ../doc/results
echo "Done with 3"
../bin/kftclient 127.0.0.1 1234 ../test/10mb ../test/recv 1000 5 >> ../doc/results
