REBOL [title: "soccer"]

context [
    nums: charset [#"0" - #"9" "-."]
    players: [ "all" 1 2 3 4 5 6 7 8 9 10 11]

    soccer-style: stylize [
	player: image 12x12 effect [oval 255.255.255 key 255.255.255]
	abs-box: box effect [multiply 150.150.150] ibevel
	rel-box: box effect [tint 50] ibevel
    ]

    init_data: func [] [
        formationen: clear []
        name: none
        namen: clear []
        parse read %formation.conf [some [
        pos:
        "#" to newline
        | ":" copy name to newline (repend formationen [name copy []] append namen name)
        | newline
        | copy str some nums (append/only last formationen load str)
    ]]
    ]

    coo: func [x y] [
        to-pair reduce [
            to-integer 310 - (x * 600 / 105)
            to-integer 210 - (y * 400 / 68)
        ]
    ]

    ++: func [ 'word ] [ set word 1 + get word ]

    hilfe: layout [
    backdrop effect [gradient 1x1 0.0.0 0.0.180]
    title "Hilfe zu FormationViewer" red
    area wrap yellow 400x500 {
        Algemeines:
        Beim Start werden die Formationen aus der "formation.conf"
eingelesen. Sollte es diese Datei nicht geben ... naja, dann
bekommst du das hier auch nicht zu Gesicht ...

        Wenn die Datei geändert wird, kann sie durch 'reload' neu
eingelesen werden. Allerdings muss man dann an einen der
Einstelungen etwas machen, damit die Änderungen auch wirksam
werden.

        Funktionen:
        1. In der ersten Combo-Box kann die Formation ausgewählt werden.
        2. In der zweiten Combo-Box kann man einstellen, welchen Spieler man betrachten will.
        3. In den Checkboxen kann man eingeben, ob man die Home- und/oder MaxRange angezeigt haben
möchte.
    }
    button "Schliessen" [hide-popup]
    ]



    mem: none
    player: "all"
    display: func [name /local cx cy counter content content2 tmp m1 m2] [
	mem: name
	clear draw-pane
	clear drawer
        counter: 1
	content: clear []
	content2: clear []
	append content [
		styles soccer-style
	]
        foreach pos formationen/:name [
		cx: pos/3 + pos/1
		cy: pos/4 + pos/2
		if any [ counter = player player = "all" ] [
			m1: coo pos/7 - (pos/9 / 2) pos/8 - (pos/10 / 2)
			m2: coo pos/7 + (pos/9 / 2) pos/8 + (pos/10 / 2)
			if absolute [repend content [
				'at tmp: min m1 m2
				'abs-box abs (max m1 m2) - tmp
			]]
			m1: coo cx - (pos/5 / 2) cy - (pos/6 / 2)
			m2: coo cx + (pos/5 / 2) cy + (pos/6 / 2)
			if relative [repend content [
				'at tmp: min m1 m2
				'rel-box abs (max m1 m2) - tmp
			]]
		]
		repend content2 [
			'at (coo pos/1 pos/2) - 6x6
			'player either counter = player [ black ] [ red ]
			'with compose [number: (counter)]
			[player: face/number display mem]
                        ]

        repend drawer [
            'pen white 'fill-pen none
            'circle
                coo 0 0
                50
        ]
        repend drawer [
            'pen white 'fill-pen none
            'box
                coo 0 - 0.0 0 - 34
                coo 0 + 0.0 0 + 34
        ]
        repend drawer [
            'pen white 'fill-pen none
            'box
                coo 0 - 52.5 0 - 34
                coo 0 + 52.5 0 + 34
        ]

		++ counter
	]
	content: layout append content content2
	append draw-pane content/pane
	show draw-box
	]


    display_player: func [ number ] [
        player: number
        display mem
    ]

    init_data
    draw-box: none
    drawer: none
    draw-pane: none
    absolute: relative: none
    view layout [
        across backdrop green
        text "Formation:" tab choice data namen [ display first value]
        return
        text "Spieler:" tab choice data players [ display_player first value]
        check [absolute: value display mem ] text "MaxRange"
        check [relative: value display mem] text "HomeRange"
        return
	draw-box: box 620x420 leaf effect [draw []] with [pane: []]
        return
        across
        button "reload" [ init_data display mem ]
        button "help" [ inform hilfe ]
        tab
        tab
        tab
        button "quit" [quit]

        below
        do [
		drawer: tail draw-box/effect/draw
	        draw-pane: tail draw-box/pane
		display first namen
	]
    ]
]
