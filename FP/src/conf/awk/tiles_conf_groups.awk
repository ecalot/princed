#   Princed V3 - Prince of Persia Level Editor for PC Version
#   Copyright (C) 2003 Princed Development Team
#
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation; either version 2 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program; if not, write to the Free Software
#   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
#   The authors of this program may be contacted at http://forum.princed.com.ar

# tiles_conf_groups.awk: FreePrince : tiles.conf to tiles_conf_groups.h parser
# ���������������������
# Copyright 2004, 2003 Princed Development Team
#  Created: 19 Oct 2004
#
#  Author: Enrique Calot <ecalot.cod@princed.com.ar>
#
# Note:
#  DO NOT remove this copyright notice
#

/^[ ]*tile .*$/ {
	tile=sprintf("T_%s",$2)
}

$0 !~ /^[ ]*(#.*|tile .*|[ ]*)$/ {
	for (i=1;i<=NF;i++) {
		if (!total[$i]) total[$i]=0
		groups[$i,total[$i]]=tile
		total[$i]++;
	}
}

END {
	offset=0
	coma=""
	printf "#define TILES_GROUP_LIST {"
	for (group in total) {
		offsets[group]=offset
		for (i=0;i<total[group];i++) {
			printf "%s%s+1",coma,groups[group,i]
			coma=","
			offset++
			if (offset%5==0) printf("\\\n")
		}
		printf ",0"
		offset++
		if (offset%5==0) printf("\\\n")
	}
	printf "}\n"
	for (group in offsets) {
		printf "#define TG_%s %d\n",group,offsets[group]
	}
}
