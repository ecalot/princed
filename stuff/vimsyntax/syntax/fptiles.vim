" Vim syntax file
" Language:	FreePrince tiles.conf file
" Maintainer:	Diego Essaya <dessaya@fi.uba.ar>
" Last Change:	$Date: 2004-10-30 01:28:55 $
" URL: http://www.fp.princed.com.ar/
" $Revision: 1.1 $

" For version 5.x: Clear all syntax items
" For version 6.x: Quit when a syntax file was already loaded
if version < 600
  syntax clear
elseif exists("b:current_syntax")
  finish
endif

syn case ignore

syntax match fpComment /#.*$/ contains=fpTodo
syntax match fpTodo /\<TODO\>/ contained

syntax match fpTile /^tile\>/ nextgroup=fpName skipwhite
syntax match fpName /\<\w\+\>/ contained nextgroup=fpID skipwhite
syntax match fpID /\<\d\+\>/ contained nextgroup=fpGroup skipwhite
syntax match fpGroup /\<\w\+\>/ contained

syntax match fpGroups /^\(tile\)\@![^#]\+/

highlight link fpComment Comment
highlight link fpTodo    Todo
highlight link fpTile    Keyword
highlight link fpName    Type
highlight link fpID      Constant
highlight link fpGroup   Function
highlight link fpGroups  Identifier
