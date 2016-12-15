
.PHONY: nacl all android android-arm android-x86

all: nacl nacl.demo android

nacl:
	make -f nacl.make

nacl.demo: nacl
	cp -rf ./demo/nacl/* out
	cp -rf ./nacl-module.js ./out

android: android-arm android-x86

android-arm:
	make -f android.make ARCH=arm

android-x86:
	make -f android.make ARCH=x86

clean:
	rm -rfv out/*