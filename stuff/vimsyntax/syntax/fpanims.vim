" Vim syntax file
" Language:	FreePrince animations description file (anim.conf)
" Maintainer:	Diego Essaya <dessaya@fi.uba.ar>
" Last Change:	$Date: 2005-03-10 21:39:10 $
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

syntax match fpName /\S\+/ contained contains=fpIdent
syntax match fpWav /\<wav\>/ contained nextgroup=fpName skipwhite contains=fpKeyword
syntax match fpSpeaker /\<speaker\>/ contained nextgroup=fpName skipwhite contains=fpKeyword
syntax match fpMidi /\<midi\>/ contained nextgroup=fpName skipwhite contains=fpKeyword

"-------------------

syntax match fpObjMirror /\<\(yes\|no\)\>/ contained contains=fpKeyword
syntax match fpObjFloor /\<\(up\|middle\|down\|center\|\d\+\)\>/ contained nextgroup=fpObjMirror skipwhite contains=fpKeyword,fpNumber
syntax match fpObjLoc /\d\+/ contained nextgroup=fpObjFloor skipwhite contains=fpNumber
syntax match fpObjState /\S\+/ contained nextgroup=fpObjLoc skipwhite contains=fpIdent
syntax match fpObjDur /\<\(\d\+\|infinite\|untilexit\)\>/ contained nextgroup=fpObjState skipwhite contains=fpNumber,fpKeyword
syntax match fpObjName /\S\+/ contained nextgroup=fpObjDur skipwhite contains=fpIdent
syntax match fpObject /\<object\>/ contained nextgroup=fpObjName skipwhite contains=fpKeyword

"-------------------

syntax match fpImgY /\<\(\d\+\|top\|bottom\)\>/ contained contains=fpNumber,fpKeyword
syntax match fpImgX /\<\(\d\+\|left\|right\)\>/ contained nextgroup=fpImgY skipwhite contains=fpNumber,fpKeyword
syntax match fpImgLayer /\<\(bottom\|top\)\>/ contained nextgroup=fpImgX skipwhite contains=fpKeyword
syntax match fpImgDur /\<\(\d\+\|infinite\)\>/ contained nextgroup=fpImgLayer skipwhite contains=fpNumber,fpKeyword
syntax match fpImgName /\S\+/ contained nextgroup=fpImgDur skipwhite contains=fpIdent
syntax match fpImage /\<image\>/ contained nextgroup=fpImgName skipwhite contains=fpKeyword

"-------------------

syntax match fpFrameNo /^\s*\d\+/ contains=fpNumber nextgroup=fpImage,fpObject,fpMidi,fpWav,fpSpeaker skipwhite

"-------------------

syntax match fpAnimNum /\d\+/ contained contains=fpNumber
syntax match fpAnimName /\w\+/ contained nextgroup=fpAnimNum skipwhite contains=fpIdent
syntax match fpAnimLine /^animation\>/ contains=fpKeyword nextgroup=fpAnimName skipwhite

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

