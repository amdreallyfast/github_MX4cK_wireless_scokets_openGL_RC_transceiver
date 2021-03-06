The folder "accel_and_wireless_TCPIP.X" contains the microcontroller source 
code.  It does not, however, contain any of the framework code that I developed
to make my life easier when using TCPIP and I2C capable pmods.  That code is 
contained in my "github_my_MX4cK_framework" repo.

The microcontroller code was built in Microchip's MPLAB X IDE using the 
Microchip XC32 compiler with C++ enabled.  Using class enlarges my project size
a bit, but not by much because the vast majority of the project space is 
consumed by the TCPIP and wireless stacks, and I am more concerned with 
readability and maintainability for my projects than maximizing performance or 
flash usage.  

I used the legacy Microchip TCPIP stack for wireless socket communication.  A 
former student friend of mine from a microcontroller class had managed to 
figure out how to utilize the stack for simple wireless socket communication, 
but his code wasn't portable or easily understandable.  It was a bit of a hack 
job that was made to simply work for a class, and was not made to be easily 
maintained or ported.

I took that code and build my own TCPIP wrapper framework.  I designed the 
framework and built it only with my particular application in mind.  While it
is still not portable with different hardware, it is much more portable and 
more easily learned and maintainable for other students in the same class.  The 
TCPIP stack only supports up to WPA2 Personal encryption, not enterprise, so it 
cannot connect to my school's wireless network.  That makes me sad, but so be 
it.



The folder "accel_to_scokets_to_openGL" contains the code that I used in Visual
Studio 2013 express.  It does not contain any of the openGL libraries and does
not contain any project files.

The openGL material and PC-side socket communication was created in Visual 
Studio 2013 Express using the open source Qt framework.  I was unable to use 
the Qt pluggin for Visual Studio because the Qt website says that the pluggin 
does not work with Visual Studio Express editions, so I had to manually change 
specify Qt libraries to link in, and change project settings for additional 
include directories and the debug environment PATH macro to hook up the correct
.lib and .dll files, respectively, that stored the Qt code.  Lastly, and
most problematically, I had to manually execute the Qt Meta-Object Compiler in
order to utilize Qt's cross-threading communications and asynchronous execution 
of the socket and Qt application.  It was a pain, but it worked.



Now, about the data:
(1) The accelerometer provides a 16bit float for each axis.
(2) The data, if the accelerometer were calibrated correctly, which it isn't,
would be between -1.0f and +1.0f.  It's close enough for my purposes though.
(3) I decided to utilize this data as a three-dimensional directional vector
that I would rotate my shapes to "point at".  I put "point at" in quotes 
because openGL has no concept of direction aside from the one and only "true"
coordinate space, by which I mean the reference space by which all other 
spaces, such as object and model and view spaces, are calculated.  A "shape" 
is, to openGL, a collection of points in space.  There is no "front", "back",
"top", "bottom", "left", or "right", but as long as my rotation matrix is 
consistent enough to track, then it doesn't matter and the term "point at" will
work.

Now, here's how the data gets turned into a shape rotation:
(1) The three floats, one each for the X, Y, and Z axes, are sent over the 
network 10 times per second.
(2) Socket communication is blocking.  There is no way around this.  The only
way to perform socket communication asynchronously with the rest of my program
is to take advantage of Qt's automatic multi-threading capabilities with it's 
own TCP socket.
(3) I made a custom socket class that utilized the Qt library's QTcpSocket 
class.  Once created, the QTcpSocket is automatically spun off into its own 
thread.  Therefore, I do not have to manually handle the threading.
(4) I used some Qt functions and meta-data tags to hook up a method in my
custom socket class, a method that was responsible for checking the number of 
bytes available and reading the data if any was available, to a method in 
another class that would then create a new rotation matrix for whatever shape I
was currently rendering.  In this case, it is a pyramid.

