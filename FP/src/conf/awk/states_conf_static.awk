BEGIN {
	currentCondition=-1
	currentAction=0
	printf("#define STATES_MOVETYPES_RELATIVE 0\n")
	printf("#define STATES_MOVETYPES_ABSOLUTEFORWARD 1\n\n")
	printf("#define STATES_CONDITIONS {\\\n")
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
	latestLevel=-1
}

{
	if ( $1 == "-" ) {
		if ($2=="-") {
			if ($3=="-") {
				if (listType == "next") {
					nextState=tolower($4)
				} else if (listType == "steps") {
					moveOffset=$5
					moveType=tolower($4)
				} else if (listType == "conditions") {
					if (oldType != listType ) {
						oldType=listType
						currentCondition++
						printf("\t{esTrue,0}, /* condition number %d */\\\n",currentCondition)
						conditions=currentCondition+1
					}
					currentCondition++;
					if ($5!=sprintf("%d",$5)) {
						if (defines[$5]) {
							result=sprintf("STATES_COND_%s /* %d */",$5,defines[$5])
						} else {
							if ($5) {
								printf("Parsing error in states.conf: Condition modifier '%s' not recognized on uncommented line %d.\n",$5,NR)>"/dev/stderr"
								exit 22
							}
							result=0
						}
					} else {
						result=$5
					}
					printf("\t{es%s,%s}, /* condition number %d */\\\n",$4,result,currentCondition)
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
				} else if (listType == "level") {
					if (arrayLevel[$4]) {
						printf("Parsing error in states.conf: Redeclaration of level '%d' in uncommented line %d.\n",$4,NR)>"/dev/stderr"
						exit 23
					} else {
						arrayLevel[$4]=currentAction+1
						if ($4>latestLevel) {
							latestLevel=$4
						}
					}
				}
			} else {
				$3=tolower($3)
				oldType=listType
				listType=$3
			}
		} else {		
			if (conditions) {
				actionArray[currentAction,"description"]=rememberAction
				actionArray[currentAction,"isFirstInState"]=currentState
				actionArray[currentAction,"animationStart"]=startAnimation
				actionArray[currentAction,"animationSize"]=currentAnimation-startAnimation
				actionArray[currentAction,"conditionStart"]=conditions
				actionArray[currentAction,"nextState"]=nextState
				actionArray[currentAction,"moveType"]=moveType
				actionArray[currentAction,"moveOffset"]=moveOffset
				actionArray[currentAction,"lastComma"]=","
				currentAction++;
				currentState=""
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
		stateList[currentState]=currentAction
	}

}

END {
	actionArray[currentAction,"description"]=rememberAction
	actionArray[currentAction,"isFirstInState"]=currentState
	actionArray[currentAction,"animationStart"]=startAnimation
	actionArray[currentAction,"animationSize"]=currentAnimation-startAnimation
	actionArray[currentAction,"conditionStart"]=conditions
	actionArray[currentAction,"nextState"]=nextState
	actionArray[currentAction,"moveType"]=moveType
	actionArray[currentAction,"moveOffset"]=moveOffset
	actionArray[currentAction,"lastComma"]=""
	printf("\t{esTrue,0} /* the end */\\\n};\n\n#define STATES_ACTIONS {\\\n");

	for (i=0;i<=currentAction;i++) {
		nextStateId=stateList[actionArray[i,"nextState"]]
		#print comments
		printf("\t/* Action: %s (%d) \\\n",\
			actionArray[i,"description"],\
			i\
		)
		if (actionArray[i,"isFirstInState"]) {
			printf("\t * State: %s (%d) \\\n",\
				actionArray[i,"isFirstInState"],\
				i\
			)
		}
		printf("\t * Animations: animStart=%d, animSize=%d \\\n",\
			actionArray[i,"animationStart"],\
			actionArray[i,"animationSize"]\
		)
		printf("\t * Conditionals: conditionId=%d, nextStateId=%d (%s) \\\n",\
			actionArray[i,"conditionStart"],\
			nextStateId,\
			actionArray[i,"nextState"]\
		)
		printf("\t * Movements: moveType=%s, moveOffset=%d */ \\\n",\
			actionArray[i,"moveType"],\
			actionArray[i,"moveOffset"]\
		)
		#print array
		printf("\t\t{%d,STATES_MOVETYPES_%s,%d,%d,%d,%d}%s\\\n",\
			actionArray[i,"conditionStart"],\
			toupper(actionArray[i,"moveType"]),\
			actionArray[i,"moveOffset"],\
			nextStateId,\
			actionArray[i,"animationStart"],\
			actionArray[i,"animationSize"],\
			actionArray[i,"lastComma"]\
		)
	}

	printf("};\n\n#define STATES_ANIMATIONS {\\\n\t")
	coma=""
	for (i=0;i<currentAnimation;i++) {
		printf "%s%d",coma,arrayAnimation[i]
		if (i%10==9) printf("\\\n\t");
		coma=","
	}

	printf("\\\n};\n\n#define STATES_LEVELS {")
	coma=""
	for (i=0;i<=latestLevel;i++) {
		if (!arrayLevel[i]) {
			printf("Parsing error in states.conf: Level number %d is defined but level %d is not.\n",latestLevel,i)>"/dev/stderr"
			exit 24
		}
		printf "%s%s",coma,arrayLevel[i]-1
		coma=","
	}
	printf("};\n")

}

