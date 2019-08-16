sed -i -e 's/libsystemc\.a/libsystemc\.so/g' ./configure.in
sed -i -e 's/g++-5\.2\.0/g++-5\.2\.0 \| g++-8/g' ./configure.in
sed -i -e 's/c++11/c++14/g' ./configure.in
sed -i -e '25s/^/#include \<sstream\>/' src/scml2_base/callback_observers_owner.h
autoconf
