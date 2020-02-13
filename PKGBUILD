# Maintainer: Reberti Carvalho Soares <6reberti6@gmail.com>
pkgname=kyfm
pkgver=1.0
pkgrel=1
epoch=1
pkgdesc="The fastest file manager."
arch=('i686' 'x86_64')
url='www.github.com/KisuYami/kyfm'
license=('GPLv2')
depends=('ncurses' 'readline')
makedepends=('ncurses' 'readline')
options=()
source=('git://github.com/KisuYami/kyfm')
md5sums=('SKIP')

build() {
  cd "$pkgname/"
  make
}

package() {
  cd "$pkgname/"

  mkdir -p "$pkgdir"/usr/bin/
  make DESTDIR="$pkgdir" install
}

# vim:set ts=2 sw=2 et:
