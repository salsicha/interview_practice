
## Protobuf example

This repo contains a gRPC server and client for rotating and filtering images.

Specification:  
This was tested on MacOS, but it should work on both Mac and Ubuntu, in either case pip needs to be installed.
Also, this was only tested with python 3 (/usr/bin/python3).  

Discussion:  
This is a proof of concept.
Basic functionality is here, but three is no proof that it can scale, be secure, or safely maintained.
This runs natively, but this should be isolated/containerized for deployment.
There is no authentication and the messages are not validated/sanitized.
Testing should be added so that the code can be safely maintained.
Finally, the normalized convolution solution in the mean filter method is trivial,
was designed to conform to the stated requirements,
and may be improved with a better algorithm.

#### Setup

To install run setup and build:  
./setup  
./build

#### Server

Start server first:  
./server --host 127.0.0.1 --port 5005

#### Client

To rotate image 90 degrees:  
./client --port 5005 --input ./test/color.png --output ./test/output.png --rotate NINETY_DEG

To rotate and mean filter image:  
./client --port 5005 --input ./test/color.png --output ./test/output.png --rotate NINETY_DEG --mean

Valid rotation values are:  
"NINETY_DEG", "ONE_EIGHTY_DEG", or "TWO_SEVENTY_DEG
