" Vim syntax file
" Language:	FreePrince flags.conf file
" Maintainer:	Diego Essaya <dessaya@fi.uba.ar>
" Last Change:	$Date: 2005-03-10 15:02:12 $
" URL: http://www.fp.princed.com.ar/
" $Revision: 1.2 $

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

syntax match fpFlagDesc /.*/ contained contains=fpComment
highlight link fpFlagDesc String
syntax match fpFlagline /^\a\+/ nextgroup=fpFlagDesc contains=fpComment
highlight link fpFlagLine Operator

"-------------------

syntax match fpComment /#.*$/ contains=fpTodo contained
syntax match fpCommentedLine /\s*#.*$/ contains=fpComment
highlight link fpComment Comment

syntax match fpTodo /\<TODO\>/ contained
highlight link fpTodo    Todo

