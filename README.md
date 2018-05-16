# simpleudpclientserver
Simple UDP server and client applications

How to build and run?
- Prequistes:
+ Ubuntu or Mac OSX or Windows
+ GCC/Clang support C++11. In my case, I used g++4.9 or clang-900.0.39.2
+ Boost version 1.59.0 or latest 
+ C++ Thread Pool Library: https://github.com/vit-vit/ctpl
+ msvc-14.0
+ Visual Studio 2015

- Build:
+ Ubuntu or Mac OSX:
cd /working/dir
git clone https://github.com/ducvo1983/simpleudpclientserver.git
cd simpleudpclientserver
cd udpserver
git clone https://github.com/vit-vit/CTPL.git
cd ..

+ In the case if boost header files and libraries locate at /usr/include and /usr/lib 
cd udpserver
make clean
make

cd ../udpclient
make clean
make

+ In the case if boost header files and libraries locate at other location
cd udpserver
make clean
make BOOST_INCLUDEDIR=/path/to/boost/header/file BOOST_LIBRARYDIR=/path/to/boost/library

cd ../udpclient
make clean
make

+ Windows:
  Make sure the Visual Studio 2015 and msvc-14.0 are installed
  Boost version 1.62.0 is installed
  
  Steps:
	Open coding_exercise.sln using Visual Studio 2015
	Right click on the udpserver project and select Properties
	In the left panel, select C/C++ -> General -> Additional Include Directory -> Change boost include dir to your include dir on your PC -> Apply
        Build the coding_exercise solution
        Check the output (udpserver.exe, udpclient.exe) at: coding_exercise\[Debug | Release]

- Run:
* Ubuntu or Mac OSX:
+ Open a terminal and run server:
$ ./udpserver -s
Listening port 49632

+ Open another terminal to run client:
$ ./udpclient -s 127.0.0.1:49632 -n 1500

Output:
$ ./udpserver -s
Listening port 49632
Listening port 49632
Received request from 127.0.0.1:59672with value"1500"
Sent response "DONE" to 127.0.0.1:59672.

$ ./udpclient -s 127.0.0.1:49632 -n 1500
Sent datagram with value "1500" to 127.0.0.1:49632. Waiting for response...
Received response  "DONE" from 127.0.0.1:49632.
Exiting...$

+ To test multiple requests:
Open different terminals and run the following command:
for i in {1..100}; do ./udpclient -s 127.0.0.1:49632 -n 1500 ; done

* Windows:
Open two Command Prompt 
	Server:
		>udpserver.exe -s
		Listening port 64394
	Client:
		>udpclient.exe -s 127.0.0.1:64394 -n 1500

Happy testing!
