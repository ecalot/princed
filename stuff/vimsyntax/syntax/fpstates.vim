" Vim syntax file
" Language:	FreePrince states file
" Maintainer:	Diego Essaya <dessaya@fi.uba.ar>
" Last Change:	$Date: 2004-10-30 01:28:04 $
" URL: http://www.fp.princed.com.ar/
" $Revision: 1.2 $

" For version 5.x: Clear all syntax items
" For version 6.x: Quit when a syntax file was already loaded
if version < 600
  syntax clear
elseif exists("b:current_syntax")
  finish
endif

syn case match

syntax match fpComment /#.*$/ contains=fpTodo
syntax match fpTodo /\<TODO\>/ contained
syntax match fpState /^\S\+:/me=e-1
syntax match fpAction /^\taction\>/ms=s+1 nextgroup=fpTitle skipwhite
syntax match fpTitle /[^#]*/ contained
syntax match fpProps /^\t\t\(conditions\|next\|steps\|animation\|level\|guardskill\|mark\)\>/ms=s+2
syntax match fpIdent /^\t\t\t\S\+/ms=s+3 nextgroup=fpValue skipwhite contains=fpNumber,fpOp
syntax match fpValue /\w\+/ contained

"syntax region fpFlags start=/^\t\t\t\d\+-\d\+\s\+/ms=e end=/$/ oneline
"syntax match fbRange1 /^\t\t\t\d\+/ nextgroup=fbRangeSep
"syntax match fbRangeSep /\-/ contained nextgroup=fbRange2
"syntax match fbRange2 /\d\+/ contained nextgroup=fbFlags skipwhite
"syntax match fbFlags /[ps]\+/ contained

syntax match fpNumber /\<\d\+\>/ contained
syntax match fpOp /[:;\-+()*&^%$@!~.,/]/

highlight link fpComment Comment
highlight link fpState   Label
highlight link fpAction  Keyword
highlight link fpProps   Keyword
highlight link fpIdent   Identifier
highlight link fpNumber  Constant
highlight link fpValue   Constant
highlight link fpOp      Operator
highlight link fpFlags   Operator
highlight link fpTitle   Function
highlight link fpTodo    Todo
