" Vim syntax file
" Language:	FreePrince tiles.conf file
" Maintainer:	Diego Essaya <dessaya@fi.uba.ar>
" Last Change:	$Date: 2005-03-10 16:01:29 $
" URL: http://www.fp.princed.com.ar/
" $Revision: 1.2 $

" For version 5.x: Clear all syntax items
" For version 6.x: Quit when a syntax file was already loaded
if version < 600
  syntax clear
elseif exists("b:current_syntax")
  finish
endif

syn case ignore

syntax match fpGroups /^[^#]*/ contains=fpOp,fpIdent,fpNumber

syntax match fpTile /^tile\>/ nextgroup=fpName skipwhite
syntax match fpName /\<\w\+\>/ contained nextgroup=fpID skipwhite
syntax match fpID /\<\d\+\>/ contained nextgroup=fpGroup skipwhite
syntax match fpGroup /\<\w\+\>/ contained

syntax match fpComment /#.*$/ contains=fpTodo
syntax match fpTodo /\<TODO\>/ contained

syntax match fpNumber /\d\+/ contained
highlight link fpNumber      Number
syntax match fpIdent /\<\a\w*\>/ contained
highlight link fpIdent      Identifier
syntax match fpOp /[:;\-=<>+()*&^%$@!~.,/]/ contained
highlight link fpOp      Operator

highlight link fpComment Comment
highlight link fpTodo    Todo
highlight link fpTile    Keyword
highlight link fpName    Type
highlight link fpID      Constant
highlight link fpGroup   Function
highlight link fpGroups  Identifier
