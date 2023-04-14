#LogViewer

This is a project I am currently working on, to make it possible to have a stack logging of the functions that are called inside your embedded software.
The following files can be included inside your embedded project to make it possible to generate the logs.
CFuncTracer.cpp & CFuncTracer.h : those files contains the function tracers that you can call at the beginning of each function.  Those functions will automatically add timestamp, dbg level, procid and threadid + description. you can get then a output like this:

09:11:48.589 TRACE     [4174:4233]  - HomeSensor::IsDeviceConnected() : CONNECTED
09:11:48.600 TRACE     [4174:4223]  - TcpIpPortMapper::Add() : Entr
09:11:48.601 TRACE     [4174:4223]  - TcpIpPortMapperMapper::Add() : Videodevice HomeSensor
09:11:48.602 TRACE     [4174:4223]  - TcpIpPortMapperMapper::Add() : ProtocolInfo (7)
09:11:48.604 TRACE     [4174:4223]  - TcpIpPortMapperMapper::Add() :     - TraficonSdk2/Xml2
09:11:48.605 TRACE     [4174:4223]  - TcpIpPortMapperMapper::Add() :     - RTSP
09:11:48.605 TRACE     [4174:4223]  - TcpIpPortMapperMapper::Add() :     - HTTP

Ctracer.cpp & CTracer.h : is the different tracers you can implement like filetracer, dayfiletracer, circularbuffertracer, tcpiptracer etc.
This object will generate the output and send to the required destination (file, tcp/ip connection, etc).

The logviewer is an application that makes it possible to do filtering on the trace files to make it easy to do your analysis and find bugs (like timing related issues etc.)
The purpose of this project is to let the possible customers to see the style of my software writing.
This project is not fully debugged and released -> it is still under development.

Goal : this project will be extended with a tcpip receiver that gets the logs from an embedded system under test so that the loggings are not need to be stored on flash but on an external pc.  But like I said not completely finished.

