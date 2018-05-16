# simpleudpclientserver
Simple UDP server and client applications
  
Prequistes:  
 Ubuntu or Mac OSX or Windows <br />
 GCC/Clang support C++11. In my case, I used g++4.9 or clang-900.0.39.2 <br />
 Boost version 1.59.0 or latest <br />
 C++ Thread Pool Library: https://github.com/vit-vit/ctpl <br />
 msvc-14.0 <br />
 Visual Studio 2015 <br /> 
  
Build:  
 Ubuntu or Mac OSX:  
  cd /working/dir <br /> 
  git clone https://github.com/ducvo1983/simpleudpclientserver.git <br />
  cd simpleudpclientserver <br />
  cd udpserver <br />
  git clone https://github.com/vit-vit/CTPL.git <br />
  cd .. <br />
  <br />
 In the case if boost header files and libraries locate at /usr/include and /usr/lib <br />
  cd udpserver <br />
  make clean <br />
  make <br />
 <br />
  cd ../udpclient <br />
  make clean <br />
  make <br />
  
 In the case if boost header files and libraries locate at other location <br />
  cd udpserver <br />
  make clean <br />
  make BOOST_INCLUDEDIR=/path/to/boost/header/file BOOST_LIBRARYDIR=/path/to/boost/library <br />
 <br />
  cd ../udpclient <br />
  make clean <br />
  make <br />
 <br />
 Windows: <br />
  Make sure the Visual Studio 2015 and msvc-14.0 are installed <br />
  Boost version 1.62.0 is installed <br />
 <br />
  Steps: <br />
	Open coding_exercise.sln using Visual Studio 2015 <br />
	Right click on the udpserver project and select Properties <br />
	In the left panel, select C/C++ -> General -> Additional Include Directory -> Change boost include dir to your include dir on your PC -> Apply <br />
        Build the coding_exercise solution <br />
        Check the output (udpserver.exe, udpclient.exe) at: coding_exercise\[Debug | Release] <br />
 <br />
 Run:   <br />
  Ubuntu or Mac OSX:   <br />
   Open a terminal and run server:   <br />
    $ ./udpserver -s <br />
    Listening port 49632 <br />
 <br />
   Open another terminal to run client:   <br />
    $ ./udpclient -s 127.0.0.1:49632 -n 1500 <br />
   <br />
   Output:   <br />
    $ ./udpserver -s <br />
    Listening port 49632 <br />
    Listening port 49632 <br />
    Received request from 127.0.0.1:59672with value"1500" <br />
    Sent response "DONE" to 127.0.0.1:59672 <br />

    $ ./udpclient -s 127.0.0.1:49632 -n 1500
    Sent datagram with value "1500" to 127.0.0.1:49632. Waiting for response...
    Received response  "DONE" from 127.0.0.1:49632.
    Exiting...$
  
   To test multiple requests:   <br />
    Open different terminals and run the following command:   <br />
    for i in {1..100}; do ./udpclient -s 127.0.0.1:49632 -n 1500 ; done  <br />
   <br />
 Windows:   <br />
  Open two Command Prompt    <br />
   Server:   <br />
    >udpserver.exe -s <br />
     Listening port 64394 <br />
   Client:   <br />
    >udpclient.exe -s 127.0.0.1:64394 -n 1500 <br />
   <br />
Happy testing!  
