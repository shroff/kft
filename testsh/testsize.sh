#!/bin/bash

echo "Testing file size" >> ../doc/results
echo "Testing file size"
../bin/kftclient 127.0.0.1 1234 ../test/100kb ../test/recv 1000 5 >> ../doc/results
echo "Done with 100kb"
../bin/kftclient 127.0.0.1 1234 ../test/500kb ../test/recv 1000 5 >> ../doc/results
echo "Done with 500kb"
../bin/kftclient 127.0.0.1 1234 ../test/1mb ../test/recv 1000 5 >> ../doc/results
echo "Done with 1mb"
../bin/kftclient 127.0.0.1 1234 ../test/5mb ../test/recv 1000 5 >> ../doc/results
echo "Done with 5mb"
../bin/kftclient 127.0.0.1 1234 ../test/10mb ../test/recv 1000 5 >> ../doc/results
echo "Done with 10mb"
../bin/kftclient 127.0.0.1 1234 ../test/50mb ../test/recv 1000 5 >> ../doc/results
echo "Done with 50mb"
../bin/kftclient 127.0.0.1 1234 ../test/100mb ../test/recv 1000 5 >> ../doc/results
echo "Done with 100mb"
