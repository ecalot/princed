" Vim syntax file
" Language:	FreePrince states description files (main.conf, titles.conf)
" Maintainer:	Diego Essaya <dessaya@fi.uba.ar>
" Last Change:	$Date: 2005-03-10 14:53:18 $
" URL: http://www.fp.princed.com.ar/
" $Revision: 1.5 $

" For version 5.x: Clear all syntax items
" For version 6.x: Quit when a syntax file was already loaded
if version < 600
  syntax clear
elseif exists("b:current_syntax")
  finish
endif

syntax case ignore

syntax sync minlines=50

syntax match fpError /\S[^#]*.*/ contained contains=fpComment
syntax match fpErrorLine /\s*\S\+/ contains=fpError
highlight link fpError   Error

syntax match fpEndLine /\s*.*/ contained contains=fpError

syntax match fpIdent /\w\+/ contained
highlight link fpIdent  Identifier
syntax match fpIdentLine /^\t\t\t\w\+/ contained contains=fpIdent nextgroup=fpEndLine
syntax match fpLastIdent /\w\+/ contained contains=fpIdent nextgroup=fpEndLine

"-------------------

syntax match fpCreateLine /^create\s\+exit/ nextgroup=fpLastIdent skipwhite
highlight link fpCreateLine Keyword

"-------------------

syntax match fpAnimNumber /\<-\=\d\+\>/ contained contains=fpNumber,fpOp
syntax match fpAnimRange /\<\d\+-\d\+\>/ contained contains=fpNumber,fpOp
syntax match fpAnimVMove /\s[@]-\=\d\+\>/ contained contains=fpNumber,fpOp
syntax match fpAnimHMove /\s[$]-\=\d\+\>/ contained contains=fpNumber,fpOp
syntax match fpAnimHMove /\s[$]-\=\d\+\>/ contained contains=fpNumber,fpOp
syntax match fpAnimFlags /\<[pcsgfutmxdh]\+\>/ contained contains=fpNumber,fpOp
highlight link fpAnimFlags  Operator
syntax match fpAnimLine /^\t\t\t.*/ contained nextgroup=fpEndLine contains=fpAnimNumber,fpAnimRange,fpAnimVMove,fpAnimHMove,fpAnimFlags,fpError
syntax match fpAnim /^\t\tanimation/ contained nextgroup=fpEndLine
highlight link fpAnim  Keyword

syntax region fpAnimList start=/^\t\tanimation/ end=/^\t\{,2}\S/me=s-1 contains=fpAnim,fpAnimLine,fpError

"-------------------

syntax match fpStepNumber /[+-]\=\d\+/ contained nextgroup=fpEndLine contains=fpOp,fpNumber
syntax match fpStepsLine /^\t\t\t\(relative\(Turn\)\=\|absoluteOn\(Start\|Stop\)\)/ contained nextgroup=fpStepNumber skipwhite
highlight link fpStepsLine  Keyword

syntax match fpSteps /^\t\tsteps/ contained nextgroup=fpEndLine
highlight link fpSteps  Keyword

syntax region fpStepsList start=/^\t\tsteps/ end=/^\t\{,2}\S/me=s-1 contains=fpSteps,fpStepsLine,fpError

"-------------------

syntax match fpGSLine contained /^\t\t\t\d\+/ contains=fpNumber nextgroup=fpEndLine
syntax match fpGuardSkill /^\t\tguardskill/ contained nextgroup=fpEndLine
highlight link fpGuardSkill  Keyword

syntax region fpGSList start=/^\t\tguardskill/ end=/^\t\{,2}\S/me=s-1 contains=fpGuardSkill,fpGSLine,fpError

"-------------------

syntax match fpLevelLine contained /^\t\t\t\d\+/ contains=fpNumber nextgroup=fpEndLine
syntax match fpLevel /^\t\tlevel/ contained nextgroup=fpEndLine
highlight link fpLevel  Keyword

syntax region fpLevelList start=/^\t\tlevel/ end=/^\t\{,2}\S/me=s-1 contains=fpLevel,fpLevelLine,fpError

"-------------------

syntax match fpMark /^\t\tmark/ contained nextgroup=fpEndLine
highlight link fpMark  Keyword

syntax region fpMarkList start=/^\t\tmark/ end=/^\t\{,2}\S/me=s-1 contains=fpMark,fpIdentLine,fpError

"-------------------

syntax match fpExitLine /^\t\t\texit/ contained nextgroup=fpLastIdent skipwhite
highlight link fpExitLine  Keyword
syntax match fpNext /^\t\tnext/ contained nextgroup=fpEndLine
highlight link fpNext  Keyword

syntax region fpNextList start=/^\t\tnext/ end=/^\t\{,2}\S/me=s-1 contains=fpNext,fpIdentLine,fpError,fpExitLine

"-------------------

syntax match fpMapCond /^\t\t\tMap\(Not\)\=\(On\|Up\|Down\|Forward\|Back\)\+\>/ contained nextgroup=fpMapState skipwhite
highlight link fpMapCond  Keyword
syntax keyword fpMapState contained walkable exitdoor potion space block spikes nextgroup=fpEndLine
highlight link fpMapState  Constant

syntax match fpKeyCond /^\t\t\tKey\(Up\|Forward\|Back\|Down\|Shift\)\+\>/ contained nextgroup=fpKeyState skipwhite
highlight link fpKeyCond  Keyword
syntax keyword fpKeyState contained pressed nextgroup=fpEndLine
highlight link fpKeyState  Constant

syntax match fpNoneCond contained /^\t\t\tnone/ nextgroup=fpEndLine
highlight link fpNoneCond  Keyword
syntax match fpOtherCond contained /^\t\t\tForwardTileNearerThan\s\+\d\+/ contains=fpNumber nextgroup=fpEndLine
highlight link fpOtherCond  Keyword

syntax match fpCondition /^\t\tconditions\=/ nextgroup=fpEndLine
highlight link fpCondition  Keyword

syntax region fpCondList start=/^\t\tconditions\=/ end=/^\t\{,2}\S/me=s-1 contains=fpCondition,fpMapCond,fpKeyCond,fpNoneCond,fpOtherCond,fpError


"-------------------

syntax match fpAction /^\taction\>/ nextgroup=fpTitle skipwhite
syntax match fpTitle /.*/ contained contains=fpComment
highlight link fpAction  Function
highlight link fpTitle   String

"-------------------

syntax match fpState /^\w\+:/ contains=fpIdent,fpOp nextgroup=fpEndLine

"-------------------

syntax match fpNumber /\d\+/ contains=fpOp contained
highlight link fpNumber  Constant

syntax match fpOp /[:;\-+()*&^%$@!~.,/]/ contained
highlight link fpOp      Operator

syntax match fpComment /#.*$/ contains=fpTodo contained
syntax match fpCommentedLine /\s*#.*$/ contains=fpComment
highlight link fpComment Comment

syntax match fpTodo /\<TODO\>/ contained
highlight link fpTodo    Todo

