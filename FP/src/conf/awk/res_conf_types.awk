BEGIN {
	type=0
}

{
	printf "#define RES_TYPE_%s %d\n",$1,type++
}
