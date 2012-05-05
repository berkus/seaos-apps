export MTHREAD=-j2
cd ported
MOST_PKGS="bash binutils coreutils diffutils e2fsprogs findutils gawk grep grub gzip less make nano nasm ncurses newlib patch readline seaos-util sed tar termcap which"
ALL_PKGS="$MOST_PKGS gcc"

PKGS="$@"
if [ "$PKGS" = "all" ]; then
	PKGS="$ALL_PKGS"
fi
if [ "$PKGS" = "" ]; then
	PKGS="$ALL_PKGS"
fi
if [ "$PKGS" = "most" ]; then
	PKGS="$MOST_PKGS"
fi

echo building: $PKGS

for i in $PKGS; do
	echo Cleaning $i...
	rm -rf $i-* 2>/dev/null
	echo Building $i...
	sh patch_$i.sh
	if [ $? = 1 ]; then
		exit 1
	fi
	sh install_$i.sh
	if [ $? = 1 ]; then
		exit 1
	fi
done
echo Cleaning up...
for i in $PKGS; do
	rm -rf $i-* 2>/dev/null
done
rm -rf seaos-util
cd ..
