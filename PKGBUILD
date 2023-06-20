# Maintainer: Skidout <Skidout@proton.me>

pkgname=jiffyedit-extra
pkgver=1.4.7.21
pkgrel=1
pkgdesc="Extra plugins for Jiffyedit"
arch=(x86_64)
url="https://github.com/Skidout/jiffyedit-extra"
license=('custom')
depends=(
	ffmpeg
	whisper.cpp
	whisper.cpp-model-base
	)
makedepends=(
	gcc
	)
provides=(jiffyedit)
source=("$pkgname::git+$url")
md5sums=('SKIP')

build() {
	cd "$pkgname"
	g++ -Ofast jiffyedit-vd.cpp -o jiffyedit-vd
	g++ -Ofast jiffyedit-nz.cpp -o jiffyedit-nz
}

package() {
	cd "$pkgname"
	install -D jiffyedit-vd -t ${pkgdir}/usr/lib/jiffyedit
	install -D jiffyedit-vd.dat -t ${pkgdir}/usr/lib/jiffyedit
	install -D jiffyedit-nz -t ${pkgdir}/usr/lib/jiffyedit
	install -D jiffyedit-nz.dat -t ${pkgdir}/usr/lib/jiffyedit
}
