#pragma once
// adb killserver = 30303039686f73743a6b696c6c
//adb Tap import 
#include <format>

#define adbHexHostHeaderOneStr "000chost:version"

#define adbHexHostHeaderTwoAnyPortStr "000dhost:features"
#define adbHexHostHeaderTwoSelectPorttPort(X)  std::format("0024host-serial:{}:features",X) //xxx.xxx.xxx.xxx:xxx   

#define adbHexAnyPortStr "000ehost:tport:any"
#define adbHexSelectPort(X)  std::format("0021host:tport:serial:{}",X) //xxx.xxx.xxx.xxx:xxx   


#define adbHexEndStr "\x05\x0b\x00\x00\x0030x120,0x0\x00"

#define adbHexTap(X,Y)  std::format("0032shell,v2,TERM=xterm-256color,raw:input tap {} {}",X,Y)  

#define adbHexDevices "000chost:devices" // adb devices

