pkgname=yafm
pkgver=0.0.1
pkgrel=1
pkgdesc="Suckless File Manager"
arch=('x86_64')
url="https://github.com/KisuYami/yafm"
license=('GPL2')
makedepends=('make' 'gcc')
source=("YAFM.tar.gz::https://github.com/KisuYami/yafm/blob/master/YAFM.tar.gz")
md5sums=('e74159ead512ac5982f577e76e72a43c')
build() {

  cd "$srcdir"
  tar -xf YAFM.tar.gz
  cd YAFM
  make
}

package() {

  cd "$srcdir/$pkgname/YAFM"
  make DESTDIR="$pkgdir" install
}
