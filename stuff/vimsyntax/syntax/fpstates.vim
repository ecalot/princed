" Vim syntax file
" Language:	FreePrince states description files (main.conf, titles.conf)
" Maintainer:	Diego Essaya <dessaya@fi.uba.ar>
" Last Change:	$Date: 2005-03-10 02:58:37 $
" URL: http://www.fp.princed.com.ar/
" $Revision: 1.3 $

" For version 5.x: Clear all syntax items
" For version 6.x: Quit when a syntax file was already loaded
if version < 600
  syntax clear
elseif exists("b:current_syntax")
  finish
endif

syntax case ignore

syntax match fpError /\S[^#]*.*/ contained contains=fpComment
syntax match fpEndLine /\s*.*/ contained contains=fpError

syntax match fpErrorLine /\s*\S\+/ contains=fpError

syntax match fpIdent /^\t\t\t\w\+/ contained nextgroup=fpEndLine

"-------------------

syntax match fpAnimNumber /\<-\=\d\+\>/ contained contains=fpNumber,fpOp
syntax match fpAnimRange /\<\d\+-\d\+\>/ contained contains=fpNumber,fpOp
syntax match fpAnimVMove /\s[@]-\=\d\+\>/ contained contains=fpNumber,fpOp
syntax match fpAnimHMove /\s[$]-\=\d\+\>/ contained contains=fpNumber,fpOp
syntax match fpAnimHMove /\s[$]-\=\d\+\>/ contained contains=fpNumber,fpOp
syntax match fpAnimFlags /\<[xdpsufh]\+\>/ contained contains=fpNumber,fpOp
syntax match fpAnimLine /^\t\t\t.*/ contained nextgroup=fpEndLine contains=fpAnimNumber,fpAnimRange,fpAnimVMove,fpAnimHMove,fpAnimFlags,fpError
syntax match fpAnim /^\t\tanimation/ contained nextgroup=fpEndLine

syntax region fpAnimList start=/^\t\tanimation/ end=/^\t\{,2}\S/me=s-1 contains=fpAnim,fpAnimLine,fpError

"-------------------

syntax match fpStepNumber /[+-]\=\d\+/ contained nextgroup=fpEndLine contains=fpOp,fpNumber
syntax match fpStepsLine /^\t\t\t\(relative\(Turn\)\=\|absoluteOn\(Start\|Stop\)\)/ contained nextgroup=fpStepNumber skipwhite
syntax match fpSteps /^\t\tsteps/ contained nextgroup=fpEndLine

syntax region fpStepsList start=/^\t\tsteps/ end=/^\t\{,2}\S/me=s-1 contains=fpSteps,fpStepsLine,fpError

"-------------------

syntax match fpGSLine contained /^\t\t\t\d\+/ contains=fpNumber nextgroup=fpEndLine
syntax match fpGuardSkill /^\t\tguardskill/ contained nextgroup=fpEndLine

syntax region fpGSList start=/^\t\tguardskill/ end=/^\t\{,2}\S/me=s-1 contains=fpGuardSkill,fpGSLine,fpError

"-------------------

syntax match fpLevelLine contained /^\t\t\t\d\+/ contains=fpNumber nextgroup=fpEndLine
syntax match fpLevel /^\t\tlevel/ contained nextgroup=fpEndLine

syntax region fpLevelList start=/^\t\tlevel/ end=/^\t\{,2}\S/me=s-1 contains=fpLevel,fpLevelLine,fpError

"-------------------

syntax match fpMark /^\t\tmark/ contained nextgroup=fpEndLine

syntax region fpMarkList start=/^\t\tmark/ end=/^\t\{,2}\S/me=s-1 contains=fpMark,fpIdent,fpError

"-------------------

syntax match fpNext /^\t\tnext/ contained nextgroup=fpEndLine

syntax region fpNextList start=/^\t\tnext/ end=/^\t\{,2}\S/me=s-1 contains=fpNext,fpIdent,fpError

"-------------------

syntax match fpMapCond /^\t\t\tMap\(Not\)\=\(On\|Up\|Down\|Forward\|Back\)\+\>/ contained nextgroup=fpMapState skipwhite
syntax keyword fpMapState contained walkable exitdoor potion space block spikes nextgroup=fpEndLine

syntax match fpKeyCond /^\t\t\tKey\(Up\|Forward\|Back\|Down\|Shift\)\+\>/ contained nextgroup=fpKeyState skipwhite
syntax keyword fpKeyState contained pressed nextgroup=fpEndLine

syntax match fpNoneCond contained /^\t\t\tnone/ nextgroup=fpEndLine
syntax match fpOtherCond contained /^\t\t\tForwardTileNearerThan\s\+\d\+/ contains=fpNumber nextgroup=fpEndLine

syntax match fpCondition /^\t\tconditions\=/ nextgroup=fpEndLine

syntax region fpCondList start=/^\t\tconditions\=/ end=/^\t\{,2}\S/me=s-1 contains=fpCondition,fpMapCond,fpKeyCond,fpNoneCond,fpOtherCond,fpError

"-------------------

syntax match fpAction /^\taction\>/ nextgroup=fpTitle skipwhite
syntax match fpTitle /.*/ contained contains=fpComment

"-------------------

syntax match fpState /^\w\+:/ contains=fpOp nextgroup=fpEndLine

"-------------------

syntax match fpNumber /\d\+/ contains=fpOp contained
syntax match fpOp /[:;\-+()*&^%$@!~.,/]/ contained

syntax match fpComment /#.*$/ contains=fpTodo contained
syntax match fpTodo /\<TODO\>/ contained

syntax match fpCommentedLine /\s*#.*$/ contains=fpComment

highlight link fpComment Comment
highlight link fpState  Label
highlight link fpAction  Keyword

highlight link fpCondition  Keyword
highlight link fpNoneCond  Identifier
highlight link fpOtherCond  Identifier
highlight link fpKeyCond  Identifier
highlight link fpKeyState  Constant
highlight link fpMapCond  Identifier
highlight link fpMapState  Constant

highlight link fpNext  Keyword

highlight link fpMark  Keyword

highlight link fpSteps  Keyword
highlight link fpStepsLine  Identifier

highlight link fpAnim  Keyword
highlight link fpAnimFlags  Operator

highlight link fpLevel  Keyword

highlight link fpGuardSkill  Keyword

highlight link fpIdent  Identifier
highlight link fpNumber  Constant
highlight link fpOp      Operator
highlight link fpTitle   Function
highlight link fpTodo    Todo
highlight link fpError   Error
