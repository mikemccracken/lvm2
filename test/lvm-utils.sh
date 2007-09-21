# Put lvm-related utilities here.
# This file is sourced from test-lib.sh.

# Copyright (C) 2007 Red Hat, Inc. All rights reserved.
#
# This copyrighted material is made available to anyone wishing to use,
# modify, copy, or redistribute it subject to the terms and conditions
# of the GNU General Public License v.2.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

export LVM_SUPPRESS_FD_WARNINGS=1

ME=$(basename "$0")
warn() { echo >&2 "$ME: $@"; }

unsafe_losetup_()
{
  f=$1
  # Prefer the race-free losetup from recent util-linux-ng.
  dev=$(losetup --find --show "$f" 2>/dev/null) \
      && { echo "$dev"; return 0; }

  # If that fails, try to use util-linux-ng's -f "find-device" option.
  dev=$(losetup -f 2>/dev/null) \
      && losetup "$dev" "$f" \
      && { echo "$dev"; return 0; }

  # Last resort: iterate through /dev/loop{,/}{0,1,2,3,4,5,6,7,8,9}
  for slash in '' /; do
    for i in 0 1 2 3 4 5 6 7 8 9; do
      dev=/dev/loop$slash$i
      losetup $dev > /dev/null 2>&1 && continue;
      losetup "$dev" "$f" > /dev/null && { echo "$dev"; return 0; }
      break
    done
  done

  return 1
}

loop_setup_()
{
  file=$1
  dd if=/dev/zero of="$file" bs=1M count=1 seek=1000 > /dev/null 2>&1 \
    || { warn "loop_setup_ failed: Unable to create tmp file $file"; return 1; }

  # NOTE: this requires a new enough version of losetup
  dev=$(unsafe_losetup_ "$file" 2>/dev/null) \
    || { warn "loop_setup_ failed: Unable to create loopback device"; return 1; }

  echo "$dev"
  return 0;
}


check_pv_size_()
{
  return $(test $(pvs --noheadings -o pv_free $1) == $2)
}

check_lv_size_()
{
  return $(test $(lvs --noheadings -o lv_size $1) == $2)
}
