Kernel module development and kernel usage exercises.

# kernel building
## Rocky Linux 9
Reference: https://docs.rockylinux.org/9/guides/custom-linux-kernel/
1) Installing prerequisite tools and libraries
1.1) Update repo meta
dnf makecache  ## /etc/yum.repos.d
1.2) Install developing tools
dnf -y groupinstall 'Development Tools'
1.3) Install other header files
dnf -y install ncurses-devel openssl-devel elfutils-libelf-devel python3
1.4) Enable crb(aka Powertools) repo
dnf config-manager --set-enabled crb
dnf makecache
1.5) Install dwarves package
dnf -y install dwarves
1.6) Install a special dwarves(pahole) version for Rocky[pahole 1.21~1.23 is good for Rocky 9.4; pahole 1.27 is too high)
1.6.1) remove installed dwarves
dnf remove dwarves
1.6.2) Enable CRB and install build dependencies [Rocky 9 often needs CRB for -devel packages]
dnf config-manager --set-enabled crb
1.6.3) Fetch dwarves 1.21 source and build it
git clone https://git.kernel.org/pub/scm/devel/pahole/pahole.git
cd pahole
git checkout tags/v1.21 -b v1.21
make
1.6.4)  Install to /usr/local so it doesn’t clash with RPM-managed files
make install //same as: make install PREFIX=/usr/local
1.6.5) Verify and use
// should show v1.21
/usr/local/bin/pahole --version
// export PATH=/usr/local/bin:$PATH
export PATH=/usr/local/bin:$PATH
// For kernel builds, you can explicitly point to it:
make PAHOLE=/usr/local/bin/pahole
// or  export PAHOLE=/usr/local/bin/pahole
1.6.6) solve lib dependency issue
// check current dependencies
ldd /usr/local/bin/pahole
// Add /usr/local library dirs to the dynamic linker config
echo -e "/usr/local/lib64\n/usr/local/lib" | tee /etc/ld.so.conf.d/usr-local.conf
// Update cache
ldconfig
// Verify
ldconfig -p | grep dwarves_reorganize
/usr/local/bin/pahole --version
2) Download or install linux kernel source files
2.1) Download from URLs
URL Roots:
2.1.1) general versions
https://www.kernel.org/pub/linux/kernel/v6.x
https://www.kernel.org/pub/linux/kernel/v5.x
wget -c https://www.kernel.org/pub/linux/kernel/v5.x/linux-5.14.21.tar.xz
2.1.2) Rocky
https://download.rockylinux.org/pub/rocky/9/BaseOS/source/tree/Packages/k/
wget -c https://download.rockylinux.org/pub/rocky/9/BaseOS/source/tree/Packages/k/kernel-5.14.0-570.52.1.el9_6.src.rpm
2.2) Install Rocky Kernel source by shell
2.2.1) Install the necessary plugin
dnf install -y dnf-plugins-core
2.2.2) Enable the source repositories
dnf config-manager --set-enabled baseos-source appstream-source extras-source
2.2.3) Download the latest kernel SRPM
dnf download --source kernel
2.2.4) Extract the SRPM contents(This places kernel.spec in ~/rpmbuild/SPECS and sources/patches in ~/rpmbuild/SOURCES)
rpm -i kernel-*.src.rpm
2.2.5) Prepare the full source tree the way the spec expects
dnf install -y rpmdevtools
rpmbuild -bp ./rpmbuild/SPECS/kernel.spec
2.2.6) Untar kernel source file tarball under rpmbuild/SOURCES
3) config the kernel
3.1) workspace management with make
3.1.1) check support functionality: make help
3.1.2) cleanup source tree: make mrproper; make clean
3.1.3) configure the kernel
cp /boot/config-$(uname -r) <YourRootDir4Building>/.config
make O=<YourRootDir4Building> menuconfig
// Do other edits on <YourRootDir4Building>/.config
3.1.4) for Rocky, empty CONFIG_SYSTEM_TRUSTED_KEYS in <YourRootDir4Building>/.config
sed -ri '/CONFIG_SYSTEM_TRUSTED_KEYS/s/=.+/=""/g' <YourRootDir4Building>/.config
// CONFIG_SYSTEM_TRUSTED_KEYS="certs/rhel.pem"
// Otherwise, the building process will fail due to no certs target to make
4) compiling kernel
4.1) add extra version info: sed  -i 's/^EXTRAVERSION.*/EXTRAVERSION = -custom/'  Makefile
4.2) check kernel version: make O=<YourRootDir4Building> kernelversion
4.3) do compiling: make O=<YourRootDir4Building> -j $(nproc)
5) install
5.0) install modules[default path: /lib/modules/<KernelVersion>]
make O=<YourRootDir4Building> modules_install
make O=<YourRootDir4Building> INSTALL_MOD_STRIP=1 modules_install
5.1) install kernel
cp <YourRootDir4Building>/arch/x86/boot/bzImage /boot/vmlinuz-<KernelVersion>
// debug version
cp <YourRootDir4Building>/arch/x86/boot/vmlinux.bin /boot/vmlinux-<KernelVersion>
// symbolic table
cp <YourRootDir4Building>/System.map /boot/System.map-<KernelVersion>
6) install more packages for better Makefile template from "Linux Kernel Programming"
6.1) install indent
dnf install -y indent
6.2) install cppcheck
dnf install -y cppcheck
6.3) install sparse
dnf install -y sparse
6.4) install flawfinder # flawfinder released by snaps, a package platform
dnf install -y epel-release
dnf install -y snapd
ln -s /var/lib/snapd/snap /snap
systemctl enable snapd
systemctl start snapd
snap install flawfinder

# system configurations
Load module: CONFIG_MODULES=y
Unload module: CONFIG_MODULE_UNLOAD=y
Unload module forcefully(Ignore reference count of module): MODULE_FORCE_UNLOAD=y

# module building
Related files:
- Kconfig, mainly used with in-kernel tree building for feature selections
- Kbuild or Makefile, makefile name of kbuild system of linux kernel
Kbuild has higher priority over Makefile.

# Cross building
## rocky linux
1) install crossbuild tools: cross-gcc-common, cross-binutils-common, gcc-<ARCH_NAME>-linux-gnu, binutils-<ARCH_NAME>-linux-gnu
dnf install gcc-arm-linux-gnu
dnf install gcc-aarch64-linux-gnu
dnf install gcc-mips-linux-gnu
dnf install gcc-powerpc-linux-gnu
apt install crossbuild-essential-amd64
apt install crossbuild-essential-i386
apt install crossbuild-essential-arm64
apt install crossbuild-essential-armel
apt install crossbuild-essential-armhf
apt install crossbuild-essential-mips
apt install crossbuild-essential-mipsel
apt install crossbuild-essential-mipsr6
apt install crossbuild-essential-mipsr6el
apt install crossbuild-essential-mips64
apt install crossbuild-essential-mips64el
apt install crossbuild-essential-mips64r6
apt install crossbuild-essential-mips64r6el
apt install crossbuild-essential-powerpc
apt install crossbuild-essential-ppc64el
apt install crossbuild-essential-riscv64
apt install crossbuild-essential-s390x

# Usefual commands
1) check linux kernel tags
git log --date-order --graph --tags --simplify-by-decoration --pretty=format:'%ai %h %d'
2) check linux kernel release info
curl -L https://www.kernel.org/finger_banner
