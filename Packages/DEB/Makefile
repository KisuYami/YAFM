NAME=yafm
VERSION=1.1

${NAME}-${VERSION}.deb:

	wget https://github.com/KisuYami/YAFM/archive/${VERSION}.tar.gz
	tar xf ${VERSION}.tar.gz
	rm ${VERSION}.tar.gz

	./build

	mkdir -p yafm/DEBIAN/
	mkdir -p yafm/usr/bin/

	cp yafm.control yafm/DEBIAN/control
	cp ${NAME}-${VERSION}/build/yafm yafm/usr/bin/

	dpkg-deb --build yafm
	mv yafm.deb ${NAME}-${VERSION}.x86_64.deb

clear:
	rm *.deb
	rm -rd ${NAME}/
	rm -rd ${NAME}-${VERSION}
