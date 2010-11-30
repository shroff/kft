#!/bin/bash

echo "Testing packet size:" >> ../doc/results
echo "Testing packet size:"
../bin/kftclient 127.0.0.1 1234 ../test/10mb ../test/recv 100 5 >> ../doc/results
echo "Done with 100"
../bin/kftclient 127.0.0.1 1234 ../test/10mb ../test/recv 200 5 >> ../doc/results
echo "Done with 200"
../bin/kftclient 127.0.0.1 1234 ../test/10mb ../test/recv 400 5 >> ../doc/results
echo "Done with 400"
../bin/kftclient 127.0.0.1 1234 ../test/10mb ../test/recv 800 5 >> ../doc/results
echo "Done with 800"
../bin/kftclient 127.0.0.1 1234 ../test/10mb ../test/recv 1600 5 >> ../doc/results
echo "Done with 1600"
../bin/kftclient 127.0.0.1 1234 ../test/10mb ../test/recv 3200 5 >> ../doc/results
echo "Done with 3200"
../bin/kftclient 127.0.0.1 1234 ../test/10mb ../test/recv 6400 5 >> ../doc/results
echo "Done with 6400"
../bin/kftclient 127.0.0.1 1234 ../test/10mb ../test/recv 12800 5 >> ../doc/results
echo "Done with 12800"
../bin/kftclient 127.0.0.1 1234 ../test/10mb ../test/recv 25600 5 >> ../doc/results
echo "Done with 25600"
../bin/kftclient 127.0.0.1 1234 ../test/10mb ../test/recv 51200 5 >> ../doc/results
echo "Done with 51200"
