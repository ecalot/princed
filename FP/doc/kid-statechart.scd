Storage 
{
	{ Format 1.33 }
	{ GeneratedFrom TSCD-version-2.20 }
	{ WrittenBy diego }
	{ WrittenOn "" }
}

Document 
{
	{ Type "Statechart Diagram" }
	{ Name kid-statechart.scd }
	{ Author diego }
	{ CreatedOn "" }
	{ Annotation "" }
	{ Hierarchy True }
}

Page 
{
	{ PageOrientation Landscape }
	{ PageSize A4 }
	{ ShowHeaders False }
	{ ShowFooters False }
	{ ShowNumbers False }
}

Scale 
{
	{ ScaleValue 1 }
}

# GRAPH NODES

SCDOrState 1
{
	{ Name "Stand" }
	{ Annotation "" }
	{ Parent 0 }
	{ Index "" }
}

SCDOrState 2
{
	{ Name "Turning(i)" }
	{ Annotation "" }
	{ Parent 0 }
	{ Index "" }
}

SCDOrState 14
{
	{ Name "Walking(i)" }
	{ Annotation "" }
	{ Parent 0 }
	{ Index "" }
}

SCDOrState 22
{
	{ Name "Run" }
	{ Annotation "" }
	{ Parent 0 }
	{ Index "" }
}

SCDOrState 23
{
	{ Name "Start run(i)" }
	{ Annotation "" }
	{ Parent 0 }
	{ Index "" }
}

SCDOrState 34
{
	{ Name "Couch" }
	{ Annotation "" }
	{ Parent 0 }
	{ Index "" }
}

SCDOrState 35
{
	{ Name "couching(i)" }
	{ Annotation "" }
	{ Parent 0 }
	{ Index "" }
}

SCDOrState 36
{
	{ Name "standing(i)" }
	{ Annotation "" }
	{ Parent 0 }
	{ Index "" }
}

SCDOrState 48
{
	{ Name "Crawling(i)" }
	{ Annotation "" }
	{ Parent 0 }
	{ Index "" }
}

SCDOrState 60
{
	{ Name "Running(i)" }
	{ Annotation "" }
	{ Parent 0 }
	{ Index "" }
}

SCDOrState 68
{
	{ Name "Stop_run(i)" }
	{ Annotation "" }
	{ Parent 0 }
	{ Index "" }
}

SCDOrState 69
{
	{ Name "idle_standing(i)" }
	{ Annotation "" }
	{ Parent 0 }
	{ Index "" }
}

SCDOrState 70
{
	{ Name "idle_couch(i)" }
	{ Annotation "" }
	{ Parent 0 }
	{ Index "" }
}

SCDOrState 71
{
	{ Name "turn_running(i)" }
	{ Annotation "" }
	{ Parent 0 }
	{ Index "" }
}

SCDOrState 94
{
	{ Name "go_up(i)" }
	{ Annotation "" }
	{ Parent 0 }
	{ Index "" }
}

SCDOrState 95
{
	{ Name "jump_running(i)" }
	{ Annotation "" }
	{ Parent 0 }
	{ Index "" }
}

SCDOrState 96
{
	{ Name "jumping(i)" }
	{ Annotation "" }
	{ Parent 0 }
	{ Index "" }
}

# GRAPH EDGES

SCDTransitionEdge 3
{
	{ Name "([<-] or [->]) in\ropposite direction" }
	{ Annotation "" }
	{ Parent 0 }
	{ Subject1 1 }
	{ Subject2 2 }
}

SCDTransitionEdge 10
{
	{ Name "i==n: dir = !dir" }
	{ Annotation "" }
	{ Parent 0 }
	{ Subject1 2 }
	{ Subject2 1 }
}

SCDTransitionEdge 15
{
	{ Name "(Shift+[->]or[<-]) in\rsame direction" }
	{ Annotation "" }
	{ Parent 0 }
	{ Subject1 1 }
	{ Subject2 14 }
}

SCDTransitionEdge 16
{
	{ Name "" }
	{ Annotation "" }
	{ Parent 0 }
	{ Subject1 14 }
	{ Subject2 1 }
}

SCDTransitionEdge 30
{
	{ Name "[<-]or[->] in \rsame direction" }
	{ Annotation "" }
	{ Parent 0 }
	{ Subject1 1 }
	{ Subject2 23 }
}

SCDTransitionEdge 31
{
	{ Name "i==n" }
	{ Annotation "" }
	{ Parent 0 }
	{ Subject1 23 }
	{ Subject2 22 }
}

SCDTransitionEdge 37
{
	{ Name "i==n" }
	{ Annotation "" }
	{ Parent 0 }
	{ Subject1 36 }
	{ Subject2 1 }
}

SCDTransitionEdge 38
{
	{ Name "release[down]" }
	{ Annotation "" }
	{ Parent 0 }
	{ Subject1 34 }
	{ Subject2 36 }
}

SCDTransitionEdge 39
{
	{ Name "i==n" }
	{ Annotation "" }
	{ Parent 0 }
	{ Subject1 35 }
	{ Subject2 34 }
}

SCDTransitionEdge 40
{
	{ Name "[down]" }
	{ Annotation "" }
	{ Parent 0 }
	{ Subject1 1 }
	{ Subject2 35 }
}

SCDTransitionEdge 50
{
	{ Name "[<-]or[->] in\rsame direction" }
	{ Annotation "" }
	{ Parent 0 }
	{ Subject1 34 }
	{ Subject2 48 }
}

SCDTransitionEdge 51
{
	{ Name "i==n" }
	{ Annotation "" }
	{ Parent 0 }
	{ Subject1 48 }
	{ Subject2 34 }
}

SCDTransitionEdge 61
{
	{ Name "[->]or[<-] in\rsame direction" }
	{ Annotation "" }
	{ Parent 0 }
	{ Subject1 22 }
	{ Subject2 60 }
}

SCDTransitionEdge 62
{
	{ Name "i==n" }
	{ Annotation "" }
	{ Parent 0 }
	{ Subject1 60 }
	{ Subject2 22 }
}

SCDTransitionEdge 72
{
	{ Name "no key" }
	{ Annotation "" }
	{ Parent 0 }
	{ Subject1 1 }
	{ Subject2 69 }
}

SCDTransitionEdge 73
{
	{ Name "i==n" }
	{ Annotation "" }
	{ Parent 0 }
	{ Subject1 69 }
	{ Subject2 1 }
}

SCDTransitionEdge 74
{
	{ Name "[down]" }
	{ Annotation "" }
	{ Parent 0 }
	{ Subject1 34 }
	{ Subject2 70 }
}

SCDTransitionEdge 75
{
	{ Name "i==n" }
	{ Annotation "" }
	{ Parent 0 }
	{ Subject1 70 }
	{ Subject2 34 }
}

SCDTransitionEdge 76
{
	{ Name "no key" }
	{ Annotation "" }
	{ Parent 0 }
	{ Subject1 22 }
	{ Subject2 68 }
}

SCDTransitionEdge 77
{
	{ Name "i==n" }
	{ Annotation "" }
	{ Parent 0 }
	{ Subject1 68 }
	{ Subject2 1 }
}

SCDTransitionEdge 78
{
	{ Name "[->]or[<-] in\ropposite direction" }
	{ Annotation "" }
	{ Parent 0 }
	{ Subject1 22 }
	{ Subject2 71 }
}

SCDTransitionEdge 79
{
	{ Name "i==n:\rdir = !dir" }
	{ Annotation "" }
	{ Parent 0 }
	{ Subject1 71 }
	{ Subject2 22 }
}

SCDTransitionEdge 92
{
	{ Name "[down]" }
	{ Annotation "" }
	{ Parent 0 }
	{ Subject1 22 }
	{ Subject2 35 }
}

SCDTransitionEdge 97
{
	{ Name "[up]" }
	{ Annotation "" }
	{ Parent 0 }
	{ Subject1 1 }
	{ Subject2 94 }
}

SCDTransitionEdge 98
{
	{ Name "i==n" }
	{ Annotation "" }
	{ Parent 0 }
	{ Subject1 94 }
	{ Subject2 1 }
}

SCDTransitionEdge 99
{
	{ Name "[up]+[->]or[<-] in\rsame direction" }
	{ Annotation "" }
	{ Parent 0 }
	{ Subject1 22 }
	{ Subject2 95 }
}

SCDTransitionEdge 100
{
	{ Name "i==n" }
	{ Annotation "" }
	{ Parent 0 }
	{ Subject1 95 }
	{ Subject2 22 }
}

SCDTransitionEdge 101
{
	{ Name "[up]+[dir]" }
	{ Annotation "" }
	{ Parent 0 }
	{ Subject1 1 }
	{ Subject2 96 }
}

SCDTransitionEdge 102
{
	{ Name "i==n" }
	{ Annotation "" }
	{ Parent 0 }
	{ Subject1 96 }
	{ Subject2 1 }
}

# VIEWS AND GRAPHICAL SHAPES

View 5
{
	{ Index "0" }
	{ Parent 0 }
}

RoundedBox 6
{
	{ View 5 }
	{ Subject 1 }
	{ Position 600 420 }
	{ Size 152 114 }
	{ Color "black" }
	{ LineWidth 1 }
	{ LineStyle Solid }
	{ FillStyle Unfilled }
	{ FillColor "white" }
	{ FixedName False }
	{ Font "-*-courier-medium-r-normal--10*" }
	{ TextAlignment Center }
	{ TextColor "black" }
	{ NameUnderlined False }
}

RoundedBox 7
{
	{ View 5 }
	{ Subject 2 }
	{ Position 930 320 }
	{ Size 112 24 }
	{ Color "black" }
	{ LineWidth 1 }
	{ LineStyle Solid }
	{ FillStyle Unfilled }
	{ FillColor "white" }
	{ FixedName False }
	{ Font "-*-courier-medium-r-normal--10*" }
	{ TextAlignment Center }
	{ TextColor "black" }
	{ NameUnderlined False }
}

Line 8
{
	{ View 5 }
	{ Subject 3 }
	{ FromShape 6 }
	{ ToShape 7 }
	{ Curved True }
	{ End1 Empty }
	{ End2 FilledArrow }
	{ Points 4 }
	{ Point 667 363 }
	{ Point 730 310 }
	{ Point 830 290 }
	{ Point 890 308 }
	{ NamePosition 780 300 }
	{ Color "black" }
	{ LineWidth 1 }
	{ LineStyle Solid }
	{ FixedName False }
	{ Font "-*-courier-medium-r-normal--10*" }
	{ TextAlignment Center }
	{ TextColor "black" }
	{ NameUnderlined False }
}

Line 12
{
	{ View 5 }
	{ Subject 10 }
	{ FromShape 7 }
	{ ToShape 6 }
	{ Curved True }
	{ End1 Empty }
	{ End2 FilledArrow }
	{ Points 4 }
	{ Point 918 332 }
	{ Point 860 390 }
	{ Point 750 410 }
	{ Point 676 410 }
	{ NamePosition 805 400 }
	{ Color "black" }
	{ LineWidth 1 }
	{ LineStyle Solid }
	{ FixedName False }
	{ Font "-*-courier-medium-r-normal--10*" }
	{ TextAlignment Center }
	{ TextColor "black" }
	{ NameUnderlined False }
}

RoundedBox 18
{
	{ View 5 }
	{ Subject 14 }
	{ Position 880 500 }
	{ Size 114 26 }
	{ Color "black" }
	{ LineWidth 1 }
	{ LineStyle Solid }
	{ FillStyle Unfilled }
	{ FillColor "white" }
	{ FixedName False }
	{ Font "-*-courier-medium-r-normal--10*" }
	{ TextAlignment Center }
	{ TextColor "black" }
	{ NameUnderlined False }
}

Line 19
{
	{ View 5 }
	{ Subject 15 }
	{ FromShape 6 }
	{ ToShape 18 }
	{ Curved True }
	{ End1 Empty }
	{ End2 FilledArrow }
	{ Points 4 }
	{ Point 676 440 }
	{ Point 730 440 }
	{ Point 810 450 }
	{ Point 862 487 }
	{ NamePosition 770 445 }
	{ Color "black" }
	{ LineWidth 1 }
	{ LineStyle Solid }
	{ FixedName False }
	{ Font "-*-courier-medium-r-normal--10*" }
	{ TextAlignment Center }
	{ TextColor "black" }
	{ NameUnderlined False }
}

Line 20
{
	{ View 5 }
	{ Subject 16 }
	{ FromShape 18 }
	{ ToShape 6 }
	{ Curved True }
	{ End1 Empty }
	{ End2 FilledArrow }
	{ Points 4 }
	{ Point 823 500 }
	{ Point 790 500 }
	{ Point 710 490 }
	{ Point 676 468 }
	{ NamePosition 750 495 }
	{ Color "black" }
	{ LineWidth 1 }
	{ LineStyle Solid }
	{ FixedName False }
	{ Font "-*-courier-medium-r-normal--10*" }
	{ TextAlignment Center }
	{ TextColor "black" }
	{ NameUnderlined False }
}

RoundedBox 26
{
	{ View 5 }
	{ Subject 22 }
	{ Position 650 750 }
	{ Size 152 112 }
	{ Color "black" }
	{ LineWidth 1 }
	{ LineStyle Solid }
	{ FillStyle Unfilled }
	{ FillColor "white" }
	{ FixedName False }
	{ Font "-*-courier-medium-r-normal--10*" }
	{ TextAlignment Center }
	{ TextColor "black" }
	{ NameUnderlined False }
}

RoundedBox 27
{
	{ View 5 }
	{ Subject 23 }
	{ Position 710 600 }
	{ Size 142 30 }
	{ Color "black" }
	{ LineWidth 1 }
	{ LineStyle Solid }
	{ FillStyle Unfilled }
	{ FillColor "white" }
	{ FixedName False }
	{ Font "-*-courier-medium-r-normal--10*" }
	{ TextAlignment Center }
	{ TextColor "black" }
	{ NameUnderlined False }
}

Line 32
{
	{ View 5 }
	{ Subject 30 }
	{ FromShape 6 }
	{ ToShape 27 }
	{ Curved False }
	{ End1 Empty }
	{ End2 FilledArrow }
	{ Points 2 }
	{ Point 635 477 }
	{ Point 701 585 }
	{ NamePosition 679 526 }
	{ Color "black" }
	{ LineWidth 1 }
	{ LineStyle Solid }
	{ FixedName False }
	{ Font "-*-courier-medium-r-normal--10*" }
	{ TextAlignment Center }
	{ TextColor "black" }
	{ NameUnderlined False }
}

Line 33
{
	{ View 5 }
	{ Subject 31 }
	{ FromShape 27 }
	{ ToShape 26 }
	{ Curved False }
	{ End1 Empty }
	{ End2 FilledArrow }
	{ Points 2 }
	{ Point 704 615 }
	{ Point 672 694 }
	{ NamePosition 714 655 }
	{ Color "black" }
	{ LineWidth 1 }
	{ LineStyle Solid }
	{ FixedName False }
	{ Font "-*-courier-medium-r-normal--10*" }
	{ TextAlignment Center }
	{ TextColor "black" }
	{ NameUnderlined False }
}

RoundedBox 41
{
	{ View 5 }
	{ Subject 34 }
	{ Position 230 550 }
	{ Size 132 122 }
	{ Color "black" }
	{ LineWidth 1 }
	{ LineStyle Solid }
	{ FillStyle Unfilled }
	{ FillColor "white" }
	{ FixedName False }
	{ Font "-*-courier-medium-r-normal--10*" }
	{ TextAlignment Center }
	{ TextColor "black" }
	{ NameUnderlined False }
}

RoundedBox 42
{
	{ View 5 }
	{ Subject 35 }
	{ Position 430 570 }
	{ Size 120 30 }
	{ Color "black" }
	{ LineWidth 1 }
	{ LineStyle Solid }
	{ FillStyle Unfilled }
	{ FillColor "white" }
	{ FixedName False }
	{ Font "-*-courier-medium-r-normal--10*" }
	{ TextAlignment Center }
	{ TextColor "black" }
	{ NameUnderlined False }
}

RoundedBox 43
{
	{ View 5 }
	{ Subject 36 }
	{ Position 400 450 }
	{ Size 110 24 }
	{ Color "black" }
	{ LineWidth 1 }
	{ LineStyle Solid }
	{ FillStyle Unfilled }
	{ FillColor "white" }
	{ FixedName False }
	{ Font "-*-courier-medium-r-normal--10*" }
	{ TextAlignment Center }
	{ TextColor "black" }
	{ NameUnderlined False }
}

Line 44
{
	{ View 5 }
	{ Subject 37 }
	{ FromShape 43 }
	{ ToShape 6 }
	{ Curved False }
	{ End1 Empty }
	{ End2 FilledArrow }
	{ Points 2 }
	{ Point 454 442 }
	{ Point 524 431 }
	{ NamePosition 487 427 }
	{ Color "black" }
	{ LineWidth 1 }
	{ LineStyle Solid }
	{ FixedName False }
	{ Font "-*-courier-medium-r-normal--10*" }
	{ TextAlignment Center }
	{ TextColor "black" }
	{ NameUnderlined False }
}

Line 45
{
	{ View 5 }
	{ Subject 38 }
	{ FromShape 41 }
	{ ToShape 43 }
	{ Curved False }
	{ End1 Empty }
	{ End2 FilledArrow }
	{ Points 2 }
	{ Point 296 511 }
	{ Point 380 462 }
	{ NamePosition 331 478 }
	{ Color "black" }
	{ LineWidth 1 }
	{ LineStyle Solid }
	{ FixedName False }
	{ Font "-*-courier-medium-r-normal--10*" }
	{ TextAlignment Center }
	{ TextColor "black" }
	{ NameUnderlined False }
}

Line 46
{
	{ View 5 }
	{ Subject 39 }
	{ FromShape 42 }
	{ ToShape 41 }
	{ Curved False }
	{ End1 Empty }
	{ End2 FilledArrow }
	{ Points 2 }
	{ Point 370 564 }
	{ Point 296 557 }
	{ NamePosition 334 551 }
	{ Color "black" }
	{ LineWidth 1 }
	{ LineStyle Solid }
	{ FixedName False }
	{ Font "-*-courier-medium-r-normal--10*" }
	{ TextAlignment Center }
	{ TextColor "black" }
	{ NameUnderlined False }
}

Line 47
{
	{ View 5 }
	{ Subject 40 }
	{ FromShape 6 }
	{ ToShape 42 }
	{ Curved False }
	{ End1 Empty }
	{ End2 FilledArrow }
	{ Points 2 }
	{ Point 535 477 }
	{ Point 447 555 }
	{ NamePosition 482 509 }
	{ Color "black" }
	{ LineWidth 1 }
	{ LineStyle Solid }
	{ FixedName False }
	{ Font "-*-courier-medium-r-normal--10*" }
	{ TextAlignment Center }
	{ TextColor "black" }
	{ NameUnderlined False }
}

RoundedBox 55
{
	{ View 5 }
	{ Subject 48 }
	{ Position 60 640 }
	{ Size 103 30 }
	{ Color "black" }
	{ LineWidth 1 }
	{ LineStyle Solid }
	{ FillStyle Unfilled }
	{ FillColor "white" }
	{ FixedName False }
	{ Font "-*-courier-medium-r-normal--10*" }
	{ TextAlignment Center }
	{ TextColor "black" }
	{ NameUnderlined False }
}

Line 56
{
	{ View 5 }
	{ Subject 50 }
	{ FromShape 41 }
	{ ToShape 55 }
	{ Curved True }
	{ End1 Empty }
	{ End2 FilledArrow }
	{ Points 4 }
	{ Point 180 611 }
	{ Point 180 650 }
	{ Point 140 670 }
	{ Point 100 655 }
	{ NamePosition 166 678 }
	{ Color "black" }
	{ LineWidth 1 }
	{ LineStyle Solid }
	{ FixedName False }
	{ Font "-*-courier-medium-r-normal--10*" }
	{ TextAlignment Center }
	{ TextColor "black" }
	{ NameUnderlined False }
}

Line 57
{
	{ View 5 }
	{ Subject 51 }
	{ FromShape 55 }
	{ ToShape 41 }
	{ Curved True }
	{ End1 Empty }
	{ End2 FilledArrow }
	{ Points 4 }
	{ Point 90 625 }
	{ Point 90 600 }
	{ Point 140 580 }
	{ Point 164 580 }
	{ NamePosition 87 590 }
	{ Color "black" }
	{ LineWidth 1 }
	{ LineStyle Solid }
	{ FixedName False }
	{ Font "-*-courier-medium-r-normal--10*" }
	{ TextAlignment Center }
	{ TextColor "black" }
	{ NameUnderlined False }
}

RoundedBox 64
{
	{ View 5 }
	{ Subject 60 }
	{ Position 870 750 }
	{ Size 114 28 }
	{ Color "black" }
	{ LineWidth 1 }
	{ LineStyle Solid }
	{ FillStyle Unfilled }
	{ FillColor "white" }
	{ FixedName False }
	{ Font "-*-courier-medium-r-normal--10*" }
	{ TextAlignment Center }
	{ TextColor "black" }
	{ NameUnderlined False }
}

Line 65
{
	{ View 5 }
	{ Subject 61 }
	{ FromShape 26 }
	{ ToShape 64 }
	{ Curved True }
	{ End1 Empty }
	{ End2 FilledArrow }
	{ Points 4 }
	{ Point 726 731 }
	{ Point 770 710 }
	{ Point 810 710 }
	{ Point 849 736 }
	{ NamePosition 794 693 }
	{ Color "black" }
	{ LineWidth 1 }
	{ LineStyle Solid }
	{ FixedName False }
	{ Font "-*-courier-medium-r-normal--10*" }
	{ TextAlignment Center }
	{ TextColor "black" }
	{ NameUnderlined False }
}

Line 66
{
	{ View 5 }
	{ Subject 62 }
	{ FromShape 64 }
	{ ToShape 26 }
	{ Curved True }
	{ End1 Empty }
	{ End2 FilledArrow }
	{ Points 4 }
	{ Point 842 764 }
	{ Point 800 780 }
	{ Point 750 780 }
	{ Point 726 750 }
	{ NamePosition 775 787 }
	{ Color "black" }
	{ LineWidth 1 }
	{ LineStyle Solid }
	{ FixedName False }
	{ Font "-*-courier-medium-r-normal--10*" }
	{ TextAlignment Center }
	{ TextColor "black" }
	{ NameUnderlined False }
}

RoundedBox 80
{
	{ View 5 }
	{ Subject 68 }
	{ Position 550 610 }
	{ Size 118 24 }
	{ Color "black" }
	{ LineWidth 1 }
	{ LineStyle Solid }
	{ FillStyle Unfilled }
	{ FillColor "white" }
	{ FixedName False }
	{ Font "-*-courier-medium-r-normal--10*" }
	{ TextAlignment Center }
	{ TextColor "black" }
	{ NameUnderlined False }
}

RoundedBox 81
{
	{ View 5 }
	{ Subject 69 }
	{ Position 460 280 }
	{ Size 148 26 }
	{ Color "black" }
	{ LineWidth 1 }
	{ LineStyle Solid }
	{ FillStyle Unfilled }
	{ FillColor "white" }
	{ FixedName False }
	{ Font "-*-courier-medium-r-normal--10*" }
	{ TextAlignment Center }
	{ TextColor "black" }
	{ NameUnderlined False }
}

Line 82
{
	{ View 5 }
	{ Subject 72 }
	{ FromShape 6 }
	{ ToShape 81 }
	{ Curved True }
	{ End1 Empty }
	{ End2 FilledArrow }
	{ Points 4 }
	{ Point 525 370 }
	{ Point 480 360 }
	{ Point 450 330 }
	{ Point 460 293 }
	{ NamePosition 465 345 }
	{ Color "black" }
	{ LineWidth 1 }
	{ LineStyle Solid }
	{ FixedName False }
	{ Font "-*-courier-medium-r-normal--10*" }
	{ TextAlignment Center }
	{ TextColor "black" }
	{ NameUnderlined False }
}

Line 83
{
	{ View 5 }
	{ Subject 73 }
	{ FromShape 81 }
	{ ToShape 6 }
	{ Curved True }
	{ End1 Empty }
	{ End2 FilledArrow }
	{ Points 4 }
	{ Point 506 293 }
	{ Point 550 310 }
	{ Point 560 330 }
	{ Point 562 363 }
	{ NamePosition 555 320 }
	{ Color "black" }
	{ LineWidth 1 }
	{ LineStyle Solid }
	{ FixedName False }
	{ Font "-*-courier-medium-r-normal--10*" }
	{ TextAlignment Center }
	{ TextColor "black" }
	{ NameUnderlined False }
}

RoundedBox 84
{
	{ View 5 }
	{ Subject 70 }
	{ Position 150 410 }
	{ Size 121 24 }
	{ Color "black" }
	{ LineWidth 1 }
	{ LineStyle Solid }
	{ FillStyle Unfilled }
	{ FillColor "white" }
	{ FixedName False }
	{ Font "-*-courier-medium-r-normal--10*" }
	{ TextAlignment Center }
	{ TextColor "black" }
	{ NameUnderlined False }
}

Line 85
{
	{ View 5 }
	{ Subject 74 }
	{ FromShape 41 }
	{ ToShape 84 }
	{ Curved True }
	{ End1 Empty }
	{ End2 FilledArrow }
	{ Points 4 }
	{ Point 164 500 }
	{ Point 130 500 }
	{ Point 100 450 }
	{ Point 126 422 }
	{ NamePosition 100 475 }
	{ Color "black" }
	{ LineWidth 1 }
	{ LineStyle Solid }
	{ FixedName False }
	{ Font "-*-courier-medium-r-normal--10*" }
	{ TextAlignment Center }
	{ TextColor "black" }
	{ NameUnderlined False }
}

Line 86
{
	{ View 5 }
	{ Subject 75 }
	{ FromShape 84 }
	{ ToShape 41 }
	{ Curved True }
	{ End1 Empty }
	{ End2 FilledArrow }
	{ Points 4 }
	{ Point 174 422 }
	{ Point 210 440 }
	{ Point 220 470 }
	{ Point 200 489 }
	{ NamePosition 185 451 }
	{ Color "black" }
	{ LineWidth 1 }
	{ LineStyle Solid }
	{ FixedName False }
	{ Font "-*-courier-medium-r-normal--10*" }
	{ TextAlignment Center }
	{ TextColor "black" }
	{ NameUnderlined False }
}

Line 87
{
	{ View 5 }
	{ Subject 76 }
	{ FromShape 26 }
	{ ToShape 80 }
	{ Curved False }
	{ End1 Empty }
	{ End2 FilledArrow }
	{ Points 2 }
	{ Point 610 694 }
	{ Point 559 622 }
	{ NamePosition 555 661 }
	{ Color "black" }
	{ LineWidth 1 }
	{ LineStyle Solid }
	{ FixedName False }
	{ Font "-*-courier-medium-r-normal--10*" }
	{ TextAlignment Center }
	{ TextColor "black" }
	{ NameUnderlined False }
}

Line 88
{
	{ View 5 }
	{ Subject 77 }
	{ FromShape 80 }
	{ ToShape 6 }
	{ Curved False }
	{ End1 Empty }
	{ End2 FilledArrow }
	{ Points 2 }
	{ Point 553 598 }
	{ Point 585 477 }
	{ NamePosition 546 550 }
	{ Color "black" }
	{ LineWidth 1 }
	{ LineStyle Solid }
	{ FixedName False }
	{ Font "-*-courier-medium-r-normal--10*" }
	{ TextAlignment Center }
	{ TextColor "black" }
	{ NameUnderlined False }
}

RoundedBox 89
{
	{ View 5 }
	{ Subject 71 }
	{ Position 660 890 }
	{ Size 162 26 }
	{ Color "black" }
	{ LineWidth 1 }
	{ LineStyle Solid }
	{ FillStyle Unfilled }
	{ FillColor "white" }
	{ FixedName False }
	{ Font "-*-courier-medium-r-normal--10*" }
	{ TextAlignment Center }
	{ TextColor "black" }
	{ NameUnderlined False }
}

Line 90
{
	{ View 5 }
	{ Subject 78 }
	{ FromShape 26 }
	{ ToShape 89 }
	{ Curved True }
	{ End1 Empty }
	{ End2 FilledArrow }
	{ Points 4 }
	{ Point 659 806 }
	{ Point 680 820 }
	{ Point 680 860 }
	{ Point 673 877 }
	{ NamePosition 769 844 }
	{ Color "black" }
	{ LineWidth 1 }
	{ LineStyle Solid }
	{ FixedName False }
	{ Font "-*-courier-medium-r-normal--10*" }
	{ TextAlignment Center }
	{ TextColor "black" }
	{ NameUnderlined False }
}

Line 91
{
	{ View 5 }
	{ Subject 79 }
	{ FromShape 89 }
	{ ToShape 26 }
	{ Curved True }
	{ End1 Empty }
	{ End2 FilledArrow }
	{ Points 4 }
	{ Point 634 877 }
	{ Point 600 860 }
	{ Point 600 820 }
	{ Point 631 806 }
	{ NamePosition 556 848 }
	{ Color "black" }
	{ LineWidth 1 }
	{ LineStyle Solid }
	{ FixedName False }
	{ Font "-*-courier-medium-r-normal--10*" }
	{ TextAlignment Center }
	{ TextColor "black" }
	{ NameUnderlined False }
}

Line 93
{
	{ View 5 }
	{ Subject 92 }
	{ FromShape 26 }
	{ ToShape 42 }
	{ Curved True }
	{ End1 Empty }
	{ End2 FilledArrow }
	{ Points 4 }
	{ Point 574 722 }
	{ Point 500 730 }
	{ Point 430 640 }
	{ Point 430 585 }
	{ NamePosition 465 685 }
	{ Color "black" }
	{ LineWidth 1 }
	{ LineStyle Solid }
	{ FixedName False }
	{ Font "-*-courier-medium-r-normal--10*" }
	{ TextAlignment Center }
	{ TextColor "black" }
	{ NameUnderlined False }
}

RoundedBox 103
{
	{ View 5 }
	{ Subject 94 }
	{ Position 630 230 }
	{ Size 88 22 }
	{ Color "black" }
	{ LineWidth 1 }
	{ LineStyle Solid }
	{ FillStyle Unfilled }
	{ FillColor "white" }
	{ FixedName False }
	{ Font "-*-courier-medium-r-normal--10*" }
	{ TextAlignment Center }
	{ TextColor "black" }
	{ NameUnderlined False }
}

Line 104
{
	{ View 5 }
	{ Subject 97 }
	{ FromShape 6 }
	{ ToShape 103 }
	{ Curved True }
	{ End1 Empty }
	{ End2 FilledArrow }
	{ Points 4 }
	{ Point 600 363 }
	{ Point 600 320 }
	{ Point 600 270 }
	{ Point 619 241 }
	{ NamePosition 586 285 }
	{ Color "black" }
	{ LineWidth 1 }
	{ LineStyle Solid }
	{ FixedName False }
	{ Font "-*-courier-medium-r-normal--10*" }
	{ TextAlignment Center }
	{ TextColor "black" }
	{ NameUnderlined False }
}

Line 105
{
	{ View 5 }
	{ Subject 98 }
	{ FromShape 103 }
	{ ToShape 6 }
	{ Curved True }
	{ End1 Empty }
	{ End2 FilledArrow }
	{ Points 4 }
	{ Point 640 241 }
	{ Point 640 280 }
	{ Point 640 340 }
	{ Point 619 363 }
	{ NamePosition 655 309 }
	{ Color "black" }
	{ LineWidth 1 }
	{ LineStyle Solid }
	{ FixedName False }
	{ Font "-*-courier-medium-r-normal--10*" }
	{ TextAlignment Center }
	{ TextColor "black" }
	{ NameUnderlined False }
}

RoundedBox 106
{
	{ View 5 }
	{ Subject 95 }
	{ Position 380 820 }
	{ Size 160 26 }
	{ Color "black" }
	{ LineWidth 1 }
	{ LineStyle Solid }
	{ FillStyle Unfilled }
	{ FillColor "white" }
	{ FixedName False }
	{ Font "-*-courier-medium-r-normal--10*" }
	{ TextAlignment Center }
	{ TextColor "black" }
	{ NameUnderlined False }
}

Line 107
{
	{ View 5 }
	{ Subject 99 }
	{ FromShape 26 }
	{ ToShape 106 }
	{ Curved True }
	{ End1 Empty }
	{ End2 FilledArrow }
	{ Points 4 }
	{ Point 574 750 }
	{ Point 510 750 }
	{ Point 420 780 }
	{ Point 420 807 }
	{ NamePosition 400 757 }
	{ Color "black" }
	{ LineWidth 1 }
	{ LineStyle Solid }
	{ FixedName False }
	{ Font "-*-courier-medium-r-normal--10*" }
	{ TextAlignment Center }
	{ TextColor "black" }
	{ NameUnderlined False }
}

Line 108
{
	{ View 5 }
	{ Subject 100 }
	{ FromShape 106 }
	{ ToShape 26 }
	{ Curved True }
	{ End1 Empty }
	{ End2 FilledArrow }
	{ Points 4 }
	{ Point 460 820 }
	{ Point 530 820 }
	{ Point 560 810 }
	{ Point 574 792 }
	{ NamePosition 502 801 }
	{ Color "black" }
	{ LineWidth 1 }
	{ LineStyle Solid }
	{ FixedName False }
	{ Font "-*-courier-medium-r-normal--10*" }
	{ TextAlignment Center }
	{ TextColor "black" }
	{ NameUnderlined False }
}

RoundedBox 109
{
	{ View 5 }
	{ Subject 96 }
	{ Position 330 370 }
	{ Size 116 24 }
	{ Color "black" }
	{ LineWidth 1 }
	{ LineStyle Solid }
	{ FillStyle Unfilled }
	{ FillColor "white" }
	{ FixedName False }
	{ Font "-*-courier-medium-r-normal--10*" }
	{ TextAlignment Center }
	{ TextColor "black" }
	{ NameUnderlined False }
}

Line 110
{
	{ View 5 }
	{ Subject 101 }
	{ FromShape 6 }
	{ ToShape 109 }
	{ Curved True }
	{ End1 Empty }
	{ End2 FilledArrow }
	{ Points 4 }
	{ Point 524 388 }
	{ Point 480 370 }
	{ Point 440 360 }
	{ Point 388 365 }
	{ NamePosition 457 379 }
	{ Color "black" }
	{ LineWidth 1 }
	{ LineStyle Solid }
	{ FixedName False }
	{ Font "-*-courier-medium-r-normal--10*" }
	{ TextAlignment Center }
	{ TextColor "black" }
	{ NameUnderlined False }
}

Line 111
{
	{ View 5 }
	{ Subject 102 }
	{ FromShape 109 }
	{ ToShape 6 }
	{ Curved True }
	{ End1 Empty }
	{ End2 FilledArrow }
	{ Points 4 }
	{ Point 366 382 }
	{ Point 390 400 }
	{ Point 470 410 }
	{ Point 524 410 }
	{ NamePosition 427 410 }
	{ Color "black" }
	{ LineWidth 1 }
	{ LineStyle Solid }
	{ FixedName False }
	{ Font "-*-courier-medium-r-normal--10*" }
	{ TextAlignment Center }
	{ TextColor "black" }
	{ NameUnderlined False }
}

