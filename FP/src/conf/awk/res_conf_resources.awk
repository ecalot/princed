BEGIN {
	res=0
	coma=""
	filelist=""
	items=0
}

{
	oldres=res
	if (substr($2,1,2)=="OF") {
		if ((items!=0)&&(NF!=items)) {
			print "ERROR: item numbers do not match between different OFFSETs\n"
			exit -1;
		}
		if (items==0) items=NF
		if (offsets!="") offsets=( offsets "|" )
		offsets=( offsets "RES_MODS_HAS_" substr($2,3,1) )
		inc=3
		for (i=0;i<=((NF-3)/2);i++) {
			one=$inc
			inc++
			two=$inc
			inc++
			res++
			filelist=(filelist coma "(short)" ((two+64)*256+(one+64)) )
			coma=","
		}
	} else {
		for (i=3;i<=NF;i++) {
			if (match($i,/^[A-Z]*$/)) {
				filelist=(filelist coma "RES_FILE_" $i)
				res++
			} else if (match($i,/^[0-9]+-[0-9]*$/)) {
				split($i,a,"-")
				for (g=a[1];g<=a[2];g++) {
					filelist=(filelist coma g)
					res++
					coma=","
				}
			} else {
				res++
				filelist=(filelist coma $i)
			}
			coma=","
		}
		if (items==0) {
			extra="";
		} else {
			extra=( "|res_set_part_mods(" offsets ")" )
			offsets=""
		}
		printf "#define RES_%s res_set_part_type(RES_TYPE_%s)|res_set_part_from(%d)|res_set_part_size(%d)%s\n",$1,$2,oldres,res-oldres,extra
		if ((items!=0)&&(res-oldres!=items)) {
			print "ERROR: item numbers do not match between OFFSETs and IMGs\n"
			exit -1;
		}
		items=0
	}
}
END {
	print(" /* define codes */\n#define RES_LIST {" filelist "}\n")
}

