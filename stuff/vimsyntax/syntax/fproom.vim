" Vim syntax file
" Language:	FreePrince resources description file (resources.conf)
" Maintainer:	Diego Essaya <dessaya@fi.uba.ar>
" Last Change:	$Date: 2005-03-10 22:26:43 $
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

syntax match fpDrawY /\S\+/ contained contains=fpIdent,fpOp,fpNumber nextgroup=fpCondition skipwhite
syntax match fpDrawX /\S\+/ contained contains=fpIdent,fpOp,fpNumber nextgroup=fpDrawY skipwhite
syntax match fpDrawLine /^\s*draw\S*\>/ contains=fpKeyword nextgroup=fpDrawX skipwhite

"-------------------

syntax match fpCondition /.*/ contains=fpIdent,fpOp,fpNumber,fpError
syntax match fpCondLine /^\s*cond\>/ contains=fpKeyword nextgroup=fpExpression skipwhite

"-------------------

syntax match fpXXIdent /\<\a\w*\>/ contained contains=fpIdent
syntax match fpXXLine /^XX\>/ contains=fpKeyword nextgroup=fpXXIdent skipwhite

"-------------------

syntax match fpKeyword /\<\a\w*\>/ contained
highlight link fpKeyword  Keyword

syntax match fpIdent /\<\a\w*\>/ contained
highlight link fpIdent  Identifier

syntax match fpNumber /\d\+/ contained
highlight link fpNumber  Constant

syntax match fpOp /[:;\-=<>+()*&^%$@|!~.,/]/ contained
highlight link fpOp      Operator

syntax match fpComment /#.*$/ contains=fpTodo contained
syntax match fpCommentedLine /\s*#.*$/ contains=fpComment
highlight link fpComment Comment

syntax match fpTodo /\<TODO\>/ contained
highlight link fpTodo    Todo

