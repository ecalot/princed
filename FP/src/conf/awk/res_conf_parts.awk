BEGIN {
	offset=0
	printf("\n/* parts */\n")
} 

{
	start=offset
	offset=end=$2+offset
	printf " /* %s, size=%d */\n#define res_get_part_%s(a) (((a)>>%d)&0x%08x)\n#define res_set_part_%s(a) (((a)&0x%08x)<<%d)\n",$1,$2,$1,start,(2^$2-1),$1,(2^$2-1),start
}
