" Vim syntax file
" Language:	FreePrince flags.conf file
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

syntax match fpFlag /^\s*.\>/ nextgroup=fpDescription
syntax match fpDescription /[^#]*/ contained

highlight link fpComment Comment
highlight link fpTodo    Todo
highlight link fpFlag    Identifier
highlight link fpDescription    Function
