" Vim syntax file
" Language:	FreePrince resources description file (resources.conf)
" Maintainer:	Diego Essaya <dessaya@fi.uba.ar>
" Last Change:	$Date: 2005-03-10 22:00:33 $
" URL: http://www.fp.princed.com.ar/
" $Revision: 1.1 $

" For version 5.x: Clear all syntax items
" For version 6.x: Quit when a syntax file was already loaded
if version < 600
  syntax clear
elseif exists("b:current_syntax")
  finish
endif

syntax case ignore

syntax match fpError /\S[^#]*.*/ contained contains=fpComment
syntax match fpErrorLine /\s*\S\+/ contains=fpError
highlight link fpError   Error

"-------------------

syntax match fpRes /\<\(\d\+\|\a\w*\)\>[+-]\=/ contained contains=fpNumber,fpOp,fpIdent nextgroup=fpRes skipwhite
syntax match fpFile /\<\a\w*\>/ contained contains=fpIdent nextgroup=fpRes skipwhite
syntax match fpSND /\<SND\>/ contained contains=fpKeyword nextgroup=fpRes skipwhite
syntax match fpOF /\<OF\(L\|R\|D\)\>/ contained contains=fpKeyword nextgroup=fpRes skipwhite
syntax match fpType /\<\(LVL\|IMG\|PAL\)\>/ contained contains=fpKeyword nextgroup=fpFile skipwhite
syntax match fpBeginLine /^\a\w*\>/ contains=fpIdent nextgroup=fpType,fpOF,fpSND skipwhite

"-------------------

syntax match fpKeyword /\<\a\w*\>/ contained
highlight link fpKeyword  Keyword

syntax match fpIdent /\<\a\w*\>/ contained
highlight link fpIdent  Identifier

syntax match fpNumber /\d\+/ contained
highlight link fpNumber  Constant

syntax match fpOp /[:;\-+()*&^%$@!~.,/]/ contained
highlight link fpOp      Operator

syntax match fpComment /#.*$/ contains=fpTodo contained
syntax match fpCommentedLine /\s*#.*$/ contains=fpComment
highlight link fpComment Comment

syntax match fpTodo /\<TODO\>/ contained
highlight link fpTodo    Todo

