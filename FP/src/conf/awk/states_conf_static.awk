
BEGIN {
	currentCondition=-1
	currentAction=0
	printf("#define STATES_CONDITIONS={\\\n");
	tmp="conf/statesproperties.conf"
	while ((getline line < tmp) > 0) {
		gsub(/[	 ]+/, "-",line)
		m=index(line,"-")
		if (m) {
			word=substr(line,0,m)
			number=substr(line,m+1)
			defines[word]=number
		}
	}
	close(tmp)
	currentAnimation=0
}

{
	if ( $1 == "-" ) {
		if ($2=="-") {
			if ($3=="-") {
				if (listType == "next") {
					nextState=tolower($4)
				} else if (listType == "steps") {
					steps=$4
				} else if (listType == "conditions") {
					if (oldType != listType ) {
						oldType=listType
						currentCondition++
						printf("\t{esTrue,0}, /* condition number %d */\\\n",currentCondition)
						conditions=currentCondition+1
					}
					currentCondition++;
					result=$5
					if (defines[$5]) result=defines[$5]
					printf("\t{es%s,%d}, /* condition number %d */\\\n",$4,result,currentCondition)
				} else if (listType == "animation") {
					if (match($4,/^[0-9]+-[0-9]*$/)) {
						split($4,a,"-")
						for (g=a[1];g<=a[2];g++) {
							arrayAnimation[currentAnimation]=g
							currentAnimation++
						}
					} else {
						arrayAnimation[currentAnimation]=$4
						currentAnimation++
					}
				}
			} else {
				$3=tolower($3)
				oldType=listType
				listType=$3
			}
		} else {		
			if (conditions) {
				linkedState=currentState
				currentState=""
				stateList[linkedState]=currentAction
				if (linkedState) {
					linkedState=sprintf("State: %s (%d), ",linkedState,currentActions)
				}
				stateArray[currentAction]=sprintf("{/*initial condition*/ %d, /*next state*/ **replace*%s**, /*steps*/ %d},\\\n\t/* %sAction: %s, Animations: start=%d total=%d*/",conditions,nextState,steps,linkedState,rememberAction,startAnimation,currentAnimation-startAnimation)
				currentAction++;
			}

			startAnimation=currentAnimation
			listType=""
			conditions=0
			$2=tolower($2);
			if ($2 != "action") {
				print "Error! \"$0\" should be an action."
			}
			$2=$1=""
			rememberAction=substr($0,3)
		}
	} else {
		listType=""
		currentState=tolower($1)
	}

}

END {
	printf("\t{esTrue,0} /* the end */\\\n};\n\n#define STATES_ACTIONS={\\\n");
	linkedState=currentState
	stateArray[currentAction]=sprintf("{/*initial condition*/ %d, /*next state*/ **replace*%s**, /*steps*/ %d} /* %sAction: %s */\n",conditions,nextState,steps,linkedState,rememberAction)
	for (i=0;i<=currentAction;i++) {
		replaceStart=match(stateArray[i],/\*\*replace\*(.*)\*\*/)
		line=substr(stateArray[i],10+replaceStart)
		replaceEnd=match(line,/\*\*/)
		line=substr(line,0,replaceEnd)
		stateNumber=stateList[line]
		if (!stateNumber) stateNumber=0
		printf "\t%s%d /* %s */%s\\\n", substr(stateArray[i],0,replaceStart),stateNumber,line,substr(stateArray[i],11+replaceEnd+replaceStart)
	}
	printf("};\n\n#define STATES_ANIMATIONS={\\\n\t");
	coma=""
	for (i=0;i<currentAnimation;i++) {
		printf "%s%d",coma,arrayAnimation[i]
		if (i%10==9) printf("\\\n\t");
		coma=","
	}
	printf("\\\n};\n");
}

