#!/bin/bash

echo "Testing client errors:" >> ../doc/results
echo "Testing client errors..."
../bin/kftclient 127.0.0.1 1234 ../test/10mb ../test/recv 1000 1 >> ../doc/results
echo "Done with 1%"
../bin/kftclient 127.0.0.1 1234 ../test/10mb ../test/recv 1000 2 >> ../doc/results
echo "Done with 2%"
../bin/kftclient 127.0.0.1 1234 ../test/10mb ../test/recv 1000 5 >> ../doc/results
echo "Done with 5%"
../bin/kftclient 127.0.0.1 1234 ../test/10mb ../test/recv 1000 10 >> ../doc/results
echo "Done with 10%"
../bin/kftclient 127.0.0.1 1234 ../test/10mb ../test/recv 1000 15 >> ../doc/results
echo "Done with 15%"
../bin/kftclient 127.0.0.1 1234 ../test/10mb ../test/recv 1000 20 >> ../doc/results
echo "Done with 20%"
../bin/kftclient 127.0.0.1 1234 ../test/10mb ../test/recv 1000 30 >> ../doc/results
echo "Done with 30%"
../bin/kftclient 127.0.0.1 1234 ../test/10mb ../test/recv 1000 40 >> ../doc/results
echo "Done with 40%"
../bin/kftclient 127.0.0.1 1234 ../test/10mb ../test/recv 1000 50 >> ../doc/results
echo "Done with 50%"
../bin/kftclient 127.0.0.1 1234 ../test/10mb ../test/recv 1000 60 >> ../doc/results
echo "Done with 60%"
../bin/kftclient 127.0.0.1 1234 ../test/10mb ../test/recv 1000 70 >> ../doc/results
echo "Done with 70%"
../bin/kftclient 127.0.0.1 1234 ../test/10mb ../test/recv 1000 80 >> ../doc/results
echo "Done with 80%"
