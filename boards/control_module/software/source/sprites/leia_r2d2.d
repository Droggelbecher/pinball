import string_canvas: strings_to_frames;
import display: Color;
import std.datetime: Duration, msecs;
import std.typecons: Flag, Yes, No;
import coordinate: Coord;
import sprite: Sprite;

static leia_r2d2_frames = strings_to_frames(
		Coord(16, 40),
		[
			' ': Color.BLACK,
			'R': Color.RED,
			'r': Color.DARK_RED,
			'G': Color.GREEN,
			'g': Color.DARK_GREEN,
			'Y': Color.YELLOW,
			'O': Color.ORANGE,
			'o': Color.BLOOD_ORANGE,
		],
		[
q"X
                         oooo 
                       oooYYooo
                       oooYYooo
                          YY   
                         OOOO 
                       OOOOOOO      
        GGGG          OOOOOOOOO
      GGG GGGG       OO OOOOOOOO   
     GGGGGGGGGG      OO OOOOO OOO
     GGGGGRYGGG      OO OOOOOO OO
     GG   GGGGG       O OOOOOO O
     GGGGGGGGGG         OOOOOO
     GGGGGgggGG        OOOOOOOO
     GGGGGGGGGG        OOOOOOOOO
    GGGGGGGGGGGG      OOOOOOOOOO
    GGG  GG  GGG     ooo      oooo
X", q"X
                         oooo 
                       oooYYooo
                       oooYYooo
                          YY   
                         OOOO 
                       OOOOOOO      
        GGGG          OOOOOOOOO
      GGG GGGG        OO OOOOOOO   
     GGGGGGGGGG      OOO OOOO OOO
     GGGGGrYGGG      OO OOOOOO OO
     GG   GGGGG       O OOOOOO O
     GGGGGGGGGG         OOOOOO
     GGGGGgggGG        OOOOOOOO
     GGGGGGGGGG        OOOOOOOOO
    GGGGGGGGGGGG      OOOOOOOOOO
    GGG  GG  GGG     ooo      oooo
X", q"X
                         oooo 
                       ooYYYoo 
                       ooYYYoo 
                          YY   
                         OOOO 
                       OOOOOOO      
        GGGG          OOOOOOOOO
      GGG GGGG       OOOOOOOOOOO   
     GGGGGGGGGG      OOO OOOO OOO
     GGGGGrYGGG      OOO OOOOO OO
     GG   GGGGG     OOO OOOOOO O
     GGGGGGGGGG         OOOOOO
     GGGGGgggGG        OOOOOOOO
     GGGGGGGGGG       OOOOOOOOOO
    GGGGGGGGGGGG      OOOOOOOOOO
    GGG  GG  GGG     ooo      ooo
X"
		]
);

Sprite leia_r2d2_sprite;
static this() {
	leia_r2d2_sprite = new Sprite(leia_r2d2_frames, 500.msecs, Yes.loop);
};
